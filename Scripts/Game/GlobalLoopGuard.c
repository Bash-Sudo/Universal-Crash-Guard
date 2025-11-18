// =====================================================================
// GlobalLoopGuard.c
//  - Simple global per-frame loop guard
//  - No external dependencies (only console logging)
//  - Safe baseline to make the mod compile and run
// =====================================================================

// Required "entity class" for the game mode
class GlobalLoopGuardClass : SCR_BaseGameModeClass {};

// Actual game mode implementation
class GlobalLoopGuard : SCR_BaseGameMode
{
	// Global frame counter for this game-mode instance
	static int s_GlobalLoopCounter = 0;

	// Threshold before we consider it a runaway per-frame loop
	static const int MAX_FRAMES = 50000;

	// --------------------------------------------------------
	// Per-frame update
	// --------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		s_GlobalLoopCounter++;

		if (s_GlobalLoopCounter > MAX_FRAMES)
		{
			// Simple console message so we can see it fired
			PrintFormat(
				"[CrashGuard][GlobalLoop] Frame limit hit (%1) – possible runaway per-frame script. FrameCount=%2",
				MAX_FRAMES,
				s_GlobalLoopCounter
			);

			// Reset so we can detect another spike later
			s_GlobalLoopCounter = 0;

			// Soft-stop: skip base EOnFrame on this frame
			return;
		}

		// Normal behaviour
		super.EOnFrame(owner, timeSlice);
	}

	// --------------------------------------------------------
	// Optional helper – can be called from console/debug
	// --------------------------------------------------------
	static void ResetCounter()
	{
		s_GlobalLoopCounter = 0;
	}
}
