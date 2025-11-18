// scripts/Game/SafePatches.c
// ------------------------------------------------------------
// Extension bay for per-mod patches.
// Add your `modded class SomeModComponent` definitions here
// and wrap their loops / risky methods with SafeUtils guards.
// ------------------------------------------------------------

class CrashGuardSafePatches
{
    // Example template (commented out):
    //
    // modded class WCS_SomeHeavyComponent
    // {
    //     protected int m_LoopGuard;
    //
    //     override void EOnFrame(IEntity owner, float timeSlice)
    //     {
    //         if (!SafeUtils.GuardEntity(owner, "WCS_SomeHeavyComponent::EOnFrame", this))
    //             return;
    //
    //         if (!SafeUtils.GuardLoop(
    //             m_LoopGuard,
    //             10000,
    //             "WCS_SomeHeavyComponent::EOnFrame",
    //             this,
    //             owner))
    //         {
    //             return; // loop capped & logged
    //         }
    //
    //         super.EOnFrame(owner, timeSlice);
    //     }
    // }
}
