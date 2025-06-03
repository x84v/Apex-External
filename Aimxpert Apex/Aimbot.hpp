#pragma once
#include <iostream>
#include <vector>

void FireWeapon() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    MouseInputEvent(1, &input, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    MouseInputEvent(1, &input, sizeof(INPUT));
    MouseEvent(MOUSEEVENTF_MOVE, 0, 2.85, 0, 0);
}

float Speed = 55;
float Smooth = 20;
float FOV = 12;
struct Aimbot {
    QAngle DesiredAngles = QAngle(0, 0);
    float FinalDistance = 0;
    float FOV = 10;
    Vector3D TargetPosition;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    Player* CurrentTarget = nullptr;
    bool TargetSelected = true;
    QAngle RCSLastPunch;
    std::chrono::milliseconds LastAimTime;

    Aimbot(LocalPlayer* Myself, std::vector<Player*>* GamePlayers) {
        this->Myself = Myself;
        this->Players = GamePlayers;
    }

    void TriggerUpdate() {
        if (!Settings::Triggerbot::Enabled) return;
        if (!KeyState(MouseInputs->getKeyCode(Settings::AimbotBinds::TriggerBind))) {
            return;
        }

        std::thread fireThread(FireWeapon); 
        fireThread.detach();          
    }

    void Update() {

        if (!Settings::Aimbot::Aimbot)
            return;

        if (Settings::Aimbot::SpectatorCheck && Settings::TotalSpectators >= 1)
            return;

        if (Myself->IsZooming)
            FinalDistance = Settings::Aimbot::ZoomDistance;

        if (!Myself->IsCombatReady()) { 
            CurrentTarget = nullptr; 
            return; 
        }

        if (Myself->IsHoldingGrenade) { 
            ReleaseTarget(); 
            return; 
        }

        if (!KeyState(MouseInputs->getKeyCode(Settings::AimbotBinds::AimBind))) {
            ReleaseTarget();
            return;
        }
      
        if (Settings::Aimbot::BindMethod == 0) {
            if (Settings::Aimbot::OnFire && Settings::Aimbot::OnADS && !Myself->IsInAttack && !Myself->IsZooming) {
                ReleaseTarget();
                return;
            }
            if (Settings::Aimbot::OnFire && !Settings::Aimbot::OnADS && !Myself->IsInAttack) {
                ReleaseTarget();
                return;
            }
        }

        Player* Target = CurrentTarget;
        if (!IsValidTarget(Target)) {
            if (TargetSelected) {
                TargetSelected = false;
            }
            Target = FindBestTarget();
            if (!IsValidTarget(Target)) {
                ReleaseTarget();
                return;
            }

            CurrentTarget = Target;
            CurrentTarget->IsLockedOn = true;
            TargetSelected = true;
        }

        if (TargetSelected && CurrentTarget) {
            TriggerUpdate();
            StartAiming();       
            return;
        }
    }

    void StartAiming() {
        if (!GetAngle(CurrentTarget, DesiredAngles))
            return;

        Vector2D DesiredAnglesIncrement = Vector2D(CalculatePitchIncrement(DesiredAngles), CalculateYawIncrement(DesiredAngles));

        float Extra = FinalDistance / CurrentTarget->DistanceToLocalPlayer;
        float TotalSmooth = Smooth + Extra;

        Vector2D punchAnglesDiff = Myself->punchAnglesDiff.Divide(Smooth).Multiply(Speed);
        double nrPitchIncrement = punchAnglesDiff.x;
        double nrYawIncrement = -punchAnglesDiff.y;

        Vector2D aimbotDelta = DesiredAnglesIncrement.Divide(TotalSmooth).Multiply(Speed);
        double aimYawIncrement = aimbotDelta.y * -1;
        double aimPitchIncrement = aimbotDelta.x;

        double totalPitchIncrement = aimPitchIncrement + nrPitchIncrement;
        double totalYawIncrement = aimYawIncrement + nrYawIncrement;

        int totalPitchIncrementInt = RoundHalfEven(AL1AF0(totalPitchIncrement));
        int totalYawIncrementInt = RoundHalfEven(AL1AF0(totalYawIncrement));

        if (totalPitchIncrementInt == 0 && totalYawIncrementInt == 0) return;
         MouseEvent(MOUSEEVENTF_MOVE, totalYawIncrementInt, totalPitchIncrementInt, 0, 0);
        
    }

    bool GetAngle(Player* Target, QAngle& Angle) {
        const QAngle CurrentAngle = QAngle(Myself->viewAngles.x, Myself->viewAngles.y).fixAngle();
        if (!CurrentAngle.isValid())
            return false;

        if (!GetAngleToTarget(Target, Angle))
            return false;

        return true;
    }

    bool GetAngleToTarget(Player* Target, QAngle& Angle) {

        TargetPosition = Target->GetBonePosition(Settings::AimbotHitboxes::Hitbox);
        Vector3D TargetVelocity = Target->AbsoluteVelocity;
        Vector3D CameraPosition = Myself->CameraPosition;
        QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).NormalizeAngles();
        Angle = Resolver::CalculateAngle(CameraPosition, TargetPosition);

