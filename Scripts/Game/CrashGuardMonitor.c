// scripts/Game/CrashGuardMonitor.c
// ------------------------------------------------------------
// Console + file watchdog for loop warnings.
// Called directly by SafeUtils.GuardLoop.
// ------------------------------------------------------------

class CrashGuardMonitor
{
    // Called from SafeUtils.GuardLoop when a loop is stopped.
    static void RecordLoopIssue(
        string context,
        int iterations,
        int maxIterations,
        Class instance,
        IEntity owner)
    {
        // Determine severity (basic heuristic)
        string severity = "LOW";

        float ratio = 0.0;
        if (maxIterations > 0)
        {
            ratio = iterations / (maxIterations * 1.0);
            if (ratio >= 5.0)
                severity = "HIGH";
            else if (ratio >= 2.0)
                severity = "MED";
        }

        // Convert instance
        string instStr;
        if (instance)
            instStr = instance.ToString();
        else
            instStr = "null";

        // Convert owner
        string ownerStr;
        if (owner)
            ownerStr = owner.GetName();
        else
            ownerStr = "null";

        // Console output
        PrintFormat(
            "[Crash Guard Monitor] Context=%1 | Severity=%2 | Iterations=%3/%4 | Instance=%5 | Owner=%6",
            context,
            severity,
            iterations,
            maxIterations,
            instStr,
            ownerStr
        );

        // File output
        CrashGuardLog.LogMonitorEvent(
            context,
            severity,
            iterations,
            maxIterations,
            instStr,
            ownerStr
        );
    }
}

// Ensure linker includes this class
static ref CrashGuardMonitor g_CrashGuardMonitor = new CrashGuardMonitor();
