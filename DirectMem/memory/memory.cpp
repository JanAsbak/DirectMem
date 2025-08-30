#include "memory.h"

Memory::Memory(ULONG processId)
{
	targetProcess = GetEprocess(processId);
	cr3 = GetCr3(targetProcess);
}

Memory::~Memory()
{

}

PEPROCESS Memory::GetEprocess(ULONG processId)
{
	MYPEPROCESS current = (MYPEPROCESS)PsInitialSystemProcess;
	PLIST_ENTRY listHead = &current->ActiveProcessLinks;
	PLIST_ENTRY listNext = listHead->Flink;

	while (listNext != listHead)
	{
		MYPEPROCESS entry = CONTAINING_RECORD(listNext, MYEPROCESS, ActiveProcessLinks);

		if ((ULONG_PTR)entry->UniqueProcessId == processId)
		{
			return (PEPROCESS)entry;
		}

		listNext = listNext->Flink;
	}

	return NULL;
}

PHYSICAL_ADDRESS Memory::GetCr3(PEPROCESS process)
{
	if (!process)
		return { 0 };

	PHYSICAL_ADDRESS cr3 = { 0 };
	cr3.QuadPart = *(ULONG_PTR*)((UCHAR*)process + 0x28); // x64 DirectoryTableBase offset

	return cr3;
}

UINT64 Memory::read(UINT64 va)
{
	UINT64 pml4_index	= (va >> 39) & 0x1FF;
	UINT64 pdpt_index	= (va >> 30) & 0x1FF;
	UINT64 pd_index		= (va >> 21) & 0x1FF;
	UINT64 pt_index		= (va >> 12) & 0x1FF;
	UINT64 offset		= va & 0xFFF;

	UINT64 pml4_phys = (cr3.QuadPart & ~0xFFFULL);
	

}



