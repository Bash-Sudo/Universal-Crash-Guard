// CrashGuardMonitor.c
// Simple loop monitor used by SafePatch / loop guards.
// It just turns "loop stopped" events into CrashGuard monitor log lines.

class CrashGuardMonitor
{
    // Central entry point used by SafePatch (or other loop guards)
    static void RecordLoopStop(string context, int iterations, Class instance = null, IEntity owner = null)
    {
        // Very simple severity scale based on iteration count
        string severity = "LOW";

        if (iterations > 1000000)
            severity = "SEVERE";
        else if (iterations > 200000)
            severity = "MED";

        // Unified monitor logging helper (defined in CrashGuardLog.c)
        CrashGuard_LogMonitor(context, severity, iterations, instance, owner);
    }
};

// Ensure linker keeps this class around even if only referenced statically
static ref CrashGuardMonitor g_CrashGuardMonitor = new CrashGuardMonitor();
