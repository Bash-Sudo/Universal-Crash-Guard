// scripts/Game/CrashGuard.c
// ------------------------------------------------------------
// Bootstrap: confirms that the Crash Guard addon is loaded.
// ------------------------------------------------------------

class UniversalCrashGuard
{
    void UniversalCrashGuard()
    {
        Print("[CrashShield] Universal SafeGuard loaded.");
    }
}

// Global instance so the constructor runs once at load time
static ref UniversalCrashGuard g_UniversalCrashGuard = new UniversalCrashGuard();
