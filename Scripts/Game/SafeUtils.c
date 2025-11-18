// scripts/Game/SafeUtils.c
// ------------------------------------------------------------
// Core utility functions for Crash Guard:
//  - null checks
//  - logging
//  - loop guard + monitor hook
//  - writes info into CrashGuard.log via CrashGuardLog
// ------------------------------------------------------------

class SafeUtils
{
    // --------------------------------------------------------
    // Basic validity checks
    // --------------------------------------------------------
    static bool IsEntityValid(IEntity e)
    {
        return e != null;
    }

    static bool IsObjectValid(Class obj)
    {
        return obj != null;
    }

    // --------------------------------------------------------
    // Logging helpers
    // --------------------------------------------------------
    static void Log(string msg)
    {
        PrintFormat("[CrashShield] %1", msg);
    }

    static void LogGuardMitigated(string context, string reason)
    {
        PrintFormat("[Crash Guard] Context=%1 | Mitigated=%2", context, reason);
        // Summary line in file
        CrashGuardLog.LogGuardSummary(context, reason);
    }

    static void LogGuardInfo(string context, string reason, Class instance, IEntity owner)
    {
        string instStr;
        if (instance)
            instStr = instance.ToString();
        else
            instStr = "null";

        string ownerStr;
        if (owner)
        {
            string ownerName = owner.GetName();
            ownerStr = string.Format("%1 (%2)", ownerName, owner);
        }
        else
        {
            ownerStr = "null";
        }

        PrintFormat(
            "[Crash Guard Info] Context=%1 | Reason=%2 | Instance=%3 | Owner=%4",
            context, reason, instStr, ownerStr
        );

        // Detailed line in file
        CrashGuardLog.LogGuardDetail(context, reason, instStr, ownerStr);
    }

    // --------------------------------------------------------
    // Guards
    // --------------------------------------------------------

    // Guard an owner entity
    static bool GuardEntity(IEntity owner, string context, Class instance)
    {
        if (owner)
            return true;

        LogGuardMitigated(context, "owner == null");
        LogGuardInfo(context, "owner == null", instance, owner);
        return false;
    }

    // Guard any script object / pointer
    static bool GuardNotNull(Class obj, string context, Class instance, IEntity owner)
    {
        if (obj)
            return true;

        LogGuardMitigated(context, "object == null");
        LogGuardInfo(context, "object == null", instance, owner);
        return false;
    }

    // Guard loops against runaway iteration counts
    //
    // Example:
    //   int m_LoopGuard;
    //   while (condition)
    //   {
    //       if (!SafeUtils.GuardLoop(m_LoopGuard, 10000, "MyComp::Loop", this, owner))
    //           break;
    //       ...
    //   }
    static bool GuardLoop(inout int counter, int maxIterations, string context, Class instance, IEntity owner)
    {
        counter++;

        if (counter <= maxIterations)
            return true;

        string reason = string.Format("loop iteration limit reached (%1)", maxIterations);

        LogGuardMitigated(context, reason);
        LogGuardInfo(context, reason, instance, owner);

        // Notify the monitor (console + file)
        CrashGuardMonitor.RecordLoopIssue(context, counter, maxIterations, instance, owner);

        // Reset so future loops don’t instantly trigger again
        counter = 0;

        return false;
    }
}
