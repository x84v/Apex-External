
namespace VisualSettings {
	bool GlowEnabled = true;
	float GlowMaxDistance = 200;
	bool DrawBoxes = false;
	int ViewmodelGlowID;
	int ItemGlowInsideFunction = 0;
	int ItemGlowOutlineFunction = 138;
	const std::vector<uint8_t> ItemHighlightID = { 15, 42, 47, 54, 65, 9, 58 };
	const GlowMode SetGlowOff = { 0, 0, 0, 0 };
	std::chrono::milliseconds LastLoopTime;
	std::vector<GlowMode>* StoredGlowMode = new std::vector<GlowMode>;
	bool ItemGlow = true;
	bool DrawSeer = false;
	bool DrawTracers = false;
	bool DrawDistance = true;
	bool DrawFOVCircle = true;
	bool AimedAtOnly = false;
	bool VisWarning;
	float SeerMaxDistance = 200;
	float GameFOV = 120;
	int SelectedLegitbotSubTab = 0;	  // 6
	int SelectedRagebotSubTab = 0;	  // 1
	int SelectedFlickbotSubTab = 0;	  // 2
	int SelectedTriggerbotSubTab = 0; // 2
	int SelectedGlowSubTab = 0;		  // 2
	int SelectedESPSubTabLeft = 0;	  // 3
	int SelectedESPSubTabRight = 0;	  // 4
	int SelectedESPSubTabRightv2 = 0;
	int SelectedMiscSubTab = 0;		  // 4
	int SelectedSettingsSubTab = 0;	  // 2
	int SelectedConfigSubTab = 0;	  // 1
	float Speed = 1;
	float Smooth = 18;
	float FOV = 7;
	bool NoRecoil = true;
	float NoRecoilRate = 15;
	int TotalSpectators = 0;
}


#undef min  
struct Sense {
	Level* Map;
	Camera* GameCamera;
	LocalPlayer* Myself;
	std::vector<Player*>* Players;
	std::chrono::milliseconds LastUpdateTime;
	std::vector<std::string> Spectators;

	Sense(Level* Map, std::vector<Player*>* Players, Camera* GameCamera, LocalPlayer* Myself) {
		this->Players = Players;
		this->Map = Map;
		this->GameCamera = GameCamera;
		this->Myself = Myself;
	}

	void Initialize() {
		for (int placebo = 0; placebo < 31; placebo++) {
			const GlowMode Ehh = { 135, 132, 35, 127 };
			VisualSettings::StoredGlowMode->push_back(Ehh);
		}

		const GlowMode FirstStyle = { 135, 135, 128, 64 };
		const GlowMode SecondStyle = { 135, 135, 160, 64 };
		const GlowMode ThirdStyle = { 135, 135, 255, 64 };
		const GlowMode FourthStyle = { 135, 135, 32, 64 };

		VisualSettings::StoredGlowMode->push_back(FirstStyle);
		VisualSettings::StoredGlowMode->push_back(SecondStyle);
		VisualSettings::StoredGlowMode->push_back(ThirdStyle);
		VisualSettings::StoredGlowMode->push_back(FourthStyle);
	}

	void UpdateSpectators() {
		std::chrono::milliseconds Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		if (Now >= LastUpdateTime + std::chrono::milliseconds(500)) {
			int TempTotalSpectators = 0;
			std::vector<std::string> TempSpectators;

			for (auto p : *Players) {
				if (p->IsSpectating()) {
					TempTotalSpectators++;
					TempSpectators.push_back(p->GetPlayerName());
				}
			}

			Settings::TotalSpectators = TempTotalSpectators;
			Spectators = TempSpectators;
			LastUpdateTime = Now;
		}
	}


