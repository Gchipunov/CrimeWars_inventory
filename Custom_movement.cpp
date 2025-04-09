// CustomMovement.h
#pragma once
#include "cbase.h"
#include "player.h"

class CCustomMovement : public CBasePlayer
{
public:
    DECLARE_CLASS(CCustomMovement, CBasePlayer);
    DECLARE_SERVERCLASS();
    DECLARE_DATADESC();

    CCustomMovement();
    ~CCustomMovement();

    virtual void Precache();
    virtual void Spawn();
    virtual void PreThink();
    virtual void PostThink();
    virtual void HandleClientInput();

    // Movement functions
    void HandleSprinting();
    void HandleCrouchSliding();
    void HandleJumping();

private:
    // Movement variables
    float m_flSprintSpeed;
    float m_flWalkSpeed;
    float m_flSlideSpeed;
    float m_flStamina;
    float m_flMaxStamina;
    bool m_bIsSprinting;
    bool m_bIsSliding;
    float m_flSlideTime;
    float m_flLastJumpTime;

    // Networked variables
    CNetworkVar(bool, m_bSprinting);
    CNetworkVar(bool, m_bSliding);
    CNetworkVar(float, m_flCurrentStamina);
};

// CustomMovement.cpp
#include "CustomMovement.h"
#include "in_buttons.h"

LINK_ENTITY_TO_CLASS(player, CCustomMovement);

BEGIN_DATADESC(CCustomMovement)
    DEFINE_FIELD(m_flSprintSpeed, FIELD_FLOAT),
    DEFINE_FIELD(m_flWalkSpeed, FIELD_FLOAT),
    DEFINE_FIELD(m_flSlideSpeed, FIELD_FLOAT),
    DEFINE_FIELD(m_flStamina, FIELD_FLOAT),
    DEFINE_FIELD(m_flMaxStamina, FIELD_FLOAT),
    DEFINE_FIELD(m_bIsSprinting, FIELD_BOOLEAN),
    DEFINE_FIELD(m_bIsSliding, FIELD_BOOLEAN),
    DEFINE_FIELD(m_flSlideTime, FIELD_FLOAT),
    DEFINE_FIELD(m_flLastJumpTime, FIELD_FLOAT),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CCustomMovement, DT_CustomMovement)
    SendPropBool(SENDINFO(m_bSprinting)),
    SendPropBool(SENDINFO(m_bSliding)),
    SendPropFloat(SENDINFO(m_flCurrentStamina)),
END_SEND_TABLE()

CCustomMovement::CCustomMovement()
{
    m_flSprintSpeed = 320.0f;
    m_flWalkSpeed = 190.0f;
    m_flSlideSpeed = 400.0f;
    m_flMaxStamina = 100.0f;
    m_flStamina = m_flMaxStamina;
    m_bIsSprinting = false;
    m_bIsSliding = false;
    m_flSlideTime = 0.0f;
    m_flLastJumpTime = 0.0f;
}

CCustomMovement::~CCustomMovement()
{
}

void CCustomMovement::Precache()
{
    BaseClass::Precache();
}

void CCustomMovement::Spawn()
{
    BaseClass::Spawn();
    SetModel("models/player.mdl");
    SetMoveType(MOVETYPE_WALK);
    SetSolid(SOLID_BBOX);
}

void CCustomMovement::PreThink()
{
    BaseClass::PreThink();
    HandleClientInput();
    HandleSprinting();
    HandleCrouchSliding();
    HandleJumping();
}

void CCustomMovement::PostThink()
{
    BaseClass::PostThink();
}

void CCustomMovement::HandleClientInput()
{
    // Get player input
    int buttons = m_nButtons;
    m_bIsSprinting = (buttons & IN_SPEED) && (m_flStamina > 0) && !m_bIsSliding;
}

void CCustomMovement::HandleSprinting()
{
    float flMaxSpeed = m_flWalkSpeed;
    
    if (m_bIsSprinting && IsOnGround())
    {
        flMaxSpeed = m_flSprintSpeed;
        m_flStamina -= gpGlobals->frametime * 20.0f;
        
        if (m_flStamina < 0)
            m_flStamina = 0;
    }
    else if (!m_bIsSprinting && m_flStamina < m_flMaxStamina)
    {
        m_flStamina += gpGlobals->frametime * 15.0f;
        if (m_flStamina > m_flMaxStamina)
            m_flStamina = m_flMaxStamina;
    }

    m_flCurrentStamina = m_flStamina;
    SetMaxSpeed(flMaxSpeed);
}

void CCustomMovement::HandleCrouchSliding()
{
    if (m_nButtons & IN_DUCK && m_bIsSprinting && IsOnGround() && !m_bIsSliding)
    {
        m_bIsSliding = true;
        m_flSlideTime = gpGlobals->curtime + 1.0f; // 1 second slide duration
    }

    if (m_bIsSliding)
    {
        SetMaxSpeed(m_flSlideSpeed);
        
        Vector vel = GetAbsVelocity();
        vel.z = 0;
        float speed = vel.Length();
        
        if (speed > m_flWalkSpeed)
        {
            float friction = 2.0f * gpGlobals->frametime;
            SetMaxSpeed(speed - (speed * friction));
        }
        
        if (gpGlobals->curtime > m_flSlideTime || !IsOnGround())
        {
            m_bIsSliding = false;
        }
    }
}

void CCustomMovement::HandleJumping()
{
    if (m_nButtons & IN_JUMP && IsOnGround() && gpGlobals->curtime - m_flLastJumpTime > 0.3f)
    {
        Vector velocity = GetAbsVelocity();
        velocity.z = 268.3281572999747f; // Default Source jump height
        SetAbsVelocity(velocity);
        m_flLastJumpTime = gpGlobals->curtime;
    }
}
