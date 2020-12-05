/* HostSerialLoader.cs
//
// Copyright (C) 2020 by Michel de Champlain
//
*/
#define LoadFromFile

using System;
using System.Collections;
using System.IO;
using System.IO.Ports;
using System.Threading;


public class SerialComPort {
    private const byte Ack = 0xCC;
    private const byte Nak = 0x33;
    private const byte PacketSizeMax = 11;

    private enum Status : byte {
        Success = 0x40,
        // Errors during CheckPacket():
        UnknownCmd,
        InvalidCmd,  

        // Errors before command execution:
        InvalidAddr,

        // Errors after command execution:
        MemoryFailed,
        ChecksumInvalid
    }
    private enum Cmd : byte {
        CmdBegin = 0x20,
        Ping = CmdBegin,
        Download,
        Run,
        GetStatus,
        SendData,
        Reset,
        CmdEnd
    }

    // Note: The last zero for each packet is to fullfill the specification
    // of the basic packet format. The Host sends zeros until a non-zero
    // response is received from the target. The Target sends zeros until
    // it is ready to Ack or Nak the packet that has being sent by the Host.
    // See Section 1.1 Basic Packet Format in the Serial Memory Loader
    // document.
    static byte[] pingPacketChecksumInvalid = { 0x03, 0xee, (byte)Cmd.Ping, 0 };
    static byte[] pingPacket = { 0x03, 0x20, (byte)Cmd.Ping, 0 };
    static byte[] getStatusPacket = { 0x03, 0x23, (byte)Cmd.GetStatus, 0 };
    static byte[] sendDataPacket = { 0x09, 0xBC, 0x24, 0x91, 0xFF, 0x82, 0xFF, 0x87, 0x00, 0 };
    static byte[] runPacket = { 0x03, 0x22, (byte)Cmd.Run, 0 };

    static bool       _continue;
    static bool       _run;
    static SerialPort _serialPort;

    static byte[] buffer = new byte[12];

#if LoadFromFile
    // Get the executable code into a formatted byte buffer ready to send with checksum.
    // Example: If a file.exe has a length    of 8  bytes [size(2)          + pgm(6)          ]
    //          then it needs a buffer length of 10 bytes [size|cksm|cmd(3) + pgm(6) + zero(1)] 
    //                                                       0    1    2      3..n-2     n-1
    static byte[] GetCode(string exeFilename) {
        // Read the file into a byte array.
        var fs = new FileStream(exeFilename, FileMode.Open);
        int fileLength  = (int)fs.Length;

        // The length of the executable section
        int filePgmSize = fileLength - 2;

        // 1 Download packet + SendData packets
        int numDataPackets = 1 + (int)(Math.Ceiling(filePgmSize / 8F) + 0.5);

        byte[] buffer = new byte[numDataPackets * PacketSizeMax];

        // Read the first two bytes to skip the size of the executable.
        fs.Read(buffer, 0, 2);

        // Setup Download command
        buffer[0] = 11;
        buffer[2] = 0x21;
        buffer[3] = 0x00;
        buffer[4] = 0x00;
        buffer[5] = 0x00;
        buffer[6] = 0x00;
        buffer[7] = (byte)(filePgmSize >> 24);
        buffer[8] = (byte)(filePgmSize >> 16);
        buffer[9] = (byte)(filePgmSize >> 8);
        buffer[10] = (byte)(filePgmSize);

        // Compute download checksum
        for(int i = 2; i <= 10; i++)
        {
            buffer[1] += buffer[i];
        }

        // Create SendData packets
        for(int i = 1; i < numDataPackets; i++)
        {
            buffer[(i * PacketSizeMax) + 0] = 11;
            buffer[(i * PacketSizeMax) + 2] = 0x24;
            fs.Read(buffer, (i * PacketSizeMax) + 3, 8);

            // Compute download checksum
            for (int j = (i * PacketSizeMax) + 2; j <= (i * PacketSizeMax) + 10; j++)
            {
                buffer[(i * PacketSizeMax) + 1] += buffer[j];
            }
        }

        return buffer;
    }
    
