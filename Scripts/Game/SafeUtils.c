// RBR_Universal_Crash_Guard
// SafeUtils helpers used by CrashGuard (guards, loop monitors, CSI patches)

class SafeUtils
{
    /**
     * Central helper for "log + mitigate" style events.
     * - Writes the normal CrashGuard guard line
     * - Adds an extra detail line with instance / owner as strings
     */
    static void CrashGuard_LogAndMitigate(string context, string reason, Class instance = null, IEntity owner = null)
    {
        // Convert instance to string (or "null")
        string instStr;
        if (instance)
            instStr = instance.ToString();
        else
            instStr = "null";

        // Convert owner to string (or "null")
        string ownerStr;
        if (owner)
            ownerStr = owner.ToString();
        else
            ownerStr = "null";

        // 1) Normal guard line - this uses the main CrashGuard logging API
        CrashGuard_LogGuard(context, reason, instance, owner);

        // 2) Extra detail line tagged as SAFEUTIL to make filtering easier
        string detail = string.Format(
            "[SAFEUTIL] Context=%1 | Reason=%2 | Instance=%3 | Owner=%4",
            context,
            reason,
            instStr,
            ownerStr
        );

        CrashGuard_WriteLine(detail);
    }
};