	void RenderDrawings(Aimbot* AimAssistState, LocalPlayer* Myself) {

		ImDrawList* Canvas = ImGui::GetForegroundDrawList();
		if (Settings::Sense::DrawFOVCircle && Myself->IsCombatReady()) {
			float FOVV = std::min(FOV, FOV * (AimAssistState->GetFOVScale() * Settings::Aimbot::ZoomScale));
			float Radius = tanf(DEG2RAD(FOVV) / 2) / tanf(DEG2RAD(Settings::Sense::GameFOV) / 2) * Settings::ScreenWidth;
			Renderer::DrawCircle(Canvas, Vector2D(Settings::ScreenWidth / 2, Settings::ScreenHeight / 2), Radius, 120, ImColor(Settings::Colors::FOVColor[0], Settings::Colors::FOVColor[1], Settings::Colors::FOVColor[2], Settings::Colors::FOVColor[3]), Settings::Sense::FOVThickness);
		}

		if (Settings::Sense::DrawFilledFOVCircle && Myself->IsCombatReady()) {
			float FOVV = std::min(FOV, FOV * (AimAssistState->GetFOVScale() * Settings::Aimbot::ZoomScale));
			float Radius = tanf(DEG2RAD(FOVV) / 2) / tanf(DEG2RAD(Settings::Sense::GameFOV) / 2) * Settings::ScreenWidth;
			Renderer::DrawCircleFilled( Canvas, Vector2D(Settings::ScreenWidth / 2, Settings::ScreenHeight / 2), Radius, 40, ImColor(Settings::Colors::FilledFOVColor[0], Settings::Colors::FilledFOVColor[1], Settings::Colors::FilledFOVColor[2], Settings::Colors::FilledFOVColor[3]));
		}

		for (int i = 0; i < Players->size(); i++) {
			Player* p = Players->at(i);

			if (p->IsLocal)
				continue;

			if (!p->IsValid())
				continue;

			if (p->DistanceToLocalPlayer > Conversion::ToGameUnits(Settings::Sense::ESPMaxDistance))
				continue;

			if (Settings::Sense::VisibilityCheck && !p->IsVisible)
				continue;

			if (!Settings::Sense::Teammate::DrawTeam && p->IsAlly) 
				continue;

			if (Settings::Sense::KnockedCheck && p->IsKnocked)
				continue;

			Vector3D LocalOrigin3D = p->LocalOrigin;
			Vector3D Head3D = p->GetBonePosition(HitboxType::Head);
			Vector2D LocalOriginW2S, HeadPositionW2S;
			bool bLocalOriginW2SValid = GameCamera->WorldToScreen(LocalOrigin3D, LocalOriginW2S);
			bool bHeadPositionW2SValid = GameCamera->WorldToScreen(Head3D, HeadPositionW2S);

			auto GetColor = [p](const float* Teammate_VisColor, const float* Teammate_InvisColor, const float* Teammate_KnockedColor, const float* Enemy_VisColor, const float* Enemy_InvisColor, const float* Enemy_KnockedColor) {
				const float* Teammate_Color = p->IsKnocked ? Teammate_KnockedColor : p->IsVisible ? Teammate_VisColor : Teammate_InvisColor;
				const float* Enemy_Color = p->IsKnocked ? Enemy_KnockedColor : p->IsVisible ? Enemy_VisColor : Enemy_InvisColor;
				const float* ChosenColor = p->IsAlly ? Teammate_Color : Enemy_Color;
				return ImVec4(ChosenColor[0], ChosenColor[1], ChosenColor[2], ChosenColor[3]);
			};


			ImVec4 BoxColor = GetColor(Settings::Colors::Teammate::VisibleBoxColor, Settings::Colors::Teammate::InvisibleBoxColor, Settings::Colors::Teammate::KnockedBoxColor, Settings::Colors::Enemy::VisibleBoxColor, Settings::Colors::Enemy::InvisibleBoxColor, Settings::Colors::Enemy::KnockedBoxColor);
			ImVec4 FilledBoxColor = GetColor(Settings::Colors::Teammate::VisibleFilledBoxColor, Settings::Colors::Teammate::InvisibleFilledBoxColor, Settings::Colors::Teammate::KnockedFilledBoxColor, Settings::Colors::Enemy::VisibleFilledBoxColor, Settings::Colors::Enemy::InvisibleFilledBoxColor, Settings::Colors::Enemy::KnockedFilledBoxColor);
			ImVec4 TracerColor = GetColor(Settings::Colors::Teammate::VisibleTracerColor, Settings::Colors::Teammate::InvisibleTracerColor, Settings::Colors::Teammate::KnockedTracerColor, Settings::Colors::Enemy::VisibleTracerColor, Settings::Colors::Enemy::InvisibleTracerColor, Settings::Colors::Enemy::KnockedTracerColor);
			ImVec4 SkeletonColor = GetColor(Settings::Colors::Teammate::VisibleSkeletonColor, Settings::Colors::Teammate::InvisibleSkeletonColor, Settings::Colors::Teammate::KnockedSkeletonColor, Settings::Colors::Enemy::VisibleSkeletonColor, Settings::Colors::Enemy::InvisibleSkeletonColor, Settings::Colors::Enemy::KnockedSkeletonColor);
			ImVec4 DistanceColor = GetColor(Settings::Colors::Teammate::VisibleDistanceColor, Settings::Colors::Teammate::InvisibleDistanceColor, Settings::Colors::Teammate::KnockedDistanceColor, Settings::Colors::Enemy::VisibleDistanceColor, Settings::Colors::Enemy::InvisibleDistanceColor, Settings::Colors::Enemy::KnockedDistanceColor);
			auto CalcTextPosition = [](const int PositionIndex, const Vector2D& Head, const Vector2D& LocalOrigin) {
				switch (PositionIndex) {
				case 0: // Top 1
					return Head.Add(Vector2D(0, ((Head.y - LocalOrigin.y) * 0.2) - 14));
				case 1: // Top 2
					return Head.Add(Vector2D(0, ((Head.y - LocalOrigin.y) * 0.2) - 28));
				case 2: // Bottom 1
					return LocalOrigin;
				case 3: // Bottom 2
					return LocalOrigin.Add(Vector2D(0, 10));
				case 4: // Bottom 3
					return LocalOrigin.Add(Vector2D(0, 20));
				default:
					return Vector2D();
				}
				};


			Vector2D NamePosition = CalcTextPosition(Settings::Sense::Positions::NamePosition, HeadPositionW2S, LocalOriginW2S);
			Vector2D DistancePosition = CalcTextPosition(Settings::Sense::Positions::DistancePosition, HeadPositionW2S, LocalOriginW2S);
			Vector2D LegendPosition = CalcTextPosition(Settings::Sense::Positions::LegendPosition, HeadPositionW2S, LocalOriginW2S);
			Vector2D WeaponPosition = CalcTextPosition(Settings::Sense::Positions::WeaponPosition, HeadPositionW2S, LocalOriginW2S);
			Vector2D StatusPosition = CalcTextPosition(Settings::Sense::Positions::StatusPosition, HeadPositionW2S, LocalOriginW2S);
			bool DrawBoxes = p->IsAlly ? Settings::Sense::Teammate::DrawBoxes : Settings::Sense::Enemy::DrawBoxes;
			bool DrawSkeleton = p->IsAlly ? Settings::Sense::Teammate::DrawSkeleton : Settings::Sense::Enemy::DrawSkeleton;
			bool DrawDistance = p->IsAlly ? Settings::Sense::Teammate::DrawDistance : Settings::Sense::Enemy::DrawDistance;
			bool DrawStatus = p->IsAlly ? Settings::Sense::Teammate::DrawStatus : Settings::Sense::Enemy::DrawStatus;
			bool DrawSeer = p->IsAlly ? Settings::Sense::Teammate::DrawSeer : Settings::Sense::Enemy::DrawSeer;

			VisualSettings::VisWarning = p->IsVisible;
			if (VisualSettings::DrawTracers) {
				Vector2D ScreenPosition;
				int TracerBone = p->IsAlly ? Settings::Sense::Teammate::TracerBone : Settings::Sense::Enemy::TracerBone;
				int TracerPosition = p->IsAlly ? Settings::Sense::Teammate::TracerPosition : Settings::Sense::Enemy::TracerPosition;
				float TracerThickness = p->IsAlly ? Settings::Sense::Teammate::TracerThickness : Settings::Sense::Enemy::TracerThickness;
				GameCamera->WorldToScreen(p->LocalOrigin.Add(Vector3D(0, 0, TracerBone == 0 ? 66 : 0)), ScreenPosition);

				Vector2D Position = Vector2D(Settings::ScreenWidth * 0.5f, TracerPosition == 0 ? 0 : TracerPosition == 1 ? Settings::ScreenHeight / 2 : Settings::ScreenHeight);
				if (!ScreenPosition.IsZeroVector())
					Renderer::DrawLine( Canvas, Position, ScreenPosition, TracerThickness, ImColor(TracerColor));
			}

			if (!bLocalOriginW2SValid)
				continue;
			if (!bHeadPositionW2SValid) 
				continue;

			if (DrawBoxes) {
				int BoxType = p->IsAlly ? Settings::Sense::Teammate::BoxType : Settings::Sense::Enemy::BoxType;
				int BoxStyle = p->IsAlly ? Settings::Sense::Teammate::BoxStyle : Settings::Sense::Enemy::BoxStyle;
				bool BoxOutline = p->IsAlly ? Settings::Sense::Teammate::BoxOutline : Settings::Sense::Enemy::BoxOutline;
				float BoxThickness = p->IsAlly ? Settings::Sense::Teammate::BoxThickness : Settings::Sense::Enemy::BoxThickness;
				Renderer::Draw2DBox( Canvas, BoxType, BoxStyle, BoxOutline, LocalOriginW2S, HeadPositionW2S, ImColor(BoxColor), ImColor(FilledBoxColor), BoxThickness);
			}


			if (Settings::Sense::DrawCrosshair) {
				int x = (int)(Settings::ScreenWidth * 0.5f);
				int y = (int)(Settings::ScreenHeight * 0.5f);
				Renderer::DrawLine( Canvas, Vector2D(x - Settings::Sense::CrosshairSize, y), Vector2D(x + Settings::Sense::CrosshairSize, y), Settings::Sense::CrosshairThickness, ImColor(Settings::Colors::CrosshairColor[0], Settings::Colors::CrosshairColor[1], Settings::Colors::CrosshairColor[2], Settings::Colors::CrosshairColor[3])); // Left - right
				Renderer::DrawLine( Canvas, Vector2D(x, y - Settings::Sense::CrosshairSize), Vector2D(x, y + Settings::Sense::CrosshairSize), Settings::Sense::CrosshairThickness, ImColor(Settings::Colors::CrosshairColor[0], Settings::Colors::CrosshairColor[1], Settings::Colors::CrosshairColor[2], Settings::Colors::CrosshairColor[3])); // Top - bottom
			}

			if (!DrawSeer) {
				int health = p->Health;
				int maxHealth = p->MaxHealth;
				int shield = p->Shield;
				int maxShield = p->MaxShield;
				int BarMode = p->IsAlly ? Settings::Sense::Teammate::BarMode : Settings::Sense::Enemy::BarMode;
				int BarStyle = p->IsAlly ? Settings::Sense::Teammate::BarStyle : Settings::Sense::Enemy::BarStyle;
				int BarColorMode = p->IsAlly ? Settings::Sense::Teammate::BarColorMode : Settings::Sense::Enemy::BarColorMode;
				float BarThickness = p->IsAlly ? Settings::Sense::Teammate::BarThickness : Settings::Sense::Enemy::BarThickness;
				float BarThickness2 = p->IsAlly ? Settings::Sense::Teammate::BarThickness2 : Settings::Sense::Enemy::BarThickness2;
				float BarWidth = p->IsAlly ? Settings::Sense::Teammate::BarWidth : Settings::Sense::Enemy::BarWidth;
				float BarHeight = p->IsAlly ? Settings::Sense::Teammate::BarHeight : Settings::Sense::Enemy::BarHeight;
				Renderer::Draw2DBarTest( Canvas, Settings::Sense::Enemy::HealthBar, Settings::Sense::Enemy::ShieldBar, BarStyle, BarColorMode, LocalOriginW2S, HeadPositionW2S, health, maxHealth, shield, maxShield, BarThickness, BarThickness2, BarWidth, BarHeight);
			}

			if (DrawSeer && bHeadPositionW2SValid) {
				if (!HeadPositionW2S.IsZeroVector()) {
					Renderer::DrawSeer( Canvas, HeadPositionW2S.x, HeadPositionW2S.y - 20, p->Shield, p->MaxShield, p->Health);
				}
			}

			if (DrawSkeleton) {
				int SkeletonDetail = p->IsAlly ? Settings::Sense::Teammate::SkeletonDetail : Settings::Sense::Enemy::SkeletonDetail;
				if (SkeletonDetail == 0) { 
					Vector2D Neck, UpperChest, LowerChest, Stomach, Leftshoulder, Leftelbow, LeftHand, Rightshoulder, RightelbowBone, RightHand, LeftThighs, Leftknees, Leftleg, RightThighs, Rightknees, Rightleg;	
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Neck), Neck);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::UpperChest), UpperChest);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LowerChest), LowerChest);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Stomach), Stomach);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftshoulder), Leftshoulder);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftelbow), Leftelbow);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Lefthand), LeftHand);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Rightshoulder), Rightshoulder);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightelbowBone), RightelbowBone);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Righthand), RightHand);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftThighs), LeftThighs);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftknees), Leftknees);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftleg), Leftleg);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightThighs), RightThighs);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Rightknees), Rightknees);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Rightleg), Rightleg);

					bool Outline = p->IsAlly ? Settings::Sense::Teammate::SkeletonOutline : Settings::Sense::Enemy::SkeletonOutline;
					int Thickness = p->IsAlly ? Settings::Sense::Teammate::SkeletonThickness : Settings::Sense::Enemy::SkeletonThickness;
					if (Outline) {
						Renderer::DrawLine( Canvas, HeadPositionW2S, Neck, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Neck, UpperChest, Thickness + 2.5, ImColor(0, 0, 0));

						Renderer::DrawLine( Canvas, UpperChest, LowerChest, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, LowerChest, Stomach, Thickness + 2.5, ImColor(0, 0, 0));

						Renderer::DrawLine( Canvas, Neck, Leftshoulder, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Leftshoulder, Leftelbow, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Leftelbow, LeftHand, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Neck, Rightshoulder, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Rightshoulder, RightelbowBone, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, RightelbowBone, RightHand, Thickness + 2.5, ImColor(0, 0, 0));

						Renderer::DrawLine( Canvas, Stomach, LeftThighs, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, LeftThighs, Leftknees, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Leftknees, Leftleg, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Stomach, RightThighs, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, RightThighs, Rightknees, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Rightknees, Rightleg, Thickness + 2.5, ImColor(0, 0, 0));
					}
					Renderer::DrawLine( Canvas, HeadPositionW2S, Neck, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Neck, UpperChest, Thickness, ImColor(SkeletonColor));

					Renderer::DrawLine( Canvas, UpperChest, LowerChest, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, LowerChest, Stomach, Thickness, ImColor(SkeletonColor));

					Renderer::DrawLine( Canvas, Neck, Leftshoulder, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Leftshoulder, Leftelbow, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Leftelbow, LeftHand, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Neck, Rightshoulder, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Rightshoulder, RightelbowBone, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, RightelbowBone, RightHand, Thickness, ImColor(SkeletonColor));

					Renderer::DrawLine( Canvas, Stomach, LeftThighs, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, LeftThighs, Leftknees, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Leftknees, Leftleg, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Stomach, RightThighs, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, RightThighs, Rightknees, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Rightknees, Rightleg, Thickness, ImColor(SkeletonColor));
				}

				else if (SkeletonDetail == 1) {
					Vector2D Neck, Stomach, Leftshoulder, Leftelbow, Lefthand, Rightshoulder, RightelbowBone, Righthand, LeftThighs, Leftknees, Leftleg, RightThighs, Rightknees, Rightleg;
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Neck), Neck);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Stomach), Stomach);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftshoulder), Leftshoulder);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftelbow), Leftelbow);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Lefthand), Lefthand);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Rightshoulder), Rightshoulder);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightelbowBone), RightelbowBone);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Righthand), Righthand);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::LeftThighs), LeftThighs);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftknees), Leftknees);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Leftleg), Leftleg);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::RightThighs), RightThighs);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Rightknees), Rightknees);
					GameCamera->WorldToScreen(p->GetBonePosition(HitboxType::Rightleg), Rightleg);

					bool Outline = p->IsAlly ? Settings::Sense::Teammate::SkeletonOutline : Settings::Sense::Enemy::SkeletonOutline;
					int Thickness = p->IsAlly ? Settings::Sense::Teammate::SkeletonThickness : Settings::Sense::Enemy::SkeletonThickness;
					if (Outline) {
						Renderer::DrawLine( Canvas, Neck, Stomach, Thickness + 2.5, ImColor(0, 0, 0));

						Renderer::DrawLine( Canvas, Neck, Leftshoulder, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Leftshoulder, Leftelbow, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Leftelbow, Lefthand, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Neck, Rightshoulder, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Rightshoulder, RightelbowBone, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, RightelbowBone, Righthand, Thickness + 2.5, ImColor(0, 0, 0));

						Renderer::DrawLine( Canvas, Stomach, LeftThighs, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, LeftThighs, Leftknees, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Leftknees, Leftleg, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Stomach, RightThighs, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, RightThighs, Rightknees, Thickness + 2.5, ImColor(0, 0, 0));
						Renderer::DrawLine( Canvas, Rightknees, Rightleg, Thickness + 2.5, ImColor(0, 0, 0));

					}
					Renderer::DrawLine( Canvas, Neck, Stomach, Thickness, ImColor(SkeletonColor));

					Renderer::DrawLine( Canvas, Neck, Leftshoulder, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Leftshoulder, Leftelbow, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Leftelbow, Lefthand, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Neck, Rightshoulder, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Rightshoulder, RightelbowBone, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, RightelbowBone, Righthand, Thickness, ImColor(SkeletonColor));

					Renderer::DrawLine( Canvas, Stomach, LeftThighs, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, LeftThighs, Leftknees, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Leftknees, Leftleg, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Stomach, RightThighs, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, RightThighs, Rightknees, Thickness, ImColor(SkeletonColor));
					Renderer::DrawLine( Canvas, Rightknees, Rightleg, Thickness, ImColor(SkeletonColor));
				}
			}

			if (DrawDistance) {
				ImGui::PushFont(FontSecondaryB);
				std::string Distance = std::to_string(static_cast<int>(Conversion::ToMeters(p->DistanceToLocalPlayer)));
				std::string Buffer = "(" + Distance + " M)";

				if (!LocalOriginW2S.IsZeroVector()) {
					Vector2D AdjustedDistancePosition = DistancePosition.Add(Vector2D(0, -10));
					Renderer::DrawText(Canvas, AdjustedDistancePosition, Buffer.c_str(), ImColor(DistanceColor), Settings::Sense::TextOutline, true, false);
				}

				ImGui::PopFont();
			}

			if (DrawStatus) {
				ImGui::PushFont(FontSecondaryB);
				std::stringstream healthValue, shieldValue, maxHealthValue, maxShieldValue;
				healthValue << p->Health;
				shieldValue << p->Shield;
				maxHealthValue << p->MaxHealth;
				maxShieldValue << p->MaxShield;
				std::string healthInt = healthValue.str() + " HP";
				std::string shieldInt = shieldValue.str() + " AP";
				const char* healthText = const_cast<char*>(healthInt.c_str());
				const char* shieldText = const_cast<char*>(shieldInt.c_str());
				std::string combinedHealth = healthValue.str() + " / " + maxHealthValue.str() + " HP";
				const char* combinedHealthText = combinedHealth.c_str();
				std::string combinedShield = shieldValue.str() + " / " + maxShieldValue.str() + " AP";
				const char* combinedShieldText = combinedShield.c_str();

				ImColor ShieldColor;
				switch (p->MaxShield) {
				case 75:
					ShieldColor = ImColor(39, 178, 255); // Blue
					break;
				case 100:
					ShieldColor = ImColor(206, 59, 255); // Purple
					break;
				case 125:
					ShieldColor = ImColor(219, 2, 2); // Red
					break;
				default:
					ShieldColor = ImColor(247, 247, 247); // White
					break;
				}

				bool ShowMaxStatusValues = p->IsAlly ? Settings::Sense::Teammate::ShowMaxStatusValues : Settings::Sense::Enemy::ShowMaxStatusValues;
				Renderer::DrawText(Canvas, StatusPosition, ShowMaxStatusValues ? combinedHealthText : healthText, ImColor(0, 255, 0), Settings::Sense::TextOutline, true, false);
				Renderer::DrawText(Canvas, StatusPosition.Add(Vector2D(0, 0 + 15)), ShowMaxStatusValues ? combinedShieldText : shieldText, ShieldColor, Settings::Sense::TextOutline, true, false);
				ImGui::PopFont();
			}
		}

		if (AimAssistState->TargetSelected && AimAssistState->CurrentTarget) {
			Vector2D headScreenPosition;
			GameCamera->WorldToScreen(AimAssistState->CurrentTarget->GetBonePosition(HitboxType::LowerChest), headScreenPosition);
			if (headScreenPosition.IsZeroVector())
				return;

			Renderer::DrawSeer( Canvas, headScreenPosition.x, headScreenPosition.y - 20, AimAssistState->CurrentTarget->Shield, AimAssistState->CurrentTarget->MaxShield, AimAssistState->CurrentTarget->Health);
			return;
		}
	}

	void SetGlowState(ULONG64 HighlightSettingsPointer, long HighlightSize, int HighlightID, GlowMode NewGlowMode) {
		const GlowMode oldGlowMode = aimxpert::read<GlowMode>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 0);
		if (NewGlowMode != oldGlowMode)
			aimxpert::write<GlowMode>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 0, NewGlowMode);
	}

	void SetColorState(ULONG64 HighlightSettingsPointer, long HighlightSize, int HighlightID, Color NewColor) {
		const Color oldColor = aimxpert::read<Color>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 4);
		if (oldColor != NewColor)
			aimxpert::write<Color>(HighlightSettingsPointer + (HighlightSize * HighlightID) + 4, NewColor);
	}

	void setGlowEnable(Player* Target, int glowEnable) {
		ULONG64 ptrLong = Target->BasePointer + OFF_GLOW_ENABLE;
		aimxpert::write<int>(ptrLong, glowEnable);
	}
	void setGlowThroughWall(Player* Target, int glowThroughWall) {
		ULONG64 ptrLong = Target->BasePointer + OFF_GLOW_THROUGH_WALL;
		aimxpert::write<int>(ptrLong, glowThroughWall);
	}
	int getGlowThroughWall(Player* Target) {
		int ptrInt = aimxpert::read<int>(Target->BasePointer + OFF_GLOW_THROUGH_WALL);
		if (!aimxpert::IsValidPointer(ptrInt))
			return -1;
		return ptrInt;
	}
	int getGlowEnable(Player* Target) {
		int ptrInt = aimxpert::read<int>(Target->BasePointer + OFF_GLOW_ENABLE);
		if (!aimxpert::IsValidPointer(ptrInt))
			return -1;
		return ptrInt;
	}

	void setCustomGlow(Player* Target, bool isVisible, bool isKnocked, bool isSameTeam, bool GlowEnabled) {
		if (GlowEnabled) {
			static const int contextId = 0; // 
			ULONG64 basePointer = Target->BasePointer;
			int settingIndex = 63;

			// Glow
			int InsideFunction = 2;    // Leave
			int OutlineFunction = 125; // Leave

			// Custom Glow Body Style
			int bodyStyleArray[] = {
				0, // None
				1, // Pink
				110, // Pink Visible Only
				117, // Pulsing 1
				12, // Pulsing Line Invisible Only
				13, // Dark Pulsing Line
				124, // Sharp Pulsing Visible Only
				126, // Sharp Pulsing
				3, // Pulsing Red Line
				-500, // Fast Pulsing Invisible
				132, // Pulsing Up
				14, // Solid Pulsing
				117, // Solid Pulsing 2
				109, // Bright
				118, // Bright 2
				101, // Light
				112, // Light Solid
				115, // Red Pulsing Visible Only
				103, // Wave
				136, // Shaded Visible
				134, // Wireframe
				133, // Wireframe Visible Only
				75, // Black
				77 // Black Visible Only
			};

			int outlineStyleArray[] = {
				0, // None
				6, // Bright
				102, // Bright Invisible Only
				101, // Dark
				1, // Pink
				4, // White
				5, // Gold Flashing
				7, // Gold
				8, // Brown
				103, // Wave
				107, // Red
				108, // Red Bright
				110, // Heartbeat Visible Only
				-372, // Green Invisible Only
				-249, // Orange
				-151, // Visible Only
				364 // Red
			};

			if (Settings::Glow::BodyStyle >= 0 && Settings::Glow::BodyStyle <= 23)
				InsideFunction = bodyStyleArray[Settings::Glow::BodyStyle];

			if (Settings::Glow::OutlineStyle >= 0 && Settings::Glow::OutlineStyle <= 16)
				OutlineFunction = outlineStyleArray[Settings::Glow::OutlineStyle];

			std::array<unsigned char, 4> highlightFunctionBits = {
				InsideFunction,             // InsideFunction							2
				OutlineFunction,            // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE			125
				Settings::Glow::GlowRadius, // OutlineRadius: size * 255 / 8				64
				64                          // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7) 	64
			};
			std::array<float, 3> glowColorRGB = { 0, 0, 0 };
			if (Settings::Glow::GlowColorMode == 0) {
				int shield = Target->Shield;
				int maxShield = Target->MaxShield;
				if (Settings::Glow::GlowColorShieldMode == 0) { // Current Shield
					if (Settings::Glow::KnockedCheck) {
						if (isVisible) {
							settingIndex = 63;
							glowColorRGB = { Settings::Colors::Enemy::VisibleGlowColor[0], Settings::Colors::Enemy::VisibleGlowColor[1], Settings::Colors::Enemy::VisibleGlowColor[2] }; // Visible Enemies
						}
						else if (!isVisible) {
							if (shield >= 101) { // Red Shield
								settingIndex = 66;
								glowColorRGB = { Settings::Colors::Enemy::RedShieldColor[0], Settings::Colors::Enemy::RedShieldColor[1], Settings::Colors::Enemy::RedShieldColor[2] }; // red shield
							}
							else if (shield >= 76) { // Purple Shield
								settingIndex = 67;
								glowColorRGB = { Settings::Colors::Enemy::PurpleShieldColor[0], Settings::Colors::Enemy::PurpleShieldColor[1], Settings::Colors::Enemy::PurpleShieldColor[2] }; // purple shield
							}
							else if (shield >= 51) { // Blue Shield
								settingIndex = 68;
								glowColorRGB = { Settings::Colors::Enemy::BlueShieldColor[0], Settings::Colors::Enemy::BlueShieldColor[1], Settings::Colors::Enemy::BlueShieldColor[2] }; // blue shield
							}
							else if (shield >= 1) { // Grey Shield
								settingIndex = 69;
								glowColorRGB = { Settings::Colors::Enemy::GreyShieldColor[0], Settings::Colors::Enemy::GreyShieldColor[1], Settings::Colors::Enemy::GreyShieldColor[2] }; // gray shield 
							}
							else if (shield == 0) { // Cracked / No Shield
								settingIndex = 70;
								glowColorRGB = { Settings::Colors::Enemy::LowGlowColor[0], Settings::Colors::Enemy::LowGlowColor[1], Settings::Colors::Enemy::LowGlowColor[2] }; // low health enemies
							}
						}
					}
					else if (!Settings::Glow::KnockedCheck) {
						if (!isKnocked) {
							if (isVisible) {
								settingIndex = 64;
								glowColorRGB = { Settings::Colors::Enemy::VisibleGlowColor[0], Settings::Colors::Enemy::VisibleGlowColor[1], Settings::Colors::Enemy::VisibleGlowColor[2] }; // Visible Enemies
							}
							else if (!isVisible) {
								if (shield >= 101) { // Red Shield
									settingIndex = 66;
									glowColorRGB = { Settings::Colors::Enemy::RedShieldColor[0], Settings::Colors::Enemy::RedShieldColor[1], Settings::Colors::Enemy::RedShieldColor[2] }; // red shield
								}
								else if (shield >= 76) { // Purple Shield
									settingIndex = 67;
									glowColorRGB = { Settings::Colors::Enemy::PurpleShieldColor[0], Settings::Colors::Enemy::PurpleShieldColor[1], Settings::Colors::Enemy::PurpleShieldColor[2] }; // purple shield
								}
								else if (shield >= 51) { // Blue Shield
									settingIndex = 68;
									glowColorRGB = { Settings::Colors::Enemy::BlueShieldColor[0], Settings::Colors::Enemy::BlueShieldColor[1], Settings::Colors::Enemy::BlueShieldColor[2] }; // blue shield
								}
								else if (shield >= 1) { // Grey Shield
									settingIndex = 69;
									glowColorRGB = { Settings::Colors::Enemy::GreyShieldColor[0], Settings::Colors::Enemy::GreyShieldColor[1], Settings::Colors::Enemy::GreyShieldColor[2] }; // gray shield 
								}
								else if (shield == 0) { // Cracked / No Shield
									settingIndex = 70;
									glowColorRGB = { Settings::Colors::Enemy::LowGlowColor[0], Settings::Colors::Enemy::LowGlowColor[1], Settings::Colors::Enemy::LowGlowColor[2] }; // low health enemies
								}
							}
						}
						else if (isKnocked) {
							settingIndex = 64;
							glowColorRGB = { Settings::Colors::Enemy::KnockedGlowColor[0], Settings::Colors::Enemy::KnockedGlowColor[1], Settings::Colors::Enemy::KnockedGlowColor[2] }; // Knocked Enemies
						}
					}
				}

				if (Settings::Glow::GlowColorShieldMode == 1) { // Max Shield
					if (Settings::Glow::KnockedCheck) {
						if (isVisible) {
							settingIndex = 63;
							glowColorRGB = { Settings::Colors::Enemy::VisibleGlowColor[0], Settings::Colors::Enemy::VisibleGlowColor[1], Settings::Colors::Enemy::VisibleGlowColor[2] }; // Visible Enemies
						}
						else if (!isVisible) {
							if (shield != 0 && maxShield == 125) { // Red Shield
								settingIndex = 66;
								glowColorRGB = { Settings::Colors::Enemy::RedShieldColor[0], Settings::Colors::Enemy::RedShieldColor[1], Settings::Colors::Enemy::RedShieldColor[2] }; // red shield
							}
							else if (shield != 0 && maxShield == 100) { // Purple Shield
								settingIndex = 67;
								glowColorRGB = { Settings::Colors::Enemy::PurpleShieldColor[0], Settings::Colors::Enemy::PurpleShieldColor[1], Settings::Colors::Enemy::PurpleShieldColor[2] }; // purple shield
							}
							else if (shield != 0 && maxShield == 75) { // Blue Shield
								settingIndex = 68;
								glowColorRGB = { Settings::Colors::Enemy::BlueShieldColor[0], Settings::Colors::Enemy::BlueShieldColor[1], Settings::Colors::Enemy::BlueShieldColor[2] }; // blue shield
							}
							else if (shield != 0 && maxShield == 50) { // Grey Shield
								settingIndex = 69;
								glowColorRGB = { Settings::Colors::Enemy::GreyShieldColor[0], Settings::Colors::Enemy::GreyShieldColor[1], Settings::Colors::Enemy::GreyShieldColor[2] }; // gray shield 
							}
							else if (shield == 0) { // Cracked / No Shield
								settingIndex = 70;
								glowColorRGB = { Settings::Colors::Enemy::LowGlowColor[0], Settings::Colors::Enemy::LowGlowColor[1], Settings::Colors::Enemy::LowGlowColor[2] }; // low health enemies
							}
						}
					}
					else if (!Settings::Glow::KnockedCheck) {
						if (!isKnocked) {
							if (isVisible) {
								settingIndex = 63;
								glowColorRGB = { Settings::Colors::Enemy::VisibleGlowColor[0], Settings::Colors::Enemy::VisibleGlowColor[1], Settings::Colors::Enemy::VisibleGlowColor[2] }; // Visible Enemies
							}
							else if (!isVisible) {
								if (shield != 0 && maxShield == 125) { // Red Shield
									settingIndex = 66;
									glowColorRGB = { Settings::Colors::Enemy::RedShieldColor[0], Settings::Colors::Enemy::RedShieldColor[1], Settings::Colors::Enemy::RedShieldColor[2] }; // red shield
								}
								else if (shield != 0 && maxShield == 100) { // Purple Shield
									settingIndex = 67;
									glowColorRGB = { Settings::Colors::Enemy::PurpleShieldColor[0], Settings::Colors::Enemy::PurpleShieldColor[1], Settings::Colors::Enemy::PurpleShieldColor[2] }; // purple shield
								}
								else if (shield != 0 && maxShield == 75) { // Blue Shield
									settingIndex = 68;
									glowColorRGB = { Settings::Colors::Enemy::BlueShieldColor[0], Settings::Colors::Enemy::BlueShieldColor[1], Settings::Colors::Enemy::BlueShieldColor[2] }; // blue shield
								}
								else if (shield != 0 && maxShield == 50) { // Grey Shield
									settingIndex = 69;
									glowColorRGB = { Settings::Colors::Enemy::GreyShieldColor[0], Settings::Colors::Enemy::GreyShieldColor[1], Settings::Colors::Enemy::GreyShieldColor[2] }; // gray shield 
								}
								else if (shield == 0) { // Cracked / No Shield
									settingIndex = 70;
									glowColorRGB = { Settings::Colors::Enemy::LowGlowColor[0], Settings::Colors::Enemy::LowGlowColor[1], Settings::Colors::Enemy::LowGlowColor[2] }; // low health enemies
								}
							}
						}
						else if (isKnocked) {
							settingIndex = 70;
							glowColorRGB = { Settings::Colors::Enemy::KnockedGlowColor[0], Settings::Colors::Enemy::KnockedGlowColor[1], Settings::Colors::Enemy::KnockedGlowColor[2] }; // Knocked Enemies
						}
					}
				}
			}

			if (Settings::Glow::GlowColorMode == 1) {
				if (Settings::Glow::KnockedCheck) {
					if (isVisible) {
						settingIndex = 63;
						glowColorRGB = { Settings::Colors::Enemy::VisibleGlowColor[0], Settings::Colors::Enemy::VisibleGlowColor[1], Settings::Colors::Enemy::VisibleGlowColor[2] }; // Visible Enemies
					}
					else if (!isVisible) {
						settingIndex = 70;
						glowColorRGB = { Settings::Colors::Enemy::InvisibleGlowColor[0], Settings::Colors::Enemy::InvisibleGlowColor[1], Settings::Colors::Enemy::InvisibleGlowColor[2] }; // Invisible Enemies
					}
				}
				else if (!Settings::Glow::KnockedCheck) {
					if (!isKnocked && isVisible) {
						settingIndex = 63;
						glowColorRGB = { Settings::Colors::Enemy::VisibleGlowColor[0], Settings::Colors::Enemy::VisibleGlowColor[1], Settings::Colors::Enemy::VisibleGlowColor[2] }; // Visible Enemies
					}
					else if (!isKnocked && !isVisible) {
						settingIndex = 66;
						glowColorRGB = { Settings::Colors::Enemy::InvisibleGlowColor[0], Settings::Colors::Enemy::InvisibleGlowColor[1], Settings::Colors::Enemy::InvisibleGlowColor[2] }; // Invisible Enemies
					}
					else if (isKnocked) {
						settingIndex = 70;
						glowColorRGB = { Settings::Colors::Enemy::KnockedGlowColor[0], Settings::Colors::Enemy::KnockedGlowColor[1], Settings::Colors::Enemy::KnockedGlowColor[2] }; // Knocked Enemies
					}
				}
			}

			aimxpert::write<unsigned char>(Target->BasePointer + OFF_GLOW_HIGHLIGHT_ID + contextId, settingIndex);
			ULONG64 highlightSettingsPtr = aimxpert::read<ULONG64>(OFF_REGION + OFF_GLOW_HIGHLIGHTS);
			if (!isSameTeam) {
				aimxpert::write<decltype(highlightFunctionBits)>(highlightSettingsPtr + OFF_HIGHLIGHT_TYPE_SIZE * settingIndex + 0, highlightFunctionBits);
				aimxpert::write<decltype(glowColorRGB)>(highlightSettingsPtr + OFF_HIGHLIGHT_TYPE_SIZE * settingIndex + 4, glowColorRGB);
				aimxpert::write<int>(Target->BasePointer + OFF_GLOW_FIX, 0);
			}
		}
		else if (!GlowEnabled) { // Disable Glow
			std::array<unsigned char, 4> highlightFunctionBits = {
				0, // InsideFunction
				0, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
				0, // OutlineRadius: size * 255 / 8
				0  // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
			};
			std::array<float, 3> glowColorRGB = { 0, 0, 0 };
			aimxpert::write<unsigned char>(Target->BasePointer + OFF_GLOW_HIGHLIGHT_ID + 0, 0);
			long highlightSettingsPtr = aimxpert::read<long>(OFF_REGION + OFF_GLOW_HIGHLIGHTS);
			if (!isSameTeam) {
				aimxpert::write<decltype(highlightFunctionBits)>(highlightSettingsPtr + OFF_HIGHLIGHT_TYPE_SIZE * 0 + 0, highlightFunctionBits);
				aimxpert::write<decltype(glowColorRGB)>(highlightSettingsPtr + OFF_HIGHLIGHT_TYPE_SIZE * 0 + 4, glowColorRGB);
				aimxpert::write<int>(Target->BasePointer + OFF_GLOW_FIX, 0);
			}
		}
	}

	void Update() {
		if (!Map->IsPlayable)
			return;

		const ULONG64 HighlightSettingsPointer = aimxpert::read<ULONG64>(OFF_REGION + OFF_GLOW_HIGHLIGHTS);
		const ULONG64 HighlightSize = OFF_HIGHLIGHT_TYPE_SIZE;
		const GlowMode NewGlowMode = {
			VisualSettings::ItemGlowInsideFunction,                  // Inside Glow
			VisualSettings::ItemGlowOutlineFunction,                 // Outline (Border)
			Settings::Glow::Item::ItemGlowThickness, // Outline Thickness
			127                                      // ItemGlowPostProcessing
		};

		if (Settings::Glow::Item::ItemGlow) {
			for (int i : VisualSettings::ItemHighlightID) {
				SetGlowState(HighlightSettingsPointer, HighlightSize, i, NewGlowMode);
			}
		}

		for (int i = 0; i < Players->size(); i++) {
			Player* Target = Players->at(i);
			if (!Target->IsValid())
				continue;
			if (!Target->IsHostile)
				continue;

			if (Settings::Glow::NewGlow) {
				if (Settings::Glow::GlowColorMode == 0) {
					if (Settings::Glow::GlowColorShieldMode == 0) {
						if (Target->IsVisible && Target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
							setGlowEnable(Target, 1);
							setGlowThroughWall(Target, 1);
							setCustomGlow(Target, true, Target->IsKnocked, false, true);
						}
						if (!Target->IsVisible && Target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
							setGlowEnable(Target, 1);
							setGlowThroughWall(Target, 1);
							setCustomGlow(Target, false, Target->IsKnocked, false, true);
						}
						// If player is out of max distance
						else if (Target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
							setGlowEnable(Target, 0);
							setGlowThroughWall(Target, 0);
							setCustomGlow(Target, false, false, false, false);
						}
						else if (getGlowEnable(Target) == 1 && getGlowThroughWall(Target) == 1) {
							setGlowEnable(Target, 0);
							setGlowThroughWall(Target, 0);
						}
					}

					if (Settings::Glow::GlowColorShieldMode == 1) {
						if (Target->IsVisible && Target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
							setGlowEnable(Target, 1);
							setGlowThroughWall(Target, 1);
							setCustomGlow(Target, true, Target->IsKnocked, false, true);
						}
						if (!Target->IsVisible && !Target->IsKnocked && Target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
							setGlowEnable(Target, 1);
							setGlowThroughWall(Target, 1);
							setCustomGlow(Target, false, Target->IsKnocked, false, true);
						}
						// If player is out of max distance
						else if (Target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
							setGlowEnable(Target, 0);
							setGlowThroughWall(Target, 0);
							setCustomGlow(Target, false, false, false, false);
						}
						else if (getGlowEnable(Target) == 1 && getGlowThroughWall(Target) == 1) {
							setGlowEnable(Target, 0);
							setGlowThroughWall(Target, 0);
						}
					}
				}

				if (Settings::Glow::GlowColorMode == 1) {
					if (Target->IsVisible && Target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
						setGlowEnable(Target, 1);
						setGlowThroughWall(Target, 1);
						setCustomGlow(Target, true, Target->IsKnocked, false, true);
					}
					if (!Target->IsVisible && !Target->IsKnocked && Target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
						setGlowEnable(Target, 1);
						setGlowThroughWall(Target, 1);
						setCustomGlow(Target, false, Target->IsKnocked, false, true);
					}

					// If player is out of max distance
					else if (Target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Glow::GlowMaxDistance)) {
						setGlowEnable(Target, 0);
						setGlowThroughWall(Target, 0);
						setCustomGlow(Target, false, false, false, false);
					}
					else if (getGlowEnable(Target) == 1 && getGlowThroughWall(Target) == 1) {
						setGlowEnable(Target, 0);
						setGlowThroughWall(Target, 0);
					}
				}
			}
		}
	}
	void ViewModelGlow() {
		// Viewmodel Glow
		if (Settings::Glow::ViewModelGlow) {
			// Weapon Glow
			ViewmodelGlowSettings(); // Updates the ID for what glow you want (bottom of this file)
			uint64_t actWeaponID = aimxpert::read<uint64_t>(Myself->BasePointer + OFF_VIEWMODELS) & 0xFFFF;
			uint64_t currentWeapon = aimxpert::read<uint64_t>(OFF_REGION + OFF_ENTITY_LIST + (actWeaponID << 5));

			aimxpert::write<uint8_t>(currentWeapon + OFF_GLOW_HIGHLIGHT_ID, VisualSettings::ViewmodelGlowID);
		}
	}

	void ViewmodelGlowSettings() {
		int viewModelGlowIDs[] = {
		  13, // Cyan outline
		  15, // Light red outline
		  17, // White outline
		  18, // Orange outline
		  24, // Yellow outline
		  27, // Green solid
		  28, // Orange solid
		  29, // Yellow solid
		  25, // Yellow solid pulsing
		  30, // Purple solid
		  31, // Light Blue solid
		  32, // Light Grey solid
		  34, // White solid
		  35, // Cyan solid
		  36, // Hot Pink solid
		  37, // Light Yellow solid
		  38, // Light Orange solid
		  39, // Light Green solid
		  74, // Black solid
		  49 // Chrome
		};

		if (Settings::Glow::ViewModelGlowCombo >= 0 && Settings::Glow::ViewModelGlowCombo <= 19)
			VisualSettings::ViewmodelGlowID = viewModelGlowIDs[Settings::Glow::ViewModelGlowCombo];
	}

	void ItemGlowSettings() {
		int itemGlowInsideFunctionIDs[] = {
		  0, // Clear
		  137, // Light
		  109, // Solid
		  117 // Light to dark fade
		};

		int itemGlowOutlineFunctionIDs[] = {
		  0, // None
		  138, // Light 1
		  102, // Light 2
		  6, // Solid
		  7, // Gold
		  8, // Orange
		  103, // Pulsing
		  107, // Light Red
		  108, // Red
		  110, // Fading
		  120, // Soft
		  131 // Visible only
		};

		if (Settings::Glow::Item::SelectedInsideStyle >= 0 && Settings::Glow::Item::SelectedInsideStyle <= 3)
			VisualSettings::ItemGlowInsideFunction = itemGlowInsideFunctionIDs[Settings::Glow::Item::SelectedInsideStyle];

		if (Settings::Glow::Item::SelectedOutlineStyle >= 0 && Settings::Glow::Item::SelectedOutlineStyle <= 11)
			VisualSettings::ItemGlowOutlineFunction = itemGlowOutlineFunctionIDs[Settings::Glow::Item::SelectedOutlineStyle];
	}
};