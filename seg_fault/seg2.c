#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <ucontext.h>
static void myhandler(unsigned int sn, siginfo_t si, struct ucontext *sc)
{
    unsigned int mnip;
    int i, j;
    printf(" Signal number = %d, Signal errno = %d\n", si.si_signo, si.si_errno);
    switch (si.si_code) {
    case 1:
        printf(" SI code = %d (Illegal opcode)\n", si.si_code);
        break;
    case 2:
        printf(" SI code = %d (Illegal operand)\n", si.si_code);
        break;
    case 3:
        printf(" SI code = %d (Illegal addressing mode)\n", si.si_code);
        break;
    case 4:
        printf(" SI code = %d (Illegal trap)\n", si.si_code);
        break;
    case 5:
        printf(" SI code = %d (Privileged opcode)\n", si.si_code);
        break;
    case 6:
        printf(" SI code = %d (Privileged register)\n", si.si_code);
        break;
    case 7:
        printf(" SI code = %d (Coprocessor error)\n", si.si_code);
        break;
    case 8:
        printf(" SI code = %d (Internal stack error)\n", si.si_code);
        break;
    default:
        printf("SI code = %d (Unknown SI Code)\n", si.si_code);
        break;
    }
    printf(" Machine State Register = %x \n", (((struct pt_regs *)((&(sc->uc_mcontext))->regs))->msr));
    printf(" Link register pointing to location = 0x%x, \
            Opcode at the location = 0x%x \n", (((struct pt_regs *)((&(sc->uc_mcontext))->regs))->link), *(unsigned int *)
           (((struct pt_regs *)((&(sc->uc_mcontext))->regs))->link));
    for (i = 20, j = 5; i > 0; i -= 4, j--)
        printf(" Op-Code [nip - %d] = 0x%x at address = 0x%x \n", j, *(unsigned int *)(si.si_addr - i)
               , (si.si_addr - i));
    printf(" Failed Op-code    = 0x%x at address = 0x%x \n", *(unsigned int *)(si.si_addr), (si.si_addr));
    printf(" Op-Code [nip + 1] = 0x%x at address = 0x%x \n", *(unsigned int *)(si.si_addr + 4), (si.si_addr + 4));
    (((struct pt_regs *)((&(sc->uc_mcontext))->regs))->nip) += 4;
}

my()
{
    __asm__ volatile ("add 4,5,6 \n\t":);
    __asm__ volatile ("add 7,8,9 \n\t":);
    __asm__ volatile ("mfmsr 3 \n\t":);
    __asm__ volatile ("add 4,5,6 \n\t":);
    __asm__ volatile ("add 7,8,9 \n\t":);
}

void dummy_func(void)
{
    printf("hello world\n");
    char *p = NULL;             //0地址
    *p = 0x1a;                  //对0地址写入数据，出现段错误
    return;
}

main()
{
    struct sigaction s;
    s.sa_flags = SA_SIGINFO;
    s.sa_sigaction = (void *)myhandler;
    if (sigaction(SIGSEGV, &s, (struct sigaction *)NULL)) {
        printf("Sigaction returned error = %d\n", errno);
        exit(0);
    }
    dummy_func();
    sleep(1);
    return 0;
}
