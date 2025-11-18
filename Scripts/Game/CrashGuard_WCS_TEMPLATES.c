// scripts/Game/CrashGuard_WCSTemplates.c
// -------------------------------------------------------------
// WCS compatibility templates for RBRUniversalCrashGuard
// These are *templates* only: you must copy the exact function
// signatures (return type + parameters) from your WCS .c files,
// then uncomment the blocks you want to activate.
// -------------------------------------------------------------


// =============================================================
// 1) WCS_Armament_OpticalSeekerComponent
//    Logged crashes (from your script.log):
//
//    Class:      'WCS_Armament_OpticalSeekerComponent'
//      Scripts/Game/Components/Weapons/GuidedWeapon/Seekers/
//          WCS_Armament_OpticalSeekerComponent.c:44 Function CalculateTargetPosition
//      Scripts/Game/Components/Weapons/GuidedWeapon/Seekers/
//          WCS_Armament_OpticalSeekerComponent.c:27 Function Update
//      Scripts/Game/Systems/WCS_Armament_SeekerSystem.c:23 Function OnUpdatePoint
//
//    Likely cause: NULL target / missile / owner while seeker
//    is trying to calculate positions / update guidance.
// =============================================================

/*
// STEP 1: check the original file:
//   Scripts/Game/Components/Weapons/GuidedWeapon/Seekers/
//       WCS_Armament_OpticalSeekerComponent.c
//
// Find the class + function header, then copy the *exact* signature
// over this placeholder.
//
modded class WCS_Armament_OpticalSeekerComponent : ScriptComponent
{
    // Guard counters (used with SafeUtils.GuardLoop if you have inner loops)
    protected int m_CG_SeekerCalcLoopGuard;
    protected int m_CG_SeekerUpdateLoopGuard;

    // --- TEMPLATE FOR CalculateTargetPosition -----------------
    // Replace this with the real header from WCS_Armament_OpticalSeekerComponent.c
    // Example only:
    //     void CalculateTargetPosition(float timeSlice)
    //
    void CalculateTargetPosition(/* REAL PARAMS HERE * /)
    {
        IEntity owner = GetOwner();

        // Guard owner entity – if null, don’t even try to calculate
        if (!SafeUtils.GuardEntity(owner,
            "WCS_Armament_OpticalSeekerComponent::CalculateTargetPosition",
            this))
            return;

        // If the seeker uses a target ref like m_Target / m_Lock / m_TrackedEntity,
        // guard it here. Replace "m_Target" with the real field name.
        if (!SafeUtils.GuardNotNull(m_Target,
            "WCS_Armament_OpticalSeekerComponent::CalculateTargetPosition",
            this,
            owner))
            return;

        // If this function contains any while/for loop that can go crazy,
        // put GuardLoop inside that loop body. Example:
        //
        //   while (condition)
        //   {
        //       if (!SafeUtils.GuardLoop(m_CG_SeekerCalcLoopGuard, 20000,
        //           "WCS_Armament_OpticalSeekerComponent::CalculateTargetPositionLoop",
        //           this, owner))
        //           break;
        //       ...
        //   }

        // All guards passed – run the original behaviour.
        super.CalculateTargetPosition(/* same params * /);
    }

    // --- TEMPLATE FOR Update / EOnFrame / OnUpdate ------------
    // In your logs, line 27 in this file is "Function Update".
    // Copy the exact header from the original script, then wrap it.
    //
    void Update(/* REAL PARAMS HERE * /)
    {
        IEntity owner = GetOwner();

        if (!SafeUtils.GuardEntity(owner,
            "WCS_Armament_OpticalSeekerComponent::Update",
            this))
            return;

        // Guard any critical state used in Update (target, missile, controller).
        // Replace "m_Target" etc. with real fields.
        if (!SafeUtils.GuardNotNull(m_Target,
            "WCS_Armament_OpticalSeekerComponent::Update",
            this,
            owner))
            return;

        // If Update has an internal loop, protect it as well:
        //   if (!SafeUtils.GuardLoop(m_CG_SeekerUpdateLoopGuard, 20000,
        //       "WCS_Armament_OpticalSeekerComponent::UpdateLoop",
        //       this, owner))
        //       return;

        super.Update(/* same params * /);
    }
}
*/



