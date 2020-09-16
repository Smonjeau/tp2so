/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_CPU.C    |                                                                             |
|----------------------                                                                             |
| These functions implement syscalls related to the CPU.   	                                        |
---------------------------------------------------------------------------------------------------*/

#include <cpu_info.h>
#include <lib.h>

char brandDescs[22][70] = {
    "This processor does not support the brand identification feature",
    "Intel(R) Celeron(R) processor",
    "Intel(R) Pentium(R) III processor",
    "Intel(R) Pentium(R) III Xeon(R) processor",
    "Intel(R) Pentium(R) III processor",
    "Mobile Intel(R) Pentium(R) III processor-M",
    "Mobile Intel(R) Celeron(R) processor",
    "Intel(R) Pentium(R) 4 processor",
    "Intel(R) Pentium(R) 4 processor",
    "Intel(R) Celeron(R) processor",
    "Intel(R) Xeon(R) processor",
    "Intel(R) Xeon(R) processor MP",
    "Mobile Intel(R) Pentium(R) 4 processor-M",
    "Mobile Intel(R) Celeron(R) processor",
    "Mobile Genuine Intel(R) processor",
    "Intel(R) Celeron(R) M processor",
    "Mobile Intel(R) Celeron(R) processor",
    "Intel(R) Celeron(R) processor",
    "Mobile Genuine Intel(R) processor",
    "Intel(R) Pentium(R) M processor",
    "Mobile Intel(R) Celeron(R) processor"
};


/* ---------------------------------------------------------------------------------------------------------------------------
                                                    CPUINFO
--------------------------------------------------------------------------------------------------------------------------- */

int sysCpuInfo(CPUInfo * info){

    getBrandName(info->brandName);
    
    memcpy(info->brandDesc, brandDescs[getBrandIndex()], 70);

    return 0;

}


/* ---------------------------------------------------------------------------------------------------------------------------
                                                    CPUTEMP
--------------------------------------------------------------------------------------------------------------------------- */

int sysCPUTemp(){

    return getTemperature();

}