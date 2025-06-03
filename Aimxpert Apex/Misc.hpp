
#include <map>
typedef void* PVOID;

bool IsKeyDown(int vk)
{
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

struct Misc {

    std::set<int> RapidFireList = {};
    float HangOnWall1;
    float HangOnWall2;
    float TraversalProgress1;
    float HangOnWall3;
    float HangOnWall4;
    float StartJumpTime1;

    Level* Map;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;

    Misc(Level* Map, LocalPlayer* Myself, std::vector<Player*>* GamePlayers) {
        this->Map = Map;
        this->Myself = Myself;
        this->Players = GamePlayers;
    }

    void Update() {
        if (!Map->IsPlayable)
            return;
        if (Myself->IsDead)
            return;

        if (Settings::Misc::SkinChanger) {
            ULONG64 wephandle = aimxpert::read<ULONG64>(Myself->BasePointer + OFF_WEAPON_HANDLE);
            wephandle &= 0xffff;
            ULONG64 wep_entity = Myself->WeaponEntity;
            std::map<int, int> weaponSkinMap;
            weaponSkinMap[WeaponIDs::WEAPON_P2020] = Settings::Misc::SkinP2020;
            weaponSkinMap[WeaponIDs::WEAPON_RE45] = Settings::Misc::SkinRE45;
            weaponSkinMap[WeaponIDs::WEAPON_ALTERNATOR] = Settings::Misc::SkinALTERNATOR;
            weaponSkinMap[WeaponIDs::WEAPON_R99] = Settings::Misc::SkinR99;
            weaponSkinMap[WeaponIDs::WEAPON_R301] = Settings::Misc::SkinR301;
            weaponSkinMap[WeaponIDs::WEAPON_SPITFIRE] = Settings::Misc::SkinSPITFIRE;
            weaponSkinMap[WeaponIDs::WEAPON_G7] = Settings::Misc::SkinG7;
            weaponSkinMap[WeaponIDs::WEAPON_CAR] = Settings::Misc::SkinCAR;
            weaponSkinMap[WeaponIDs::WEAPON_RAMPAGE] = Settings::Misc::SkinRAMPAGE;
            weaponSkinMap[WeaponIDs::WEAPON_3030] = Settings::Misc::SkinREPEATER;
            weaponSkinMap[WeaponIDs::WEAPON_HEMLOCK] = Settings::Misc::SkinHEMLOCK;
            weaponSkinMap[WeaponIDs::WEAPON_FLATLINE] = Settings::Misc::SkinFLATLINE;
            weaponSkinMap[WeaponIDs::WEAPON_NEMESIS] = Settings::Misc::SkinNEMESIS;
            weaponSkinMap[WeaponIDs::WEAPON_VOLT] = Settings::Misc::SkinVOLT;
            weaponSkinMap[WeaponIDs::WEAPON_TRIPLE_TAKE] = Settings::Misc::SkinTRIPLETAKE;
            weaponSkinMap[WeaponIDs::WEAPON_LSTAR] = Settings::Misc::SkinLSTAR;
            weaponSkinMap[WeaponIDs::WEAPON_DEVOTION] = Settings::Misc::SkinDEVOTION;
            weaponSkinMap[WeaponIDs::WEAPON_HAVOC] = Settings::Misc::SkinHAVOC;
            weaponSkinMap[WeaponIDs::WEAPON_SENTINEL] = Settings::Misc::SkinSENTINEL;
            weaponSkinMap[WeaponIDs::WEAPON_CHARGE_RIFLE] = Settings::Misc::SkinCHARGE_RIFLE;
            weaponSkinMap[WeaponIDs::WEAPON_LONGBOW] = Settings::Misc::SkinLONGBOW;
            weaponSkinMap[WeaponIDs::WEAPON_MOZAMBIQUE] = Settings::Misc::SkinMOZAMBIQUE;
            weaponSkinMap[WeaponIDs::WEAPON_EVA8] = Settings::Misc::SkinEVA8;
            weaponSkinMap[WeaponIDs::WEAPON_PEACEKEEPER] = Settings::Misc::SkinPEACEKEEPER;
            weaponSkinMap[WeaponIDs::WEAPON_MASTIFF] = Settings::Misc::SkinMASTIFF;
            weaponSkinMap[WeaponIDs::WEAPON_WINGMAN] = Settings::Misc::SkinWINGMAN;
            weaponSkinMap[WeaponIDs::WEAPON_PROWLER] = Settings::Misc::SkinPROWLER;
            weaponSkinMap[WeaponIDs::WEAPON_BOCEK] = Settings::Misc::SkinBOCEK;
            weaponSkinMap[WeaponIDs::WEAPON_KRABER] = Settings::Misc::SkinKRABER;

            int waponIndex = aimxpert::read<int>(wep_entity + OFF_WEAPON_INDEX);
            if (weaponSkinMap.count(waponIndex) == 0)
                return;
            int skinID = weaponSkinMap[waponIndex];
            aimxpert::write<int>(wep_entity + OFF_SKIN, skinID);
        }
    }

    void SuperGlide() {
        UpdateSuperGlide();
        if (Settings::Misc::SuperGlideMode == 0) { 
            if (InputManager::isKeyDownOrPress(InputKeyType::KEYBOARD_SPACE)) {
                static float startjumpTime = 0;
                static bool startSg = false;
                static float traversalProgressTmp = 0.0;

                float worldtime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE); 
                float traversalStartTime = aimxpert::read<float>(Myself->BasePointer + OFF_TRAVERSAL_START_TIME); 
                float traversalProgress = aimxpert::read<float>(Myself->BasePointer + OFF_TRAVERSAL_PROGRESS);
                float HangOnWall = -(traversalStartTime - worldtime);

                if (HangOnWall > HangOnWall1 && HangOnWall < HangOnWall2) {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                }
                if (traversalProgress > TraversalProgress1 && !startSg && HangOnWall > HangOnWall3 && HangOnWall < HangOnWall4) {
                    startjumpTime = worldtime;
                    startSg = true;
                }
                if (startSg) {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                    while (aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE) - startjumpTime < StartJumpTime1);
                    {
                        aimxpert::write<int>(OFF_REGION + OFF_IN_DUCK + 0x8, 6);
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                        std::this_thread::sleep_for(std::chrono::milliseconds(600));
                    }
                    startSg = false;
                }
            }
        }

        if (Settings::Misc::SuperGlideMode == 1) { // Automatic, Controller Support
            static float startjumpTime = 0;
            static bool startSg = false;
            static float traversalProgressTmp = 0.0;

            float worldtime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE); // Current time
            float traversalStartTime = aimxpert::read<float>(Myself->BasePointer + OFF_TRAVERSAL_START_TIME); // Time to start wall climbing
            float traversalProgress = aimxpert::read<float>(Myself->BasePointer + OFF_TRAVERSAL_PROGRESS); // Wall climbing, if > 0.87 it is almost
            float HangOnWall = -(traversalStartTime - worldtime);
            if (HangOnWall > HangOnWall1 && HangOnWall < HangOnWall2) {
                aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
            }
            if (traversalProgress > TraversalProgress1 && !startSg && HangOnWall > HangOnWall3 && HangOnWall < HangOnWall4) {
                startjumpTime = worldtime;
                startSg = true;
            }
            if (startSg) {
                aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                while (aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE) - startjumpTime < StartJumpTime1); 
                {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_DUCK + 0x8, 6);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                    std::this_thread::sleep_for(std::chrono::milliseconds(600));
                }
                startSg = false;
            }
        }
    }

    void UpdateSuperGlide() {
        if (Settings::Misc::SuperGlideFPS == 0) 
        {
            HangOnWall1 = 0.1;
            HangOnWall2 = 0.12;
            TraversalProgress1 = 0.87f;
            HangOnWall3 = 0.1f;
            HangOnWall4 = 1.5f;
            StartJumpTime1 = 0.011;
        }

        else if (Settings::Misc::SuperGlideFPS == 1) // 144 FPS
        {
            HangOnWall1 = 0.05;
            HangOnWall2 = 0.07;
            TraversalProgress1 = 0.90f;
            HangOnWall3 = 0.05f;
            HangOnWall4 = 0.75f;
            StartJumpTime1 = 0.007;
        }

        else if (Settings::Misc::SuperGlideFPS == 1) // 240 FPS
        {
            HangOnWall1 = 0.033;
            HangOnWall2 = 0.04;
            TraversalProgress1 = 0.95f;
            HangOnWall3 = 0.033f;
            HangOnWall4 = 0.2f;
            StartJumpTime1 = 0.004;
        }
    }

    void BHop() {
        int startCtrl = 0;
        if (InputManager::isKeyDownOrPress(Settings::Misc::BHopBind) && InputManager::isKeyDownOrPress(InputKeyType::KEYBOARD_SPACE)) {
            startCtrl = 1;
            int result = aimxpert::read<int>(Myself->BasePointer + OFF_CENTITY_FLAGS);
            if ((result & 0x1) != 0)
            {
                aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                std::this_thread::sleep_for(std::chrono::milliseconds(Settings::Misc::BHopDelay));
                aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
            }
            else
            {
                aimxpert::write<int>(OFF_REGION + OFF_IN_FORWARD + 0x8, 5);
                aimxpert::write<int>(OFF_REGION + OFF_IN_FORWARD + 0x8, 4);
            }
        }
    }

    void WallJump() {
        if (InputManager::isKeyDownOrPress(Settings::Misc::WallJumpBind)) {

            float wallStartTime = aimxpert::read<float>(Myself->BasePointer + OFF_WALL_RUN_START_TIME);
            float wallProgress = aimxpert::read<float>(Myself->BasePointer + OFF_TRAVERSAL_PROGRESS);

            static float onWallTmp = 0;
            if (wallStartTime > onWallTmp + 0.1)
            {
                int forward = aimxpert::read<int>(OFF_REGION + OFF_IN_FORWARD);
                if (forward == 0)
                {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                }
            }
            onWallTmp = wallStartTime;

            static float onEdgeTmp = 0;
            if (wallProgress > onEdgeTmp + 0.1)
            {
                int forward = aimxpert::read<int>(OFF_REGION + OFF_IN_FORWARD);
                if (forward == 0)
                {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    aimxpert::write<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                }
            }
            onEdgeTmp = wallProgress;
        }
    }

    //Credits to chettoy/apexsky
    void AutoTapStrafe() {
        bool ts_start = true;
        bool longclimb = false;
        // AutoTapStrafe
        float wallrun_start = aimxpert::read<float>(Myself->BasePointer + OFF_WALL_RUN_START_TIME);
        float wallrun_clear = aimxpert::read<float>(Myself->BasePointer + OFF_WALL_RUN_CLEAR_TIME);
        float world_time = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE);

        if (wallrun_start > wallrun_clear) {
            float climbTime = world_time - wallrun_start;
            if (climbTime > 0.8) {
                longclimb = true;
                ts_start = false;
            }
            else {
                ts_start = true;
            }
        }
        if (ts_start) {
            if (longclimb) {
                if (world_time > wallrun_clear + 0.1)
                    longclimb = false;
            }

            int flags = aimxpert::read<int>(Myself->BasePointer + OFF_CENTITY_FLAGS);
            int backward_state = aimxpert::read<int>(OFF_REGION + OFF_IN_BACKWARD);
            int forward_state = aimxpert::read<int>(OFF_REGION + OFF_IN_FORWARD);
            int force_forward = aimxpert::read<int>(OFF_REGION + OFF_IN_FORWARD + 0x8);
            int skydrive_state = aimxpert::read<int>(Myself->BasePointer + OFF_SKY_DIVE_STATUS);
            int duck_state = aimxpert::read<int>(Myself->BasePointer + OFF_DUCK_STATUS);

            if (((flags & 0x1) == 0) && !(skydrive_state > 0) && !longclimb &&
                !(backward_state > 0)) {
                if (((duck_state > 0) && (forward_state == 33))) { // Previously 33
                    if (force_forward == 0) {
                        aimxpert::write<int>(OFF_REGION + OFF_IN_FORWARD + 0x8, 1);
                    }
                    else {
                        aimxpert::write<int>(OFF_REGION + OFF_IN_FORWARD + 0x8, 0);
                    }
                }
            }
            else if ((flags & 0x1) != 0) {
                if (forward_state == 0) {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_FORWARD + 0x8, 0);
                }
                else if (forward_state == 33) {
                    aimxpert::write<int>(OFF_REGION + OFF_IN_FORWARD + 0x8, 1);
                }
            }
        }
    }
};