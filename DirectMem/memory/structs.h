#pragma once
#include <ntifs.h>

typedef struct _EPROCESS
{
    CHAR padding[0x1d0];
    VOID* UniqueProcessId;                  // 0x1d0
    struct _LIST_ENTRY ActiveProcessLinks;  // 0x1d8

} EPROCESS, *PEPROCESS;