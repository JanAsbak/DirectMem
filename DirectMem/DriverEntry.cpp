#include <ntifs.h>
#include "memory/memory.h"

extern "C" VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrint("Driver Unloaded.\n");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;

	ULONG processId = 0;
	Memory mem(processId);

	


	DbgPrint("Driver Loaded.\n");
	return STATUS_SUCCESS;
}