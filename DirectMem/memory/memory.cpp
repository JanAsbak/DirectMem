#include "memory.h"

static inline BOOLEAN ENTRY_PRESENT(UINT64 e) { return (e & 1ULL) != 0; }
static inline BOOLEAN ENTRY_PS(UINT64 e) { return (e & (1ULL << 7)) != 0; }
static inline UINT64  ENTRY_PFN(UINT64 e) { return (e >> 12); }
static inline UINT64  ENTRY_PHYS_BASE(UINT64 e, unsigned level_shift)
{
	UINT64 mask = ~((1ULL << level_shift) - 1);
	return e & mask;
}

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

	PHYSICAL_ADDRESS phys;
	PVOID mapped;
	UINT64 entry;
	UINT64 base_phys;
	
	// CR3
	base_phys = cr3.QuadPart & ~0xFFFULL; // pml4 base
	phys.QuadPart = base_phys;


	// PML4
	mapped = MmMapIoSpace(phys, PAGE_SIZE, MmNonCached);
	entry = ((UINT64*)mapped)[pml4_index];
	MmUnmapIoSpace(mapped, PAGE_SIZE);

	if (!ENTRY_PRESENT(entry)) return 0;

	// PDPT
	base_phys = ENTRY_PFN(entry) << 12;
	mapped = MmMapIoSpace(phys, PAGE_SIZE, MmNonCached);
	entry = ((UINT64*)mapped)[pdpt_index];
	MmUnmapIoSpace(mapped, PAGE_SIZE);

	if (!ENTRY_PRESENT(entry)) return 0;
	if (ENTRY_PS(entry)) // 1GB page
	{
		UINT64 phys_base = ENTRY_PHYS_BASE(entry, 30);
		return phys_base + (va & ((1ULL << 30) - 1));
	}

	// PD
	entry = ((UINT64*)mapped)[pd_index];
	if (!ENTRY_PRESENT(entry)) return 0;
	if (ENTRY_PS(entry)) // 2MB page
	{
		UINT64 phys_base = ENTRY_PHYS_BASE(entry, 21);
		return phys_base + (va & ((1ULL << 21) - 1));
	}

	// PT
	entry = ((UINT64*)mapped)[pt_index];
	if (!ENTRY_PRESENT(entry)) return 0;

	UINT64 page_base = ENTRY_PHYS_BASE(entry, 12);

	return page_base + offset;
}



