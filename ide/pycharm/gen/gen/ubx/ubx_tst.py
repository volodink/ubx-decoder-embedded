from ubx import *

data = getPacket(0)
for i in data:
    print hex(i)