        return true;
    }

    bool IsValidTarget(Player* target) {

        if (Settings::Aimbot::TeamCheck) {
            if (Settings::Aimbot::VisCheck) {
                if (target == nullptr ||
                    !target->IsCombatReady() ||
                    !target->IsVisible ||
                    !target->IsHostile ||
                    target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Aimbot::AdvancedMinDistance1) ||
                    target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Aimbot::AdvancedMaxDistance1)) // Ignore the advanced part, works for advanced + simple aimbot
                    return false;
                return true;
            }
            if (!Settings::Aimbot::VisCheck) {
                if (target == nullptr ||
                    !target->IsCombatReady() ||
                    !target->IsHostile ||
                    target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Aimbot::AdvancedMinDistance1) ||
                    target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Aimbot::AdvancedMaxDistance1)) // Ignore the advanced part, works for advanced + simple aimbot
                    return false;
                return true;
            }
        }
        if (!Settings::Aimbot::TeamCheck) {
            if (Settings::Aimbot::VisCheck) {
                if (target == nullptr ||
                    !target->IsCombatReady() ||
                    !target->IsVisible ||
                    target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Aimbot::AdvancedMinDistance1) ||
                    target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Aimbot::AdvancedMaxDistance1)) // Ignore the advanced part, works for advanced + simple aimbot
                    return false;
                return true;
            }
            if (!Settings::Aimbot::VisCheck) {
                if (target == nullptr ||
                    !target->IsCombatReady() ||
                    target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Settings::Aimbot::AdvancedMinDistance1) ||
                    target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Settings::Aimbot::AdvancedMaxDistance1)) // Ignore the advanced part, works for advanced + simple aimbot
                    return false;
                return true;
            }
        }

        return true;
    }

    float CalculatePitchIncrement(QAngle AimbotDesiredAngles) {
        float wayA = AimbotDesiredAngles.x - Myself->ViewAngles.x;
        float wayB = 180 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    float CalculateYawIncrement(QAngle AimbotDesiredAngles) {
        float wayA = AimbotDesiredAngles.y - Myself->ViewAngles.y;
        float wayB = 360 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    double CalculateDistanceFromCrosshair(Vector3D TargetPosition) {
        Vector3D CameraPosition = Myself->CameraPosition;
        QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).NormalizeAngles();

        if (CameraPosition.Distance(TargetPosition) <= 0.0001f)
            return -1;

        QAngle TargetAngle = Resolver::CalculateAngle(CameraPosition, TargetPosition);
        if (!TargetAngle.isValid())
            return -1;

        return CurrentAngle.distanceTo(TargetAngle);
    }

    void ReleaseTarget() {
        if (CurrentTarget != nullptr && CurrentTarget->IsValid())
            CurrentTarget->IsLockedOn = false;

        TargetSelected = false;
        CurrentTarget = nullptr;
    }

    float GetFOVScale() {
        if (Myself->IsValid()) {
            if (Myself->IsZooming) {
                if (Myself->TargetZoomFOV > 1.0 && Myself->TargetZoomFOV < 90.0) {
                    return tanf(DEG2RAD(Myself->TargetZoomFOV) * (0.64285714285));
                }
            }
        }
        return 1.0;
    }

    int GetBestBone(Player* Target) {
        float NearestDistance = 999;
        int NearestBone = 2;
        for (int i = 0; i < 6; i++) {
            HitboxType Bone = static_cast<HitboxType>(i);
            double DistanceFromCrosshair = CalculateDistanceFromCrosshair(Target->GetBonePosition(Bone));
            if (DistanceFromCrosshair < NearestDistance) {
                NearestBone = i;
                NearestDistance = DistanceFromCrosshair;
            }
        }
        return NearestBone;
    }

    Player* FindBestTarget() {
        Player* NearestTarget = nullptr;
        float BestDistance = 9999;
        float BestFOV = (std::min)(FOV, FOV * (GetFOVScale() * Settings::Aimbot::ZoomScale));
        float LastPov = 9999;
        Vector3D CameraPosition = Myself->CameraPosition;
        for (int i = 0; i < Players->size(); i++) {
            Player* p = Players->at(i);
            if (!IsValidTarget(p))
                continue;
            if (p->DistanceToLocalPlayer > Conversion::ToGameUnits(Settings::Aimbot::AdvancedMinDistance1) && p->DistanceToLocalPlayer < Conversion::ToGameUnits(Settings::Aimbot::AdvancedMaxDistance1)) {
                HitboxType BestBone = static_cast<HitboxType>(GetBestBone(p));
                Vector3D TargetPosition = p->GetBonePosition(BestBone);

                float Distance = CameraPosition.Distance(TargetPosition);
                float FOV = CalculateDistanceFromCrosshair(TargetPosition);

                if (FOV > BestFOV)
                    continue;

                if (Settings::Aimbot::Priority == 0 && FOV > LastPov)
                    continue;

                if (Settings::Aimbot::Priority == 1 && Distance > BestDistance)
                    continue;

                if (Settings::Aimbot::Priority == 2 && FOV > LastPov && Distance > BestDistance)
                    continue;

                NearestTarget = p;
                BestDistance = Distance;
                LastPov = FOV;
            }
        }
        return NearestTarget;
    }
};