#ifdef __AVR_ATmega328P__
#include <avr/io.h>
#include "bsl_loader.h"
#include "hal_out.h"
#include "_stdtype.h"
#include "vm.h"

#define Ack 0xcc
#define Nack 0x33

#define CommandReturn_Success 0x40
#define CommandReturn_UnknownCmd 0x41
#define CommandReturn_InvalidCmd 0x42
#define CommandReturn_InvalidAddr 0x43
#define CommandReturn_MemoryFail 0x44
#define CommandReturn_ChecksumFail 0x45
#define CommandReturn_Debug 0x46

#define CommandReceive_Ping 0x20
#define CommandReceive_Download 0x21
#define CommandReceive_Run 0x22
#define CommandReceive_GetStatus 0x23
#define CommandReceive_SendData 0x24
#define CommandReceive_Reset 0x25

#define SIZE_STATIC_MEMORY (1024)
#define MAX_PACKET_SIZE 16

u8 mem[SIZE_STATIC_MEMORY];
u8 currentPacket[MAX_PACKET_SIZE];

u8 status = CommandReturn_Success;

void processCommand(u8* packet);
void processDownloadCommand(u8* packet);
void processRunCommand(u8* packet);
void loadProgramSubRoutine(u32 address, u32 programSize);
void acknowledge();
void notAcknowledge();
u8 calculateChecksum(u8* packet);
u8 readByte(void);
void reset(void);
u8* readPacket(void);


u8* bsl_start_main_loop(int argc, char** argv)
{

	while (true)
	{

		u8* packet = readPacket();
		u8 checksum = calculateChecksum(packet);
		if (checksum != packet[1])
		{
			status = CommandReturn_ChecksumFail;
			notAcknowledge();
			continue;
		}


		processCommand(packet);


#ifndef KEEP_ALIVE
		return 0;
#endif
	}
}

void reset(void)
{
	int i;
	for (i = 0; i < SIZE_STATIC_MEMORY; i++) 
	{
		mem[i] = 0x00;
	}

	status = CommandReturn_Success;
	acknowledge();
}

void processCommand(u8* packet)
{
	u8 command = packet[2];
	switch (command)
	{
		case CommandReceive_Ping:
			acknowledge();
			break;
		case CommandReceive_Download:
			processDownloadCommand(packet);
			break;
		case CommandReceive_Run:
			acknowledge();
			processRunCommand(packet);
			break;
			break;
		case CommandReceive_GetStatus:
			acknowledge();
			VMOut_PutC(0);
			VMOut_PutC(0x03);
			VMOut_PutC(status);
			VMOut_PutC(status);
			break;
		case CommandReceive_Reset:
			reset();
			break;
		case CommandReceive_SendData:
			notAcknowledge();
			status = CommandReturn_InvalidCmd;
			break;
		default:
			notAcknowledge();
			VMOut_PutX(command);
			status = CommandReturn_UnknownCmd;

	}

}

void processDownloadCommand(u8* packet)
{
	u32 address = 0;
	u8* addresses = &address;
	addresses[0] = packet[6];
	addresses[1] = packet[5];
	addresses[2] = packet[4];
	addresses[3] = packet[3];

	u32 incomingProgramSize = 0;
	u8* incomingProgramSizes = &incomingProgramSize;
	incomingProgramSizes[0] = packet[10];
	incomingProgramSizes[1] = packet[9];
	incomingProgramSizes[2] = packet[8];
	incomingProgramSizes[3] = packet[7];

	if (address >= SIZE_STATIC_MEMORY)
	{
		notAcknowledge();
		status = CommandReturn_InvalidAddr;
		return;
	}

	if (address + incomingProgramSize >= SIZE_STATIC_MEMORY)
	{
		notAcknowledge();
		status = CommandReturn_MemoryFail;
		return;
	}


	loadProgramSubRoutine(address, incomingProgramSize);

}

void processRunCommand(u8* packet)
{
	u32 address = 0;
	u8* addresses = &address;
	addresses[0] = packet[6];
	addresses[1] = packet[5];
	addresses[2] = packet[4];
	addresses[3] = packet[3];

	acknowledge();

	VM_Init(mem + address);
	VM_execute(mem + address);

	// This is not part of the packet specs, however, we need a way to notify the host that we have completed executing the loaded program
	acknowledge();
}


void loadProgramSubRoutine(u32 address, u32 programSize)
{
	u8* memToUse = mem + address;
	int i = 0;
	for (i = 0; i < programSize; i++)
	{
		memToUse[i] = 0;
	}

	u32 programRemainingSize = programSize;

	status = CommandReturn_Success;
	acknowledge();

	i = 0;

	while (i < programSize)
	{
		u8* packet = readPacket();
		u8 checksum = calculateChecksum(packet);
		bool resetDone = false;
		if (checksum != packet[1])
		{
			notAcknowledge();
			continue;
		}

		int j;
		switch (packet[2])
		{
			case CommandReceive_SendData:
				for (j = 3; j < packet[0]; j++)
				{
					memToUse[i] = packet[j];
					i++;

					if (i == programSize)
					{
						break;
					}
				}

				acknowledge();
				break;
			case CommandReceive_Download:
			case CommandReceive_Run:
				notAcknowledge();
				status = CommandReturn_InvalidCmd;
				break;
			case CommandReceive_Reset:
				reset();
				resetDone = true;
				break;
			default:
				processCommand(packet);
		}


		if (resetDone)
		{
			break;
		}
	}



}

void acknowledge()
{
	VMOut_PutC(Ack);
	VMOut_PutC(0);
}

void notAcknowledge()
{
	VMOut_PutC(Nack);
	VMOut_PutC(0);
}

u8 calculateChecksum(u8* packet)
{
	int i = 2;
	u8 checksum = 0;
	for (i = 2; i < packet[0]; i++)
	{
		checksum += packet[i];
	}

	return checksum;
}

u8* readPacket(void)
{
	for (int i = 0; i < MAX_PACKET_SIZE; i++)
	{
		currentPacket[i] = 0;
	}
	u8 size = readByte();
	u8 checksum = readByte();

	currentPacket[0] = size;
	currentPacket[1] = checksum;
	int i = 2;
	for (i = 2; i < size; i++)
	{
		currentPacket[i] = readByte();
	}

	readByte(); // CLEAR THE 0

	return currentPacket;
}

// TODO USE HAL
u8 readByte(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	u8 byte = UDR0;
	return byte;
}



#endif
