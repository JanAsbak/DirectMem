#pragma once
#include <ntifs.h>
#include "structs.h"

class Memory
{
private:
	PEPROCESS targetProcess;


public:
	Memory(ULONG processId); // Constructor
	~Memory(); // Destructor
	PEPROCESS GetEprocess(ULONG processId);
	PHYSICAL_ADDRESS GetCr3(PEPROCESS process);
	void read();
	void write();
};