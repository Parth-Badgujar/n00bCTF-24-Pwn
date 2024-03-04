#ifndef _SECCOMP_BPF_H_
#define _SECCOMP_BPF_H_

#define _GNU_SOURCE 1
#include <stdio.h>
#include <stddef.h>
// #include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <sys/prctl.h>
#ifndef PR_SET_NO_NEW_PRIVS
#define PR_SET_NO_NEW_PRIVS 38
#endif

#include <linux/unistd.h>
#include <linux/audit.h>
#include <linux/filter.h>
#ifdef HAVE_LINUX_SECCOMP_H
#include <linux/seccomp.h>
#endif
#ifndef SECCOMP_MODE_FILTER
#define SECCOMP_MODE_FILTER 2         /* uses user-supplied filter. */
#define SECCOMP_RET_KILL 0x00000000U  /* kill the task immediately */
#define SECCOMP_RET_TRAP 0x00030000U  /* disallow and force a SIGSYS */
#define SECCOMP_RET_ALLOW 0x7fff0000U /* allow */
struct seccomp_data
{
    int nr;
    __u32 arch;
    __u64 instruction_pointer;
    __u64 args[6];
};
#endif
#ifndef SYS_SECCOMP
#define SYS_SECCOMP 1
#endif

#define syscall_nr (offsetof(struct seccomp_data, nr))
#define arch_nr (offsetof(struct seccomp_data, arch))

#if defined(__i386__)
#define REG_SYSCALL REG_EAX
#define ARCH_NR AUDIT_ARCH_I386
#elif defined(__x86_64__)
#define REG_SYSCALL REG_RAX
#define ARCH_NR AUDIT_ARCH_X86_64
#else
#warning "Platform does not support seccomp filter yet"
#define REG_SYSCALL 0
#define ARCH_NR 0
#endif

#define VALIDATE_ARCHITECTURE                               \
    BPF_STMT(BPF_LD + BPF_W + BPF_ABS, arch_nr),            \
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, ARCH_NR, 1, 0), \
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_KILL)

#define EXAMINE_SYSCALL \
    BPF_STMT(BPF_LD + BPF_W + BPF_ABS, syscall_nr)

#define ALLOW_SYSCALL(name)                                 \
    BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, __NR_##name, 0, 1), \
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW)

#define KILL_PROCESS \
    BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_KILL)

#endif /* _SECCOMP_BPF_H_ */


__attribute__((constructor))
void setup(){
	setvbuf(stdout,NULL,2,0);
	setvbuf(stderr,NULL,2,0);
	setvbuf(stdin,NULL,2,0);
}

void activate_seccomp()
{
    struct sock_filter filter[] = {
        VALIDATE_ARCHITECTURE,
        EXAMINE_SYSCALL,
        ALLOW_SYSCALL(open),
        ALLOW_SYSCALL(sendfile),
        ALLOW_SYSCALL(exit),
        KILL_PROCESS, 
    };

    struct sock_fprog prog = {
        .len = (unsigned short)(sizeof(filter) / sizeof(struct sock_filter)),
        .filter = filter,
    };

    prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
    prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog);
}


int main(){
    char buf[400];
    puts("WELCOME TO THE WORLD OF SECURE COMPUTING"); 
    puts("Send me your code and I will execute it !") ;
    printf("Y0ur input is at : %p\n>> ", buf); 
    read(STDIN_FILENO, buf, 500) ;
    activate_seccomp() ;
    return 0 ;
}

