from ubx import *

print 'Writing data file ...'

# create file and write message in it
dataFile = open('../sender/data.txt', 'wb')

packet = getPacket(3)
packet.tofile(dataFile)

packet = getPacket(0)
packet.tofile(dataFile)

packet = getPacket(3)
packet.tofile(dataFile)

packet = getPacket(0)
packet.tofile(dataFile)

packet = getPacket(3)
packet.tofile(dataFile)

packet = getPacket(0)
packet.tofile(dataFile)

packet = getPacket(3)
packet.tofile(dataFile)

dataFile.close()

print 'Writing data file ... done'
