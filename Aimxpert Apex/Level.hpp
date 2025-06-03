

namespace LvMap
{
    bool m_mixtape;
}

struct Level
{
    std::string Name;
    bool IsPlayable;
    bool IsFiringRange;
    bool m_mixtape;
    char gameMode[64] = { 0 };
    std::unordered_map<std::string, bool> mixtape = { {"control", true}, {"freedm", true}, {"arenas", true} };

    void Read() {
        Name = aimxpert::ReadString(OFF_REGION + OFF_LEVEL, 1024);
        IsPlayable = !Name.empty() && Name != "maps/mp_lobby.bsp";
        IsFiringRange = Name == "maps/mp_rr_canyonlands_staging_mu1.bsp";
        if (IsPlayable || IsFiringRange) {
            uint64_t gameModePtr = aimxpert::read<uint64_t>(OFF_REGION + OFF_GAMEMODE + 0x50);
            if (gameModePtr > 0) {
                aimxpert::read(gameModePtr, &gameMode, sizeof(gameMode));
                m_mixtape = mixtape[gameMode];
                LvMap::m_mixtape = m_mixtape;
            }
        }
    }
};