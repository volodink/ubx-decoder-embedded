from array import array

start_byte1 = 0xb5
start_byte2 = 0x62
id1 = 0x01
id2 = 0x02
lenh = 0x1c
lenl = 0x00

prefixRandomBytes = array('B', [0x31, 0x31, 0x31, 0x31, 0x31])

# construct message
dataArray = array('B', [])

#add some random bytes == noise or line crap

#construct first packet
dataArray.append(start_byte1)
dataArray.append(start_byte2)
dataArray.append(id1)
dataArray.append(id2)
dataArray.append(lenh)
dataArray.append(lenl)

for i in range(1, 28+1):
    dataArray.append(1)

#calc checksum
CK_A = 0
CK_B = 0

for elem in dataArray[2:]:
    CK_A = CK_A + elem
    CK_B += CK_A

CK_A &= 0xFF
CK_B &= 0xFF

#add checksum bytes to message
dataArray.append(CK_A)
dataArray.append(CK_B)

# create file and write message in it
dataFile = open('data.txt', 'wb')
prefixRandomBytes.tofile(dataFile)
dataArray.tofile(dataFile)
prefixRandomBytes.tofile(dataFile)
prefixRandomBytes.tofile(dataFile)
dataArray.tofile(dataFile)
prefixRandomBytes.tofile(dataFile)
dataArray.tofile(dataFile)
prefixRandomBytes.tofile(dataFile)
dataFile.close()

#show array
for i, a in enumerate(prefixRandomBytes):
    print i, hex(a)

for i, a in enumerate(dataArray):
    print i, hex(a)


