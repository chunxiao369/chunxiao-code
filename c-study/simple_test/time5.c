#include <stdio.h>
#include <time.h>
#include <sys/sysinfo.h>

int main(void)
{
    time_t timep;
    struct sysinfo sys_i;
    time (&timep);
    printf("%s\n", asctime(gmtime(&timep)));
    printf("%s\n", ctime(&timep));

#if 0
           struct sysinfo {
               long uptime;             /* Seconds since boot */
               unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
               unsigned long totalram;  /* Total usable main memory size */
               unsigned long freeram;   /* Available memory size */
               unsigned long sharedram; /* Amount of shared memory */
               unsigned long bufferram; /* Memory used by buffers */
               unsigned long totalswap; /* Total swap space size */
               unsigned long freeswap;  /* Swap space still available */
               unsigned short procs;    /* Number of current processes */
               unsigned long totalhigh; /* Total high memory size */
               unsigned long freehigh;  /* Available high memory size */
               unsigned int mem_unit;   /* Memory unit size in bytes */
               char _f[20-2*sizeof(long)-sizeof(int)];
                                        /* Padding to 64 bytes */
           };
#endif
    sysinfo(&sys_i);
    printf("%lu MB, %lu MB.\n", sys_i.totalram >> 20, sys_i.freeram >> 20);
    return 0;
}
