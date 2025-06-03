
struct Player {
    LocalPlayer* Myself;

    int Index;
    ULONG64 BasePointer;

    std::string Name;
    int Team;

    bool IsDead;
    bool IsKnocked;

    Vector2D ViewAngles;
    float ViewYaw;
    Vector3D LocalOrigin;
    Vector3D AbsoluteVelocity;

    int Health;
    int MaxHealth;
    int Shield;
    int MaxShield;

    int LastTimeAimedAt;
    int LastTimeAimedAtPrevious;
    bool IsAimedAt;
    float m_lastVisibleTime;
    int LastVisibleTime;
    int LastTimeVisiblePrevious;
    bool IsVisible;

    bool IsLocal;
    bool IsAlly;
    bool IsHostile;
    bool nonBR;
    bool friendly;

    int WeaponIndex;
    bool IsHoldingGrenade;
    ULONG64 WeaponEntity;

    float DistanceToLocalPlayer;
    float Distance2DToLocalPlayer;

    bool IsLockedOn;

    int ducking;
    Vector3D localOrigin_prev;
    Vector3D localOrigin;
    Vector3D absoluteVelocity;
    Vector3D localOrigin_predicted;
    Vector2D aimbotDesiredAngles;
    Vector2D aimbotDesiredAnglesIncrement;
    Vector2D aimbotDesiredAnglesSmoothed;
    float aimbotScore;
    Vector2D aimbotDesiredAnglesSmoothedNoRecoil;
    std::chrono::milliseconds LastRead;

    Player(int PlayerIndex, LocalPlayer* Me) {
        this->Index = PlayerIndex;
        this->Myself = Me;
    }

    void Reset() {
        Index = 0;
        BasePointer = 0;

        std::string Name = "";
        Team = 0;

        IsDead = 0;
        IsKnocked = 0;

        ViewAngles = {};
        ViewYaw = 0;
        LocalOrigin = {};
        AbsoluteVelocity = {};

        Health = 0;
        MaxHealth = 0;
        Shield = 0;
        MaxShield = 0;

        LastTimeAimedAt = 0;
        LastTimeAimedAtPrevious = 0;
        IsAimedAt = 0;

        LastVisibleTime = 0;
        LastTimeVisiblePrevious = 0;
        IsVisible = 0;

        IsLocal = 0;
        IsAlly = 0;
        IsHostile = 0;
        nonBR = 0;
        friendly = 0;

        WeaponIndex = 0;
        IsHoldingGrenade = 0;
        WeaponEntity = 0;

        DistanceToLocalPlayer = 0;
        Distance2DToLocalPlayer = 0;

        IsLockedOn = 0;

        ducking = 0;
        localOrigin_prev = {};
        localOrigin = {};
        absoluteVelocity = {};
        localOrigin_predicted = {};
        aimbotDesiredAngles = {};
        aimbotDesiredAnglesIncrement = {};
        aimbotDesiredAnglesSmoothed = {};
        aimbotScore = 0;
        aimbotDesiredAnglesSmoothedNoRecoil = {};
    }

    void Read() {
        BasePointer = aimxpert::read<ULONG64>(OFF_REGION + OFF_ENTITY_LIST + ((Index + 1) << 5));
        if (BasePointer == 0) return;

        Name = aimxpert::readString(BasePointer + OFF_NAME, 1024);
        Team = aimxpert::read<int>(BasePointer + OFF_TEAM_NUMBER);

        if (!IsPlayer() && !IsDummy()) {
            BasePointer = 0;
            return;
        }

        if (!IsDummy()) {
            IsDead = aimxpert::read<short>(BasePointer + OFF_LIFE_STATE) > 0;
            IsKnocked = aimxpert::read<short>(BasePointer + OFF_BLEEDOUT_STATE) > 0;
        }
        else {
            IsDead = false;
            IsKnocked = false;
        }

        LocalOrigin = aimxpert::read<Vector3D>(BasePointer + OFF_LOCAL_ORIGIN);
        AbsoluteVelocity = aimxpert::read<Vector3D>(BasePointer + OFF_ABSVELOCITY);

        LastTimeAimedAt = aimxpert::read<int>(BasePointer + OFF_LAST_AIMEDAT_TIME);
        IsAimedAt = LastTimeAimedAtPrevious < LastTimeAimedAt;
        LastTimeAimedAtPrevious = LastTimeAimedAt;

        float WorldTime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE);
        float LastVisibleTime = aimxpert::read<float>(BasePointer + OFF_LAST_VISIBLE_TIME);
        IsVisible = (LastVisibleTime + 0.2) >= WorldTime || IsAimedAt;

