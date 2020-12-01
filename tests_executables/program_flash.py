import serial
import sys
print(sys.argv[1])
print(sys.argv[2])

serial = serial.Serial(sys.argv[2])

file = open(sys.argv[1], "rb")

send = bytearray()
count = 0

while 1:
    read = file.read(1)
    if(not read):
        break
    serial.write(read)

#serial.write(send)

while 1:
    if serial.in_waiting > 0:
        string = serial.readline()
        print(string.decode('Ascii'))

file.close()
serial.close()

