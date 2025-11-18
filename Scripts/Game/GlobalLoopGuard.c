// -------------------------------------------------------------------------
// GlobalLoopGuard.c
// Simple global frame-loop guard used by CrashGuard.
// Attach this as a game mode / script to have it active.
// -------------------------------------------------------------------------

// Forward declaration so we can call GetInstance()
class CSI_PlayerDataManager;

// Required entity-class wrapper for game mode
class GlobalLoopGuardClass : SCR_BaseGameModeClass
{
};

// GlobalLoopGuard implementation
class GlobalLoopGuard : SCR_BaseGameMode
{
    // Global frame counter for this game-mode instance
    static int s_GlobalLoopCounter = 0;

    // Threshold before we consider it a runaway per-frame loop
    static const int MAX_FRAMES = 50000;

    // ---------------------------------------------------------------------
    // Per-frame update
    // ---------------------------------------------------------------------
    override void EOnFrame(IEntity owner, float timeSlice)
    {
        s_GlobalLoopCounter++;

        // Hit the limit -> treat as possible runaway script
        if (s_GlobalLoopCounter > MAX_FRAMES)
        {
            string reason = string.Format(
                "Global EOnFrame frame limit hit (%1) - possible runaway per-frame script",
                MAX_FRAMES
            );

            // Summary line into CrashGuard.log (simple API)
            // CrashGuard_LogGlobal(string context, string reason, int frameCount = 0);
            CrashGuard_LogGlobal("GlobalLoop", reason, s_GlobalLoopCounter);

            // Optional CSI cleanup / validation hook
            CSI_PlayerDataManager manager = CSI_PlayerDataManager.GetInstance();
            if (manager)
                manager.CheckPlayerDataValidity();

            // Reset so we can detect another spike later
            s_GlobalLoopCounter = 0;

            // Soft-stop: skip base EOnFrame this frame
            return;
        }

        // Normal behaviour when not tripping the guard
        super.EOnFrame(owner, timeSlice);
    }

    // ---------------------------------------------------------------------
    // Optional helper if you ever want to reset from script/console
    // ---------------------------------------------------------------------
    static void ResetCounter()
    {
        s_GlobalLoopCounter = 0;
    }
}
