from array import array


def addstartbytes(packet):
    p = packet
    p.append(0xB5)
    p.append(0x62)
    return p


def calculatechecksum(data):
    ck_a = 0
    ck_b = 0

    for elem in data:
        ck_a = ck_a + elem
        ck_b += ck_a

    ck_a &= 0xFF
    ck_b &= 0xFF

    return array('B', [ck_a, ck_b])


def getPacket(ptype):
    packet = array('B', [])

    if ptype == 0:
        print 'POSLLH packet'
        packet = addstartbytes(packet)

        # UBX id 1
        packet.append(0x01)
        # UBX id 2
        packet.append(0x02)

        # payload length == 28
        packet.append(0x1C)
        packet.append(0x00)

        # payload
        for i in range(1, 29):
            packet.append(1)

        # add checksum
        for checksumByte in calculatechecksum(packet[2:]):
            packet.append(checksumByte)

        print 'Packet ready.'

    elif ptype == 1:
        print 'VELNED packet'
        packet = addstartbytes(packet)

        # TODO implement this code

    elif ptype == 2:
        print 'SOL packet'
        packet = addstartbytes(packet)

        # TODO implement this code

    elif ptype == 3:
        print 'Random or other packets data'
        for i in range(1, 6):
            packet.append(0x31)
    else:
        print 'Please choose number from 0 to 3. Returning empty packet.'

    return packet
