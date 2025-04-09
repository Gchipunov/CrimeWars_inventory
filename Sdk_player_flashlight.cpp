// Add these includes at the top if not already present
#include "iefx.h"
#include "effects.h"

// Add these member variables to the CSDKPlayer class definition in sdk_player.h
class CSDKPlayer : public CBasePlayer
{
public:
    DECLARE_CLASS(CSDKPlayer, CBasePlayer);
    DECLARE_SERVERCLASS();
    // ... existing declarations ...
    void ToggleFlashlight(void);
    bool m_bFlashlightOn;
    CBaseEntity* m_pFlashlight; // Pointer to the flashlight entity
};

// In sdk_player.cpp, initialize the variables
CSDKPlayer::CSDKPlayer()
{
    m_bFlashlightOn = false;
    m_pFlashlight = NULL;
}

// Define the flashlight toggle function
void CSDKPlayer::ToggleFlashlight(void)
{
    if (m_bFlashlightOn)
    {
        // Turn off
        if (m_pFlashlight)
        {
            m_pFlashlight->Remove();
            m_pFlashlight = NULL;
        }
        m_bFlashlightOn = false;
    }
    else
    {
        // Turn on: Spawn an env_projectedtexture tied to the player
        m_pFlashlight = CreateEntityByName("env_projectedtexture");
        if (m_pFlashlight)
        {
            m_pFlashlight->KeyValue("texturename", "effects/flashlight001"); // Default flashlight texture
            m_pFlashlight->KeyValue("nearz", "4.0");    // Near plane distance
            m_pFlashlight->KeyValue("farz", "750.0");   // Far plane distance
            m_pFlashlight->KeyValue("lightfov", "60.0"); // Field of view
            m_pFlashlight->KeyValue("brightnessscale", "2.0"); // Brightness
            m_pFlashlight->KeyValue("enableshadows", "1"); // Enable shadows

            // Parent it to the player
            m_pFlashlight->SetParent(this);
            m_pFlashlight->SetLocalOrigin(Vector(0, 0, 40)); // Offset above player's head
            m_pFlashlight->SetLocalAngles(QAngle(0, 0, 0)); // Align with player initially
            DispatchSpawn(m_pFlashlight);
        }
        m_bFlashlightOn = true;
    }
}

// Bind the flashlight toggle to a key (e.g., 'F')
CON_COMMAND(ToggleFlashlight, "Toggles the flashlight")
{
    CSDKPlayer* pPlayer = ToSDKPlayer(UTIL_GetCommandClient());
    if (pPlayer)
    {
        pPlayer->ToggleFlashlight();
    }
}
