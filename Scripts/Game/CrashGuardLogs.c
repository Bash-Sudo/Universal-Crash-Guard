// scripts/Game/CrashGuardLog.c
// ------------------------------------------------------------
// Plain-text logging for Crash Guard (Arma Reforger).
// Writes to: $profile:CrashGuard.log
// No HTML, just simple lines for easy parsing and sharing.
// ------------------------------------------------------------

class CrashGuardLog
{
    protected static string s_FilePath = "$profile:CrashGuard.log";
    protected static bool   s_Initialized;

    // Ensure the log file exists and has a header
    protected static void EnsureFile()
    {
        if (s_Initialized)
            return;

        FileHandle fh = FileIO.OpenFile(s_FilePath, FileMode.WRITE);
        if (!fh)
        {
            PrintFormat("[CrashGuardLog] ERROR: Failed to open '%1' for writing.", s_FilePath);
            return;
        }

        fh.WriteLine("==== CrashGuard Log ====");
        fh.WriteLine("Format:");
        fh.WriteLine("  [GUARD]   Context | Reason | Instance | Owner");
        fh.WriteLine("  [MONITOR] Context | Severity | Iterations | Instance | Owner");
        fh.WriteLine("  [GLOBAL]  Context | Reason | FrameCount");
        fh.WriteLine("===============================================");
        fh.Close();

        s_Initialized = true;
        PrintFormat("[CrashGuardLog] Logging to '%1'", s_FilePath);
    }

    protected static void AppendLine(string line)
    {
        EnsureFile();
        if (!s_Initialized)
            return; // failed to init

        FileHandle fh = FileIO.OpenFile(s_FilePath, FileMode.APPEND);
        if (!fh)
        {
            PrintFormat("[CrashGuardLog] ERROR: Could not append to '%1'", s_FilePath);
            return;
        }

        fh.WriteLine(line);
        fh.Close();
    }

    // --------------------------------------------------------
    // Public helpers used by SafeUtils / CrashGuardMonitor / GlobalLoopGuard
    // --------------------------------------------------------

    static void LogGuardSummary(string context, string reason)
    {
        string line = string.Format("[GUARD]   %1 | %2", context, reason);
        AppendLine(line);
    }

    static void LogGuardDetail(string context, string reason, string instance, string owner)
    {
        string line = string.Format("[GUARD]   %1 | %2 | %3 | %4", context, reason, instance, owner);
        AppendLine(line);
    }

    static void LogMonitorEvent(
        string context,
        string severity,
        int iterations,
        int maxIterations,
        string instance,
        string owner)
    {
        string line = string.Format(
            "[MONITOR] %1 | %2 | %3/%4 | %5 | %6",
            context,
            severity,
            iterations,
            maxIterations,
            instance,
            owner
        );
        AppendLine(line);
    }

    // Global loop / frame-level events (EOnFrame watchdog)
    static void LogGlobalEvent(string context, string reason, int frameCount)
    {
        string line = string.Format(
            "[GLOBAL]  %1 | %2 | FrameCount=%3",
            context,
            reason,
            frameCount
        );
        AppendLine(line);
    }
}
