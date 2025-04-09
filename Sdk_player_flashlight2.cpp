void CSDKPlayer::PlayerRunCommand(CUserCmd* ucmd, IMoveHelper* moveHelper)
{
    BaseClass::PlayerRunCommand(ucmd, moveHelper);

    if (m_bFlashlightOn && m_pFlashlight)
    {
        // Update flashlight to match player's view angles
        QAngle eyeAngles;
        Vector eyeForward;
        EyeVectors(&eyeForward, NULL, NULL); // Get forward vector from player's eyes
        m_pFlashlight->SetLocalOrigin(EyePosition() + eyeForward * 10); // Position slightly ahead
        m_pFlashlight->SetAbsAngles(GetAbsAngles()); // Match player's yaw
    }
}
