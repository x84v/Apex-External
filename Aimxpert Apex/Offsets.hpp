

constexpr ULONG64 OFF_LEVEL = 0x1b729a4;
constexpr ULONG64 OFF_LOCAL_PLAYER = 0x2684A38;
constexpr ULONG64 OFF_ENTITY_LIST = 0x6a44a68;
constexpr ULONG64 OFF_NAME_INDEX = 0x38;
constexpr ULONG64 OFF_NAME_LIST = 0x949eaa0;
constexpr ULONG64 OFF_GAMEMODE = 0x026bcc20; //mp_gamemode
constexpr ULONG64 OFF_SQUAD_ID = 0x0344; // [RecvTable.DT_BaseEntity]->m_squadID

// HUD
constexpr ULONG64 OFF_VIEWRENDER = 0x3355258;
constexpr ULONG64 OFF_VIEWMATRIX = 0x11a350;

// Buttons
constexpr ULONG64 OFF_INATTACK = 0x03357b28;
constexpr ULONG64 OFF_IN_JUMP = 0x03357c20;
constexpr ULONG64 OFF_IN_DUCK = 0x03357d18;
constexpr ULONG64 OFF_IN_FORWARD = 0x03357d58;
constexpr ULONG64 OFF_IN_BACKWARD = 0x03357d80;
constexpr ULONG64 OFFSET_in_use = 0x03357c90;
constexpr ULONG64 OFF_TRAVERSAL_START_TIME = 0x2bc8; // [RecvTable.DT_LocalPlayerExclusive]->m_traversalStartTime
constexpr ULONG64 OFF_TRAVERSAL_PROGRESS = 0x2bc4; // [RecvTable.DT_LocalPlayerExclusive]->m_traversalProgress
constexpr ULONG64 OFF_WALL_RUN_START_TIME = 0x36fc; // [RecvTable.DT_LocalPlayerExclusive]->m_wallRunStartTime
constexpr ULONG64 OFF_WALL_RUN_CLEAR_TIME = 0x3700; // [RecvTable.DT_LocalPlayerExclusive]->m_wallRunClearTime

// Player
constexpr ULONG64 OFF_HEALTH = 0x0328;
constexpr ULONG64 OFF_MAXHEALTH = 0x0470;
constexpr ULONG64 OFF_SHIELD = 0x01a0;
constexpr ULONG64 OFF_MAXSHIELD = 0x01a4;
constexpr ULONG64 OFF_CAMERAORIGIN = 0x1F40;
constexpr ULONG64 OFF_STUDIOHDR = 0xfd0;
constexpr ULONG64 OFF_BONES = 0x0d88 + 0x48; // [RecvTable.DT_BaseAnimating]->m_nForceBone
constexpr ULONG64 OFF_LOCAL_ORIGIN = 0x017c;
constexpr ULONG64 OFF_ABSVELOCITY = 0x0170;
constexpr ULONG64 OFF_ZOOMING = 0x1c41; // [RecvTable.DT_Player]->m_bZooming
constexpr ULONG64 OFF_TEAM_NUMBER = 0x0338;
constexpr ULONG64 OFF_NAME = 0x0481;
constexpr ULONG64 OFF_LIFE_STATE = 0x0690; // [RecvTable.DT_Player]->m_lifeState
constexpr ULONG64 OFF_BLEEDOUT_STATE = 0x27b8; // [RecvTable.DT_Player]->m_bleedoutState
constexpr ULONG64 OFF_LAST_VISIBLE_TIME = 0x19f0;
constexpr ULONG64 OFF_LAST_AIMEDAT_TIME = 0x19f8; //CWeaponX!lastCrosshairTargetTime
constexpr ULONG64 OFF_VIEW_ANGLES = 0x2594 - 0x14; // [DataMap.C_Player]->m_ammoPoolCapacity - 0x14
constexpr ULONG64 OFF_PUNCH_ANGLES = 0x2498;  // [DataMap.C_Player]->m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle ( dont have )
constexpr ULONG64 OFF_YAW = 0x2294 - 0x8; // [DataMap.C_Player]->m_currentFramePlayer.m_ammoPoolCount - 0x8
constexpr ULONG64 OFF_MODELNAME = 0x0030;
constexpr ULONG64 OFF_OBSERVER_LIST = 0x6a46a88;
constexpr ULONG64 OFF_OBSERVER_LIST_IN_ARRAY = 0x954; // [RecvTable.DT_GlobalNonRewinding].m_gameTimescale
constexpr ULONG64 OFF_SKY_DIVE_STATUS = 0x4844; // [DataMap.C_Player]->m_skydiveState
constexpr ULONG64 OFF_DUCK_STATUS = 0x2ab8; // [DataMap.C_Player]->m_duckState
constexpr ULONG64 OFF_CENTITY_FLAGS = 0x00c8;
constexpr ULONG64 OFF_GRAPPLE = 0x2d08; // [RecvTable.DT_Player].m_grapple
constexpr ULONG64 OFF_GRAPPLE_ATTACHED = 0x2d90;