        Health = aimxpert::read<int>(BasePointer + OFF_HEALTH);
        MaxHealth = aimxpert::read<int>(BasePointer + OFF_MAXHEALTH);
        Shield = aimxpert::read<int>(BasePointer + OFF_SHIELD);
        MaxShield = aimxpert::read<int>(BasePointer + OFF_MAXSHIELD);

        if (!IsDead && !IsKnocked && IsHostile) {
            ULONG64 WeaponHandle = aimxpert::read<ULONG64>(BasePointer + OFF_WEAPON_HANDLE);
            ULONG64 WeaponHandleMasked = WeaponHandle & 0xFFFF;
            WeaponEntity = aimxpert::read<ULONG64>(OFF_REGION + OFF_ENTITY_LIST + (WeaponHandleMasked << 5));

            int OffHandWeaponID = aimxpert::read<int>(BasePointer + OFF_OFFHAND_WEAPON);
            IsHoldingGrenade = (OffHandWeaponID == -251);

            WeaponIndex = aimxpert::read<int>(WeaponEntity + OFF_WEAPON_INDEX);
        }

        if (Myself->IsValid()) {
            IsLocal = (Myself->BasePointer == BasePointer);
            IsAlly = IsTeammate();
            IsHostile = !IsAlly;
            DistanceToLocalPlayer = Myself->LocalOrigin.Distance(LocalOrigin);
            Distance2DToLocalPlayer = Myself->LocalOrigin.To2D().Distance(LocalOrigin.To2D());
        }
    }

    float getLastVisibleTime()
    {
        uint64_t ptrLong = BasePointer + OFF_LAST_VISIBLE_TIME;
        float result = aimxpert::read<float>(ptrLong);
        return result;
    }

    bool isVisible()
    {
        const float lastVisibleTime = getLastVisibleTime();
        const bool isVisible = lastVisibleTime > m_lastVisibleTime;
        m_lastVisibleTime = lastVisibleTime;
        return isVisible;
    }

    std::string GetPlayerName() {
        if (IsDummy()) {
            return "Dummie";
        }
        else {
            uintptr_t NameIndex = aimxpert::read<uintptr_t>(BasePointer + OFF_NAME_INDEX);
            uintptr_t NameOffset = aimxpert::read<uintptr_t>(OFF_REGION + OFF_NAME_LIST + ((NameIndex - 1) * 24));
            std::string PlayerName = aimxpert::readString(NameOffset, 64);
            return PlayerName;
        }
    }

    bool IsSpectating() {
        if (!IsDead)
            return false;
        uint64_t SpectatorList = aimxpert::read<uint64_t>(OFF_REGION + OFF_OBSERVER_LIST);
        int PlayerData = aimxpert::read<int>(BasePointer + 0x38);
        int SpecIndex = aimxpert::read<int>(SpectatorList + PlayerData * 8 + OFF_OBSERVER_LIST_IN_ARRAY);
        uint64_t SpectatorAddr = aimxpert::read<uint64_t>(OFF_REGION + OFF_ENTITY_LIST + ((SpecIndex & 0xFFFF) << 5));
        if (SpectatorAddr == Myself->BasePointer)
            return true;
        return false;
    }

    float GetViewYaw() {
        if (!IsDummy() || IsPlayer()) {
            return aimxpert::read<float>(BasePointer + OFF_YAW);
        }
        return 0.0f;
    }

    bool IsValid() {
        return BasePointer != 0 && Health > 0 && !IsDead && (IsPlayer() || IsDummy());
    }

    bool IsCombatReady() {
        if (!IsValid())
            return false;
        if (IsDummy())
            return true;
        if (IsDead)
            return false;
        if (IsKnocked)
            return false;
        return true;
    }

    bool IsPlayer() {
        return Name == "player";
    }

    bool IsDummy() {
        return Team == 97;
    }

    bool IsTeammate() {
        if (LvMap::m_mixtape && Myself->Squad == -1) {
            return (Team & 1) == (Myself->Team & 1);
        }
        else {
            return Team == Myself->Team;
        }
    }

    // Bones //
    int GetBoneFromHitbox(HitboxType HitBox) const {
        ULONG64 ModelPointer = aimxpert::read<ULONG64>(BasePointer + OFF_STUDIOHDR);
        if (!aimxpert::IsValidPointer(ModelPointer))
            return -1;

        ULONG64 StudioHDR = aimxpert::read<ULONG64>(ModelPointer + 0x8);
        if (!aimxpert::IsValidPointer(StudioHDR + 0x34))
            return -1;

        auto HitboxCache = aimxpert::read<uint16_t>(StudioHDR + 0x34);
        auto HitboxArray = StudioHDR + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
        if (!aimxpert::IsValidPointer(HitboxArray + 0x4))
            return -1;

        auto IndexCache = aimxpert::read<uint16_t>(HitboxArray + 0x4);
        auto HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
        auto BonePointer = HitboxIndex + HitboxArray + (static_cast<int>(HitBox) * 0x20);
        if (!aimxpert::IsValidPointer(BonePointer))
            return -1;

        return aimxpert::read<uint16_t>(BonePointer);
    }

    Vector3D GetBonePosition(HitboxType HitBox) const {
        Vector3D Offset = Vector3D(0.0f, 0.0f, 0.0f);

        int Bone = GetBoneFromHitbox(HitBox);
        if (Bone < 0 || Bone > 255)
            return LocalOrigin.Add(Offset);

        ULONG64 BonePtr = aimxpert::read<ULONG64>(BasePointer + OFF_BONES);
        BonePtr += (Bone * sizeof(Matrix3x4));
        if (!aimxpert::IsValidPointer(BonePtr))
            return LocalOrigin.Add(Offset);

        Matrix3x4 BoneMatrix = aimxpert::read<Matrix3x4>(BonePtr);
        Vector3D BonePosition = BoneMatrix.GetPosition();

        if (!BonePosition.IsValid())
            return LocalOrigin.Add(Vector3D(0, 0, 0));

        BonePosition += LocalOrigin;
        return BonePosition;
    }

    float calcYawIncrement() {
        float wayA = aimbotDesiredAngles.y - Myself->ViewAngles.y;
        float wayB = 360 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }


    float calcPitchIncrement() {
        float wayA = aimbotDesiredAngles.x - Myself->ViewAngles.x;
        float wayB = 180 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    // For AimbotMode Grinder
    float calcDesiredPitch() {
        if (IsLocal)
            return 0;
        const Vector3D shift = Vector3D(100000, 100000, 100000);
        const Vector3D originA = Myself->LocalOrigin.Add(shift);
        const float extraZ = (ducking != -1) ? 10 : 0;
        const Vector3D originB = localOrigin_predicted.Add(shift).Subtract(Vector3D(0, 0, extraZ));
        const float deltaZ = originB.z - originA.z;
        const float pitchInRadians = std::atan2(-deltaZ, Distance2DToLocalPlayer);
        const float degrees = pitchInRadians * (180.0f / M_PI);
        return degrees;
    }

    float calcDesiredYaw() {
        if (IsLocal)
            return 0;
        const Vector2D shift = Vector2D(100000, 100000);
        const Vector2D originA = Myself->LocalOrigin.To2D().Add(shift);
        const Vector2D originB = localOrigin_predicted.To2D().Add(shift);
        const Vector2D diff = originB.Subtract(originA);
        const double yawInRadians = std::atan2(diff.y, diff.x);
        const float degrees = yawInRadians * (180.0f / M_PI);
        return degrees;
    }

    Vector2D calcDesiredAnglesIncrement() {
        return Vector2D(calcPitchIncrement(), calcYawIncrement());
    }

    Vector2D calcDesiredAngles() {
        return Vector2D(calcDesiredPitch(), calcDesiredYaw());
    }
};