    // Main thread to transmit packets to target (reading .exe files).
    public static void Main(string[] args) {
        if (args.Length != 1) {
            Console.WriteLine("Usage: HostSerialLoader.exe <file.exe>");
            return;
        }
        string exeFilename = args[0];
//t        Console.WriteLine("file = {0}", exeFilename);

        // Get the executable code in a buffer to build packet(s).
        var buf = GetCode(exeFilename);

        byte[] sendDataPacketFile = new byte[buf.Length];

        for (int n = 0; n < buf.Length; ++n) {
//t            Console.Write("{0:X2} ", buf[n]);
            sendDataPacketFile[n] = buf[n];
        }

           Console.WriteLine("\nsendDataPacket.Length = {0}.", sendDataPacketFile.Length);
            Console.WriteLine("\nbuf.Length = {0}.", buf.Length);
            Console.WriteLine("\nfile = {0} loaded.", exeFilename);
#else
    // Main thread to transmit packets to target (with hardcoded packets).
    public static void Main() {
#endif
        StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;
        Thread readThread = new Thread(ReadByte);

        // Create a new SerialPort with the same Arduino Nano settings.
        _serialPort = new SerialPort();
        _serialPort.PortName = "COM3";
        _serialPort.BaudRate = 9600; ;
        _serialPort.Parity = Parity.None;
        _serialPort.DataBits = 8;
        _serialPort.StopBits = StopBits.One;
        _serialPort.Handshake = Handshake.None;

        // Set the read/write timeouts
        _serialPort.ReadTimeout = 500;
        _serialPort.WriteTimeout = 500;

        _serialPort.Open();
        _continue = true;
        _run = false;

        // Start a second thread to receive bytes from target.
        readThread.Start();

        Console.WriteLine("Host Serial Loader v1.0 (Cm Virtual Machine on Arduino Nano)");
        Console.WriteLine("Usage: type 'p'(ping), 'd'(download),'s' (status), 'e' (sendData), 'r'(run), 't'(reset), and 'q' to quit.");

        string cmd;

        // Send cmd to target using a command prompt (for debugging purpose).
        Console.Write("$ ");
        while (_continue)
        {
            cmd = Console.ReadLine();

            if (stringComparer.Equals("q", cmd))
            {
                _continue = false;
            }
            else if (stringComparer.Equals("p", cmd))
            { // ping
                _serialPort.Write(pingPacket, 0, 4);
            }
            else if (stringComparer.Equals("t", cmd))
            { // ping
                _serialPort.Write(resetPacket, 0, 4);
            }
            else if (stringComparer.Equals("s", cmd))
            { // getStatus
                _serialPort.Write(getStatusPacket, 0, 4);
            }
            else if (stringComparer.Equals("e", cmd))
            { // download (sendData - small pgm)
#if LoadFromFile
                _serialPort.Write(sendDataPacketFile, 0, sendDataPacketFile.Length);
#else
                _serialPort.Write(sendDataPacket, 0, 10);
#endif
            }
            else if (stringComparer.Equals("d", cmd))
            { // getStatus
                _serialPort.Write(downloadDataPacket, 0, downloadDataPacket.Length);
            }
            else if (stringComparer.Equals("r", cmd))
            { // run
                _serialPort.Write(runPacket, 0, 4);
                _run = true;
            }
            else
            {
                _serialPort.Write(pingPacketChecksumInvalid, 0, 4);
            }
        }

        Console.WriteLine("bye!");
        readThread.Join();
        _serialPort.Close();
    }

    // Synchronously reads one byte from the SerialPort input buffer (from target).
    public static void ReadByte()
    {
        while (_continue)
        {
            try
            {
                int size = _serialPort.Read(buffer, 0, 1);
                //t                Console.Write("size[" + string.Format("{0:X2}", buffer[0]) + "]:");
                if (buffer[0] != 0)
                {
                    do
                    {
                        if (!_run && (buffer[0] == Ack))
                        {
                            size = _serialPort.Read(buffer, 0, 1); // read the zero
                            Console.Write("Ack from target\n$ ");
                            break;
                        }

                        if (_run && (buffer[0] == Ack))
                        {
                            size = _serialPort.Read(buffer, 0, 1); // read the zero
                            Console.Write("Ack from target. Run!\n");
                            //t                            Console.Write("it's run + ack/zero " + string.Format("{0:X2} ", buffer[0]));
                            break;
                        }

                        if (!_run && (buffer[0] == Nak))
                        {
                            size = _serialPort.Read(buffer, 0, 1); // read the zero
                            Console.Write("Nak from target\n$ ");
                            break;
                        }

                        if (_run && (buffer[0] == Nak))
                        {
                            size = _serialPort.Read(buffer, 0, 1); // read the zero
                            Console.Write("Nak from target. Run!\n");
                            //t                            Console.Write("it's run + ack/zero " + string.Format("{0:X2} ", buffer[0]));
                            break;
                        }

                        size = _serialPort.Read(buffer, 0, 1);
                        Console.Write(string.Format("{0:X2} ", buffer[0]));
                    } while ((buffer[0] != 0));
                }

                if (_run)
                {
                    while (true)
                    {
                        size = _serialPort.Read(buffer, 0, 1);

                        if (buffer[0] == Ack)
                        {
                            size = _serialPort.Read(buffer, 0, 1); // read the zero
                                                                   //t                            Console.Write("running is done + ack/zero " + string.Format("{0:X2} ", buffer[0]));
                            break;
                        }
                        Console.Write((char)buffer[0]);
                    }
                    _run = false;
                    Console.Write("$ ");
                }
            }
            catch (TimeoutException) { }
        }
    }
}