// =============================================================
// 2) SCR_ArsenalManagerComponent WCS loadout extensions
//    Logged crashes:
//
//    Class:      'SCR_ArsenalManagerComponent'
//    Function:   'WCS_GetPlayerLoadoutData_S'
//      Scripts/Game/GameMode/Loadout/WCS_LoadoutEditor_ArsenalManagerComponent.c:70
//      Scripts/Game/GameMode/Loadout/WCS_LoadoutEditor_PlayerArsenalLoadout.c:42
//      Scripts/Game/GameMode/Loadout/WCS_LoadoutEditor_ArsenalManagerComponent.c:575
//      Scripts/Game/GameMode/Loadout/WCS_LoadoutEditor_ArsenalManagerComponent.c:490
//      Scripts/Game/GameMode/WCS_Core_GameModeCampaign.c:62
//
//    Likely cause: NULL loadout / player data / config when the WCS
//    arsenal tries to fetch or calculate supply costs.
// =============================================================

/*
// STEP 2: open
//   Scripts/Game/GameMode/Loadout/WCS_LoadoutEditor_ArsenalManagerComponent.c
// Find:
//   function WCS_GetPlayerLoadoutData_S( ... )
// Copy the exact header (return type + params) over this placeholder.
//
modded class SCR_ArsenalManagerComponent
{
    // Example: if the real function returns some data struct, change RETURN_TYPE
    // to that struct/class.
    RETURN_TYPE WCS_GetPlayerLoadoutData_S(/* REAL PARAMS HERE * /)
    {
        // Choose something meaningful for "owner" – e.g. the game mode,
        // or use GetOwner() if this component is attached to an entity.
        IEntity owner = GetOwner();

        // If this function uses an internal loadout map (e.g. m_Loadouts,
        // m_PlayerLoadouts, m_Config), guard it with GuardNotNull.
        if (!SafeUtils.GuardNotNull(this,
            "SCR_ArsenalManagerComponent::WCS_GetPlayerLoadoutData_S",
            this,
            owner))
        {
            // Safe default – no crash, but log the mitigation.
            RETURN_TYPE safeDefault;
            return safeDefault;
        }

        // Example extra guard for some field:
        // if (!SafeUtils.GuardNotNull(m_LoadoutData,
        //     "SCR_ArsenalManagerComponent::WCS_GetPlayerLoadoutData_S/m_LoadoutData",
        //     this, owner))
        // {
        //     RETURN_TYPE safeDefault;
        //     return safeDefault;
        // }

        return super.WCS_GetPlayerLoadoutData_S(/* same params * /);
    }
}
*/



// =============================================================
// HOW TO USE THIS FILE
// =============================================================
//
// 1. Leave everything COMMENTED OUT until you’re ready to patch a
//    specific crash.
// 2. When you pick one crash to guard:
//
//    a) Open the original WCS script at the path shown in your log.
//    b) Copy the *exact* function header into the corresponding template
//       (return type + params).
//    c) Replace placeholder fields like "m_Target", "RETURN_TYPE" with
//       real names from the script.
//    d) Uncomment that whole modded class block (remove the /* ... */).
//
// 3. Recompile in Workbench.
// 4. On the next crash attempt, you should see:
//
//    [Crash Guard]  Mitigated potential crash in <Context> ...
//    [Crash Guard Info] ...
//    [Crash Guard Monitor] Context=<Context> | Severity=... | Trips=... | ...
//
// 5. Use those log lines + the folder-based template we made earlier to
//    document which loops / functions are being protected and whether you
//    need deeper fixes in the original mods.
