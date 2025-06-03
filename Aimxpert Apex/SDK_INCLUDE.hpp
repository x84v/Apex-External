
ImFont* FontMain = nullptr;
ImFont* FontSecondaryA = nullptr;
ImFont* FontSecondaryB = nullptr;
ImFont* FontSecondaryC = nullptr;
ImFont* ThickFontA = nullptr;
ImFont* ThickFontB = nullptr;
ImFont* ThickFontC = nullptr;

//Namespace Controls
#include "Controls.hpp"
InputManager* MouseInputs = new InputManager();

//SDK Includes
#include "Driver.h"
#include "Matrix.h"
#include "Offsets.hpp"
#include "Resolver.hpp"

//Setup
#include "Render.hpp"

//Resources
#include "Camera.hpp"
#include "LocalPlayer.hpp"
#include "Level.hpp"
#include "GlowMode.hpp"
#include "Player.hpp"

//Cheat
#include "Aimbot.hpp"
#include "Visuals.hpp"
#include "Misc.hpp"
#include "Radar.hpp"
#include "Triggerbot.hpp"

//Interface
#include "ImGui.h"
#include "Menu.h"

Level* Map = new Level();
LocalPlayer* Myself = new LocalPlayer();
Camera* GameCamera = new Camera();
std::vector<Player*>* HumanPlayers = new std::vector<Player*>;
std::vector<Player*>* Dummies = new std::vector<Player*>;
std::vector<Player*>* Players = new std::vector<Player*>;
Sense* ESP = new Sense(Map, Players, GameCamera, Myself);
Aimbot* AimAssist = new Aimbot(Myself, Players);
Radar* MapRadar = new Radar(Players, GameCamera, Map, Myself);
Misc* MiscTab = new Misc(Map, Myself, Players);


void __cdecl GameDataThread(void*) {
    while (true) {
        Map->Read();
        if (!Map->IsPlayable) continue;

        Myself->ReadGameData();
        if (!Myself->IsValid()) continue;

        std::vector<Player*> RemovePlayers;
        std::vector<Player*> VaildPlayers;

        auto& PlayerList = Map->IsFiringRange ? *Dummies : *HumanPlayers;
        for (auto& p : PlayerList) {

            p->Read();
            if (p->BasePointer == 0 || !p->IsValid() || p->IsDead) {
                RemovePlayers.push_back(p);
                continue; 
            }

            if (p->IsPlayer() || p->IsDummy()) {
                VaildPlayers.push_back(p);
            }
        }

        for (auto& p : RemovePlayers) {
            auto it = std::find(Players->begin(), Players->end(), p);
            if (it != Players->end()) {
                Players->erase(it);
            }
        }

        for (auto& p : VaildPlayers) {
            if (std::find(Players->begin(), Players->end(), p) == Players->end()) {
                Players->emplace_back(p);
            }
        }

        ESP->Update();
        ESP->ViewModelGlow();
        MapRadar->ActivateBigMap();
        MiscTab->Update();
    }
}



void __cdecl LocalPlayerThreadRun(void*) {
	while (1) {
		if (!Map->IsPlayable) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
			continue;
		}

		if (!Myself->IsValid()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
			continue;
		}

		if (Settings::Misc::SuperGlide)
			MiscTab->SuperGlide();

		if (Settings::Misc::BHop)
			MiscTab->BHop();

		if (Settings::Misc::WallJump)
			MiscTab->WallJump();

		if (Settings::Misc::AutoTapStrafe)
			MiscTab->AutoTapStrafe();

		std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
	}
}