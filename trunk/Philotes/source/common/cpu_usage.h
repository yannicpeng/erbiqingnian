// ---------------------------------------------------------------------------
// FILE: cpu_usage.h
// ---------------------------------------------------------------------------
#ifndef _CPU_USAGE_H_
#define _CPU_USAGE_H_


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
struct CPU_USAGE;


// ---------------------------------------------------------------------------
// EXPORTED FUNCTIONS
// ---------------------------------------------------------------------------
CPU_USAGE * CpuUsageInit(
	struct MEMORYPOOL * mempool);

void CpuUsageFree(
	CPU_USAGE * cpu);

int CpuUsageGetValue(
	CPU_USAGE * cpu);


#endif // _CPU_USAGE_H_
