#!/usr/bin/env python

import sys, struct

argv = sys.argv[1:]

def to_le(n):
	return struct.pack('<i', n)

def from_le(s):
	return struct.unpack('<i', s)[0]

def push_string(s):
	return '\xe8' + to_le(len(s)+1) + s + '\0'

payload = open(sys.path[0]+"/elfloader").read()
elf_entry = from_le(payload[0x18:0x1c])

payload = "\xe8"+to_le(len(payload)-5) + payload[5:]

payload += '\x6a\x00' + ''.join(push_string(arg) for arg in reversed(argv)) + '\x50'
payload += '\xe8' + to_le(elf_entry-len(payload)-5)

sys.stdout.write(payload)