// Weapon
constexpr ULONG64 OFF_WEAPON_HANDLE = 0x1994; // [RecvTable.DT_Player]->m_latestPrimaryWeapons
constexpr ULONG64 OFF_WEAPON_INDEX = 0x17e8; // [RecvTable.DT_WeaponX]->m_weaponNameIndex
constexpr ULONG64 OFF_PROJECTILESCALE = 0x2788; // [WeaponSettings]->projectile_gravity_scale + 0x8
constexpr ULONG64 OFF_PROJECTILESPEED = 0x2780; // [WeaponSettings]->projectile_launch_speed
constexpr ULONG64 OFF_OFFHAND_WEAPON = 0x19a4; // [RecvTable.DT_BaseCombatCharacter]->m_latestNonOffhandWeapons
constexpr ULONG64 OFF_CURRENTZOOMFOV = 0x16ec; // [RecvTable.DT_WeaponPlayerData]->m_curZoomFOV
constexpr ULONG64 OFF_TARGETZOOMFOV = 0x1630 + 0x00bc; // [RecvTable.DT_WeaponX]->m_playerData + [RecvTable.DT_WeaponPlayerData]->m_targetZoomFOV
constexpr ULONG64 OFF_SKIN = 0x0d40; // [DataMap.C_BaseAnimating]->m_nSkin
constexpr ULONG64 OFF_TIME_BASE = 0x20e8; // [DataMap.C_Player]->m_currentFramePlayer.timeBase
constexpr ULONG64 OFF_WEAPON_DISCARDED = 0x15f9; // [RecvTable.DT_WeaponX]->m_discarded
constexpr ULONG64 OFF_VIEWMODELS = 0x2de0; // [RecvTable.DT_Player]->m_hViewModels
constexpr ULONG64 OFF_WEAPON_AMMO = 0x15b0; //RecvTable.DT_WeaponX_LocalWeaponData -> m_ammoInClip
constexpr ULONG64 OFF_m_bInReload = 0x15fa; //m_bInReload
constexpr ULONG64 OFFSET_GRAPPLE = 0x2d08; //[RecvTable.DT_Player] m_grapple
constexpr ULONG64 OFFSET_GRAPPLE_ATTACHED = 0x0048; //m_grappleAttached

// Glow
constexpr ULONG64 OFF_HIGHLIGHT_TYPE_SIZE = 0x34;
constexpr ULONG64 OFF_GLOW_THROUGH_WALL = 0x26c;
constexpr ULONG64 OFF_GLOW_FIX = 0x278;
constexpr ULONG64 OFF_GLOW_HIGHLIGHT_ID = 0x29C;
constexpr ULONG64 OFF_GLOW_HIGHLIGHTS = 0x718DFD0;
constexpr ULONG64 OFF_GLOW_ENABLE = 0x28C;

// Item
constexpr ULONG64 OFF_CUSTOM_SCRIPT_INT = 0x15b4; // [RecvTable.DT_PropSurvival]->m_customScriptInt
