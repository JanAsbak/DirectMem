# DirectMem
# VA → PA manual Translation

A Windows kernel driver demonstrating **manual virtual-to-physical address translation**.

---

## Features
- Manual VA → PA translation via page table walk
- Minimal reliance on Windows APIs

---

## Build
- Windows 10/11 x64  
- Visual Studio + Windows Driver Kit (WDK)  
- Test signing enabled or code-signed driver certificate

### Steps
1. Open the project in Visual Studio  
2. Build for x64 / Release  
3. Load the driver:
```powershell
sc create DirectMem type=kernel binPath="C:\path\to\DirectMem.sys"
sc start DirectMem
