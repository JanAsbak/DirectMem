#pragma once
#include <ntifs.h>
#include <wdm.h>
#include "structs.h"

class Memory
{
private:
	PEPROCESS targetProcess;
	PHYSICAL_ADDRESS cr3 = { 0 };


public:
	Memory(ULONG processId);	// Constructor
	~Memory();					// Destructor

	PEPROCESS GetEprocess(ULONG processId);
	PHYSICAL_ADDRESS GetCr3(PEPROCESS process);

	UINT64 read(UINT64 va);
	NTSTATUS write();
};