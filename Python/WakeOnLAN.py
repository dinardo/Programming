#!/usr/bin/env python

"""
Wake-on-LAN python code
by Mauro Dinardo
To get MAC addree remotely:
> arp -an | grep ip_address
"""

import socket
import sys
from numpy import uint32

if len(sys.argv) != 4:
    print('Usage: WakeOnLAN.py [IP-Addr] [Subnet-Mask] [MAC-Addr] (example: 212.189.204.67 255.255.255.0 b8:ca:3a:b2:91:a8 (for quark, or 212.189.204.72 255.255.255.0 e4:b9:7a:f1:a1:f4 for boson, or 212.189.204.73 255.255.255.0  00:68:eb:9b:73:65 for graviton))')
    sys.exit(1)

mac  = sys.argv[3]
data = ''.join(['FF' * 6, mac.replace(':', '') * 16])

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error as msg:
    print( 'Error establishing the socket:', msg)
    sys.exit(1)

print('Socket established')
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
ip        = sys.argv[1].split('.')
subnet    = sys.argv[2].split('.')
broadcast = [ int(i) | (~int(s) & 0xFF) for (i,s) in zip(ip,subnet) ]
strbroad  = ''
for b in broadcast:
    strbroad += str(b) + '.'
print(strbroad)
sock.sendto(bytes.fromhex(data), (strbroad[:-1], 9)) # Port = 9 is for Wake-on-LAN
