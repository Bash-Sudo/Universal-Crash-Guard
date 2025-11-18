// CrashGuard_CSI_Patches.c
// Patches for CSI_PlayerDataManager / CSI_PlayerData to prevent
// NULL-pointer VM crashes and log useful info into CrashGuard.log.

modded class CSI_PlayerDataManager : ScriptComponent
{
	// ------------------------------------------------------------------------
	//  Helper: central CSI logging hook
	// ------------------------------------------------------------------------
	protected void CrashGuard_LogCSI(string context, string reason)
	{
		// Goes into CrashGuard.log (implemented in CrashGuard_Log.c)
		CrashGuard_LogGlobal(
			"CSI_PlayerDataManager",
			context,
			reason
		);
	}

	// ------------------------------------------------------------------------
	//  Helper: validate / repair the internal player data map
	//  - removes null entries
	//  - repairs bad IDs (where stored ID != key)
	// ------------------------------------------------------------------------
	protected void CheckPlayerDataValidity()
	{
		int count = m_mPlayerDataMap.Count();
		if (count <= 0)
			return;

		// iterate backwards so Remove() is always safe
		for (int i = count - 1; i >= 0; i--)
		{
			int playerID = m_mPlayerDataMap.GetKeyByIndex(i);
			CSI_PlayerData data = m_mPlayerDataMap.Get(playerID);

			// null entry – remove it
			if (!data)
			{
				CrashGuard_LogCSI(
					"CheckPlayerDataValidity",
					string.Format("Removed NULL CSI_PlayerData entry for playerID=%1", playerID)
				);

				m_mPlayerDataMap.Remove(playerID);
				continue;
			}

			// mismatch between key and stored ID – repair it
			int internalID = data.GetPlayerID();
			if (internalID != playerID)
			{
				CrashGuard_LogCSI(
					"CheckPlayerDataValidity",
					string.Format("ID mismatch | mapKey=%1 | m_iPlayerID=%2 – repairing stored ID",
						playerID, internalID)
				);

				data.SetPlayerID(playerID);
			}

			// future invariants (icon, rank etc.) can be checked here
		}
	}

	// ------------------------------------------------------------------------
	//  Safe DataUpdate override
	//  - blocks NULL playerData
	//  - logs what happened
	//  - calls original RPC so CSI still works
	// ------------------------------------------------------------------------
	override protected void DataUpdate(CSI_PlayerData playerData)
	{
		// NULL guard – this is what was blowing up in the VM
		if (!playerData)
		{
			CrashGuard_LogCSI(
				"DataUpdate",
				"DataUpdate called with NULL playerData (blocked, CSI call skipped)"
			);
			return;
		}

		int playerID = playerData.GetPlayerID();

		// basic log so we can correlate with CrashGuard.log later
		CrashGuard_LogCSI(
			"DataUpdate",
			string.Format("Safe DataUpdate for playerID=%1", playerID)
		);

		// let the original behaviour run so CSI continues to function
		RpcDo_PlayerDataUpdate(playerData);
		Rpc(RpcDo_PlayerDataUpdate, playerData);
	}

	// ------------------------------------------------------------------------
	// Optional: hook from your GlobalLoopGuard or a debug button
	// ------------------------------------------------------------------------
	void CrashGuard_DebugCheckCSI()
	{
		CheckPlayerDataValidity();
	}
}

// ------------------------------------------------------------------------
// If you also added Extract / Inject patches earlier and want them kept,
// you can leave them in this same file *below* the class above.
// The important change for this compile error is the new
// CheckPlayerDataValidity() implementation.
// ------------------------------------------------------------------------
