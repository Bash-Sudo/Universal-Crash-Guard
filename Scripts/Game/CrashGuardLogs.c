// CrashGuardLog.c
// Core logging helpers used by CrashGuard, GlobalLoopGuard and CSI patches.
// NOTE: We only use Print / PrintFormat so this works in both Workbench and
// dedicated server without relying on OpenFile / FPrintln / CloseFile.

static const string CRASHGUARD_LOG_PATH = "$profile:CrashGuard.log"; // informational only

// Internal flag so we print the header once
static bool s_CrashGuardHeaderPrinted = false;

//--------------------------------------------------------------------------
// Ensure the banner/header is printed once into the normal game log
//--------------------------------------------------------------------------
static void CrashGuard_EnsureHeader()
{
    if (s_CrashGuardHeaderPrinted)
        return;

    Print("==== CrashGuard Log ====");
    Print("Format:");
    Print("  [GUARD]   Context | Reason | Instance | Owner");
    Print("  [MONITOR] Context | Severity | Iterations | Instance | Owner");
    Print("  [GLOBAL]  Context | Reason | FrameCount");
    Print("==============================================");

    s_CrashGuardHeaderPrinted = true;
}

//--------------------------------------------------------------------------
// Single place where we actually emit the line
//--------------------------------------------------------------------------
static void CrashGuard_WriteLine(string line)
{
    CrashGuard_EnsureHeader();
    Print(line);      // Goes to console + normal RPT/server log
}

//--------------------------------------------------------------------------
// BASIC GUARD LOG
// Used by SafePatch, loop guards, CSI patches, etc.
//--------------------------------------------------------------------------
void CrashGuard_LogGuard(string context, string reason, Class instance = null, IEntity owner = null)
{
    CrashGuard_EnsureHeader();

    string instStr;
    if (instance)
        instStr = instance.ToString();
    else
        instStr = "null";

    string ownerStr;
    if (owner)
        ownerStr = owner.ToString();
    else
        ownerStr = "null";

    string line = string.Format(
        "[GUARD] %1 | %2 | %3 | %4",
        context,
        reason,
        instStr,
        ownerStr
    );

    CrashGuard_WriteLine(line);
}

//--------------------------------------------------------------------------
// MONITOR LOG
// Used by CrashGuardMonitor / loop monitors to record iterations, etc.
//--------------------------------------------------------------------------
void CrashGuard_LogMonitor(string context, string severity, int iterations, Class instance = null, IEntity owner = null)
{
    CrashGuard_EnsureHeader();

    string instStr;
    if (instance)
        instStr = instance.ToString();
    else
        instStr = "null";

    string ownerStr;
    if (owner)
        ownerStr = owner.ToString();
    else
        ownerStr = "null";

    string line = string.Format(
        "[MONITOR] %1 | %2 | Iterations=%3 | %4 | %5",
        context,
        severity,
        iterations,
        instStr,
        ownerStr
    );

    CrashGuard_WriteLine(line);
}

//--------------------------------------------------------------------------
// GLOBAL FRAME GUARD LOG
// Used by GlobalLoopGuard to flag runaway EOnFrame situations.
//--------------------------------------------------------------------------
void CrashGuard_LogGlobal(string context, string reason, int frameCount)
{
    CrashGuard_EnsureHeader();

    string line = string.Format(
        "[GLOBAL] %1 | %2 | FrameCount=%3",
        context,
        reason,
        frameCount
    );

    CrashGuard_WriteLine(line);
}

//--------------------------------------------------------------------------
// COMPAT WRAPPER CLASS
// If any scripts still call CrashGuardLog.LogGuard/LogMonitor/LogGlobal,
// this tiny class forwards those calls to the global helpers above.
//--------------------------------------------------------------------------
class CrashGuardLog
{
    static void LogGuard(string context, string reason, Class instance = null, IEntity owner = null)
    {
        CrashGuard_LogGuard(context, reason, instance, owner);
    }

    static void LogMonitor(string context, string severity, int iterations, Class instance = null, IEntity owner = null)
    {
        CrashGuard_LogMonitor(context, severity, iterations, instance, owner);
    }

    static void LogGlobal(string context, string reason, int frameCount)
    {
        CrashGuard_LogGlobal(context, reason, frameCount);
    }
};
