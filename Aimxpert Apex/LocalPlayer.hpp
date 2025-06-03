

struct LocalPlayer {
    ULONG64 BasePointer;

    bool IsDead;
    bool IsInAttack;
    bool IsKnocked;
    bool IsZooming;
    bool InJump;

    int Team;
    int Squad;
    Vector3D LocalOrigin;
    Vector3D CameraPosition;

    Vector2D ViewAngles;
    Vector2D PunchAngles;
    Vector2D PunchAnglesPrevious;
    Vector2D PunchAnglesDifferent;
    Vector3D absoluteVelocity;

    int WeaponIndex;
    float WeaponProjectileSpeed;
    float WeaponProjectileScale;
    bool IsHoldingGrenade;
    ULONG64 WeaponEntity;
    bool weaponDiscarded;

    float ZoomFOV;
    float TargetZoomFOV;

    float ViewYaw;

    Vector3D localOrigin;
    Vector3D cameraPosition;
    Vector2D viewAngles;
    Vector2D punchAngles;
    Vector2D punchAnglesPrev;
    Vector2D punchAnglesDiff;

    bool IsReloading;
    int Ammo;

    void ReadGameData() {

        BasePointer = aimxpert::read<ULONG64>(OFF_REGION + OFF_LOCAL_PLAYER);
        if (BasePointer == 0) {
            return;
        }

        IsDead = aimxpert::read<short>(BasePointer + OFF_LIFE_STATE) > 0;
        IsKnocked = aimxpert::read<short>(BasePointer + OFF_BLEEDOUT_STATE) > 0;
        IsZooming = aimxpert::read<short>(BasePointer + OFF_ZOOMING) > 0;
        IsInAttack = aimxpert::read<short>(OFF_REGION + OFF_INATTACK) > 0;
        InJump = aimxpert::read<bool>(OFF_REGION + OFF_IN_JUMP) > 0;
        Team = aimxpert::read<int>(BasePointer + OFF_TEAM_NUMBER);
        Squad = aimxpert::read<int>(BasePointer + OFF_SQUAD_ID);

        localOrigin = aimxpert::read<Vector3D>(BasePointer + OFF_LOCAL_ORIGIN);
        LocalOrigin = aimxpert::read<Vector3D>(BasePointer + OFF_LOCAL_ORIGIN);
        absoluteVelocity = aimxpert::read<Vector3D>(BasePointer + OFF_ABSVELOCITY);
        CameraPosition = aimxpert::read<Vector3D>(BasePointer + OFF_CAMERAORIGIN);
        ViewAngles = aimxpert::read<Vector2D>(BasePointer + OFF_VIEW_ANGLES);
        PunchAngles = aimxpert::read<Vector2D>(BasePointer + OFF_PUNCH_ANGLES);
        PunchAnglesDifferent = PunchAnglesPrevious.Subtract(PunchAngles);
        PunchAnglesPrevious = PunchAngles;
        ViewYaw = aimxpert::read<float>(BasePointer + OFF_YAW);
        IsReloading = WeaponEntity + OFF_m_bInReload;
        Ammo = WeaponEntity + OFF_WEAPON_AMMO;

        cameraPosition = aimxpert::read<Vector3D>(BasePointer + OFF_CAMERAORIGIN);
        viewAngles = aimxpert::read<Vector2D>(BasePointer + OFF_VIEW_ANGLES);
        punchAngles = aimxpert::read<Vector2D>(BasePointer + OFF_PUNCH_ANGLES);
        punchAnglesDiff = punchAnglesPrev.Subtract(punchAngles);
        punchAnglesPrev = punchAngles;

        if (!IsDead && !IsKnocked) {
            ULONG64 WeaponHandle = aimxpert::read<ULONG64>(BasePointer + OFF_WEAPON_HANDLE);
            ULONG64 WeaponHandleMasked = WeaponHandle & 0xffff;
            WeaponEntity = aimxpert::read<ULONG64>(OFF_REGION + OFF_ENTITY_LIST + (WeaponHandleMasked << 5));

            int OffHandWeaponID = aimxpert::read<int>(BasePointer + OFF_OFFHAND_WEAPON);
            IsHoldingGrenade = OffHandWeaponID == -251 ? true : false;

            ZoomFOV = aimxpert::read<float>(WeaponEntity + OFF_CURRENTZOOMFOV);
            TargetZoomFOV = aimxpert::read<float>(WeaponEntity + OFF_TARGETZOOMFOV);

            WeaponIndex = aimxpert::read<int>(WeaponEntity + OFF_WEAPON_INDEX);
            weaponDiscarded = aimxpert::read<int>(WeaponEntity + OFF_WEAPON_DISCARDED) == 1;
            WeaponProjectileSpeed = aimxpert::read<float>(WeaponEntity + OFF_PROJECTILESPEED);
            WeaponProjectileScale = aimxpert::read<float>(WeaponEntity + OFF_PROJECTILESCALE);
        }
    }


    std::string GetPlayerModelName() {
        uintptr_t ModelOffset = aimxpert::read<uintptr_t>(BasePointer + OFF_MODELNAME);
        std::string ModelName = aimxpert::readString(ModelOffset, 1024);

        static std::unordered_map<std::string, std::string> ModelNameMap = { {"dummie", "Dummie"}, {"ash", "Ash"}, {"ballistic", "Ballistic"}, {"bangalore", "Bangalore"}, {"bloodhound", "Bloodhound"}, {"catalyst", "Catalyst"}, {"caustic", "Caustic"}, {"conduit", "Conduit"}, {"crypto", "Crypto"}, {"fuse", "Fuse"}, {"gibraltar", "Gibraltar"}, {"horizon", "Horizon"}, {"nova", "Horizon"}, {"holo", "Mirage"}, {"mirage", "Mirage"}, {"lifeline", "Lifeline"}, {"loba", "Loba"}, {"madmaggie", "Mad Maggie"}, {"newcastle", "Newcastle"}, {"octane", "Octane"}, {"pathfinder", "Pathfinder"}, {"rampart", "Rampart"}, {"revenant", "Revenant"}, {"seer", "Seer"}, {"stim", "Octane"}, {"valkyrie", "Valkyrie"}, {"vantage", "Vantage"}, {"wattson", "Wattson"}, {"wraith", "Wraith"}, {"alter", "Alter"}, };

        std::string replacedName = ModelName;
        for (auto& entry : ModelNameMap) {
            if (ModelName.find(entry.first) != std::string::npos) {
                replacedName = entry.second;
                break;
            }
        }

        return replacedName;
    }

    bool IsValid() {
        return BasePointer != 0;
    }

    bool IsCombatReady() {
        if (BasePointer == 0)
            return false;
        if (IsDead)
            return false;
        if (IsKnocked)
            return false;
        return true;
    }
};