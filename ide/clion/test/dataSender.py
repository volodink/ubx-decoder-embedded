import serial
from time import sleep

dataFile = open('data.txt', 'rb')

data = dataFile.read()

dataFile.close()

print len(data)
sleep(2)

ser = serial.Serial()
ser.baudrate = 57600
ser.port = '/dev/ttyUSB0'
ser.open()
sleep(1)

i = 0
for dataByte in data:
    ser.write(dataByte)
    sleep(0.1)
    print i, dataByte
    i += 1

ser.close()
