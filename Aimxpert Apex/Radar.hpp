#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <vector>

#define DEG2RAD(x) ((float)(x) * (float)(M_PI / 180.f))
struct Radar
{
	Camera* GameCamera;
	Level* m_level;
	LocalPlayer* Myself;
	std::vector<Player*>* Players;
	std::chrono::milliseconds LastUpdateTime;

	Radar(std::vector<Player*>* Players, Camera* GameCamera, Level* level, LocalPlayer* localplayer) {
		this->Players = Players;
		this->GameCamera = GameCamera;
		this->m_level = level;
		this->Myself = localplayer;
	}

	int GetTeamId(uint64_t entity)
	{
		return aimxpert::read<int>(entity + OFF_TEAM_NUMBER);
	}

	void SetTeamId(uint64_t entity, int teamId)
	{
		aimxpert::write<int>(entity + OFF_TEAM_NUMBER, teamId);
	}

	void ActivateBigMap() {
		if (!Settings::Radar::BigMap)
			return;

		if (!m_level->IsPlayable) {
			return;
		}

		if (InputManager::isKeyDownOrPress(Settings::Radar::BigMapBind)) {
			int localPlayerTeamID = GetTeamId(Myself->BasePointer);
			float curTime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE);
			double continueTime = 0.2;
			float endTime = curTime + continueTime;
			while (curTime < endTime)
			{
				SetTeamId(Myself->BasePointer, 1);
				curTime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE);
			}

			curTime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE);
			endTime = curTime + continueTime;
			while (curTime < endTime)
			{
				SetTeamId(Myself->BasePointer, localPlayerTeamID);
				curTime = aimxpert::read<float>(Myself->BasePointer + OFF_TIME_BASE);
			}
		}
	}

	void RenderDrawings(LocalPlayer* Myself) {
		int ScreenWidth;
		int ScreenHeight;
		if (!m_level->IsPlayable)
			return;

		if (Settings::Radar::MiniMap) {
			ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(ImVec2(0.0f, Center.y), ImGuiCond_Once, ImVec2(0.02f, 0.5f));
			ImGui::Begin(("Radar"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
			for (auto i = 0; i < Players->size(); i++) {
				Player* p = Players->at(i);
				if (!p->IsHostile || !p->IsValid() || Myself->BasePointer == p->BasePointer)
					continue;

				float radardistance = (int)((Myself->LocalOrigin, p->Distance2DToLocalPlayer) / 39.62);

				MiniMapRadar(p->LocalOrigin, Myself->LocalOrigin, p->ViewAngles.y, radardistance, p->Team, p->ViewYaw);
			}
			ImGui::End();
		}
	}

	void DrawRadarPointMiniMap(Vector3D EnemyPos, Vector3D LocalPos, float targetY, float enemyDist, int TeamID, int xAxis, int yAxis, int width, int height, ImColor color, float targetyaw) {
		Vector3D siz;
		siz.x = width;
		siz.y = height;
		Vector3D pos;
		pos.x = xAxis;
		pos.y = yAxis;
		bool ck = false;

		Vector3D single = Renderer::RotatePoint(EnemyPos, LocalPos, pos.x, pos.y, siz.x, siz.y, targetY, 0.3f, &ck);
		if (enemyDist >= 0.f && enemyDist < Settings::Radar::MiniMapRange) {
			for (int i = 1; i <= 30; i++) {
				if (Settings::Radar::EnemyIdentifier == 0)
					Renderer::TeamMiniMap_Arrow(single.x, single.y, Settings::Radar::IdentifierSize, TeamID, targetyaw);
				if (Settings::Radar::EnemyIdentifier == 1)
					Renderer::TeamMiniMap_Circle(single.x, single.y, Settings::Radar::IdentifierSize, TeamID, targetyaw, Settings::Radar::IdentifierSize, Settings::Radar::MiniMapBlackBGSize);
				if (Settings::Radar::EnemyViewAngles)
					Renderer::TeamMiniMap_ViewAngles(single.x, single.y, Settings::Radar::IdentifierSize, targetyaw, Settings::Radar::EnemyViewAnglesLength, ImColor(Settings::Radar::EnemyViewAnglesColor[0], Settings::Radar::EnemyViewAnglesColor[1], Settings::Radar::EnemyViewAnglesColor[2], Settings::Radar::EnemyViewAnglesColor[3]));
			}
		}
	}

	void MiniMapRadar(Vector3D EnemyPos, Vector3D LocalPos, float targetY, float eneamyDist, int TeamId, float targetyaw) {
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowRounding = Settings::Radar::RadarRounding;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(Settings::Radar::BackgroundColor[0], Settings::Radar::BackgroundColor[1], Settings::Radar::BackgroundColor[2], Settings::Radar::BackgroundColor[3]));
		ImGuiWindowFlags TargetFlags;
		TargetFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		if (Settings::Radar::MiniMap) {
			ImGui::SetNextWindowSize({ static_cast<float>(Settings::Radar::MiniMapScaleX), static_cast<float>(Settings::Radar::MiniMapScaleY) });
			ImGui::Begin(("##Radar"), 0, TargetFlags);
			{
				ImDrawList* Draw = ImGui::GetWindowDrawList();
				ImVec2 DrawPos = ImGui::GetCursorScreenPos();
				ImVec2 DrawSize = ImGui::GetContentRegionAvail();
				ImVec2 midRadar = ImVec2(DrawPos.x + (DrawSize.x / 2), DrawPos.y + (DrawSize.y / 2));
				if (Settings::Radar::MiniMapGuides) {
					ImVec2 startHorizontal(midRadar.x - DrawSize.x / 2.f, midRadar.y);
					ImVec2 endHorizontal(midRadar.x + DrawSize.x / 2.f, midRadar.y);
					ImVec2 startVertical(midRadar.x, midRadar.y - DrawSize.y / 2.f);
					ImVec2 endVertical(midRadar.x, midRadar.y + DrawSize.y / 2.f);
					float extension = -500000.f;

					ImGui::GetWindowDrawList()->AddLine(startHorizontal, ImVec2(startHorizontal.x - extension, endHorizontal.y), IM_COL32(255, 255, 255, 255));
					ImGui::GetWindowDrawList()->AddLine(endHorizontal, ImVec2(endHorizontal.x + extension, endHorizontal.y), IM_COL32(255, 255, 255, 255));

					ImGui::GetWindowDrawList()->AddLine(startVertical, ImVec2(startVertical.x, startVertical.y - extension), IM_COL32(255, 255, 255, 255));
					ImGui::GetWindowDrawList()->AddLine(endVertical, ImVec2(endVertical.x, endVertical.y + extension), IM_COL32(255, 255, 255, 255));
				}

				DrawRadarPointMiniMap(EnemyPos, LocalPos, targetY, eneamyDist, TeamId, DrawPos.x, DrawPos.y, DrawSize.x, DrawSize.y, { 255, 255, 255, 255 }, targetyaw);
			}
			ImGui::End();
		}
		ImGui::PopStyleColor();
	}
};