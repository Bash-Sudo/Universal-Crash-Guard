# RBR Universal Crash Guard

*A plug-and-play safety layer for Arma Reforger servers that protects against bad mod behavior, runaway loops, and VM crashes — without relying on mod authors to fix their code.*

![Stability Badge](https://img.shields.io/badge/Server_Stability-Enhanced-brightgreen)
![Arma Reforger](https://img.shields.io/badge/Arma-Reforger-orange)
![Crash Guard Active](https://img.shields.io/badge/Crash_Guard-Active-blue)

---

## Overview
The **RBR Universal Crash Guard** provides a universal safety net for Reforger servers running multiple mods. It catches bad behavior, prevents infinite loops, monitors global frame usage, and logs detailed information administrators can use to identify problem mods — all without requiring updates from mod authors.

---
## Why This Exists
Servers running many community mods often face instability from:
- Missing null checks  
- Infinite loops  
- Heavy `EOnFrame` execution  
- Bad update pushes  

Crash Guard functions like a seatbelt — catching failures before they take the entire server down.

---
## Installation

### **Server Installation**
1. Download the mod files and place them into your server’s `addons/` directory.
2. Add the mod entry to your server’s modlist JSON.
3. Start the server and confirm this appears in the console/logs:  
   **[CrashShield] Universal SafeGuard loaded.**

### **Optional Setup**
- Create custom guards inside `SafePatches.c` if you encounter a misbehaving mod.
- Use the included templates to add loop caps, null-checks, and safety wrappers.
---
## Core Components

### **Universal Bootstrap**
Initializes the guard system and confirms loading.

### **Safe Utilities**
Provides:
- Null safety checks  
- Loop protection  
- Structured logging  
- Context/instance/owner tracing  

### **Crash Guard Monitor**
Assigns severity levels (LOW, MED, HIGH) to loop events and writes them to logs.

### **Crash Guard Logger**
Outputs all structured events to:  
`$profile:CrashGuard.log`

### **Global Loop Guard**
A system-wide frame counter that detects runaway frame scripts.

### **Safe Patch System**
Where admins can create per-mod safety guards.

### **Integration Templates**
Reserved for future patch collections and mod-specific examples.

---
## Admin Workflow

1. Install the addon and restart the server.  
2. When instability occurs, read `CrashGuard.log`.  
3. Look for HIGH/MED severity entries and repeated contexts.  
4. Check `[GLOBAL]` entries for full server frame overloads.  
5. Use AR Explorer to trace issues back to scripts.  
6. Build guards in `SafePatches.c` to prevent repeat issues.
---
## Contributing

Contributions are welcome!  
Ways to contribute:
- Improve documentation  
- Add templates  
- Submit bug fixes  
- Submit new mod integration patches  

**How to Contribute**
1. Fork the repository  
2. Create a feature branch  
3. Commit your changes  
4. Submit a pull request  
---
## License
Arama Reforger and its associated assets are trademarks of Bohemia Interactive. APL-ND License applies to this project.
---
## Credits
Would like to thank the Rubber Band Rangers Dev Team along with:
- LoneStarActual [BTGG] 
- Rouge [RES] 
- Brooks [OG] 
