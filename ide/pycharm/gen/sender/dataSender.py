import serial
from time import sleep

# read data file content
dataFile = open('data.txt', 'rb')
data = dataFile.read()
dataFile.close()

# serial port create and open
ser = serial.Serial()
ser.baudrate = 57600
ser.port = '/dev/ttyUSB0'
ser.open()

print 'Prepare to send ...'
sleep(1)


# send data and count bytes send
i = 0
for dataByte in data:
    ser.write(dataByte)
    sleep(0.1)
    print i, dataByte
    i += 1

# closing port and finishing sending
ser.close()
