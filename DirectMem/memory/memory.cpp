#include "memory.h"

Memory::Memory(ULONG processId)
{
	
}

Memory::~Memory()
{

}

PEPROCESS GetEprocess(ULONG processId)
{
	PEPROCESS current = (PEPROCESS)PsInitialSystemProcess;
	PLIST_ENTRY listHead = &current->ActiveProcessLinks;
	PLIST_ENTRY listNext = listHead->Flink;

	while (listNext != listHead)
	{
		PEPROCESS entry = CONTAINING_RECORD(listNext, EPROCESS, ActiveProcessLinks);

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
	PHYSICAL_ADDRESS cr3 = { 0 };
	cr3.QuadPart = *(ULONG_PTR*)((UCHAR)process + 0x28); // x64 DirectoryTableBase offset

	return cr3;
}

