// CrashGuard_CSI_Patches.c
// Safety patches for CSI_PlayerDataManager to avoid NULL m_iPlayerID crashes
// and to give CrashGuard visibility into bad player data entries.

modded class CSI_PlayerDataManager : ScriptComponent
{
    // Helper: log a simple guard entry for CSI problems
    protected void CrashGuard_LogCSI(string reason)
    {
        CrashGuard_LogGuard("CSI_PlayerDataManager", reason, this, null);
    }

    // Helper: log with extra detail (e.g. playerID)
    protected void CrashGuard_LogCSIPlayer(string label, int playerID)
    {
        string reason = string.Format("%1 for playerID=%2", label, playerID);
        CrashGuard_LogGuard("CSI_PlayerDataManager", reason, this, null);
    }

    // ---------------------------------------------------------------------
    // Map health check: cleans NULL entries and fixes mismatched IDs
    // Called from GlobalLoopGuard when a runaway frame is detected, and can
    // also be called manually if needed.
    // ---------------------------------------------------------------------
    void CheckPlayerDataValidity()
    {
        if (!m_mPlayerDataMap)
            return;

        int count = m_mPlayerDataMap.Count();
        if (count <= 0)
            return;

        // Iterate backwards so Remove() is always safe
        for (int i = count - 1; i >= 0; i--)
        {
            int key = m_mPlayerDataMap.GetKey(i);
            CSI_PlayerData data = m_mPlayerDataMap.GetElement(i);

            // Remove NULL entries
            if (!data)
            {
                CrashGuard_LogCSIPlayer("Removed NULL CSI_PlayerData entry", key);
                m_mPlayerDataMap.Remove(key);
                continue;
            }

            // Repair mismatched IDs (map key vs stored m_iPlayerID)
            int internalID = data.GetPlayerID();
            if (internalID != key)
            {
                string msg = string.Format(
                    "Mismatched m_iPlayerID detected: mapKey=%1, m_iPlayerID=%2 – repairing stored ID",
                    key,
                    internalID
                );
                CrashGuard_LogGuard("CSI_PlayerDataManager", msg, data, null);

                // Force stored ID to match the map key
                data.SetPlayerID(key);
            }
        }
    }

    // ---------------------------------------------------------------------
    // Safe DataUpdate override
    // Blocks obviously bad data before it reaches CSI’s original logic.
    // ---------------------------------------------------------------------
    override protected void DataUpdate(CSI_PlayerData playerData)
    {
        // Null guard – this was one of the crash paths in your VM logs
        if (!playerData)
        {
            CrashGuard_LogCSI("DataUpdate called with NULL playerData (blocked)");
            return;
        }

        int playerID = playerData.GetPlayerID();

        // Invalid or uninitialized IDs cause serialization / RPC issues
        if (playerID <= 0)
        {
            CrashGuard_LogCSIPlayer("DataUpdate blocked due to invalid m_iPlayerID", playerID);
            return;
        }

        // Everything looks sane – hand off to original CSI logic
        super.DataUpdate(playerData);
    }
};
