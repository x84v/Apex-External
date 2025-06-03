
struct Camera {
    Vector2D ScreenSize = Vector2D(Settings::ScreenWidth, Settings::ScreenHeight);
    Vector2D ScreenCenter = Vector2D(ScreenSize.x / 2, ScreenSize.y / 2);
    bool WorldToScreen(Vector3D WorldPosition, Vector2D& ScreenPosition) const {
        ULONG64 RenderPtr = aimxpert::read<ULONG64>(OFF_REGION + OFF_VIEWRENDER);
        ULONG64 MatrixPtr = aimxpert::read<ULONG64>(RenderPtr + OFF_VIEWMATRIX);
        Vector3D transformed = aimxpert::read<ViewMatrix>(MatrixPtr).Transform(WorldPosition);
        if (transformed.z < 0.001) {
            return false;
        }

        transformed.x *= 1.0 / transformed.z;
        transformed.y *= 1.0 / transformed.z;
        ScreenPosition = Vector2D(ScreenSize.x / 2.0f + transformed.x * (ScreenSize.x / 2.0f), ScreenSize.y / 2.0f - transformed.y * (ScreenSize.y / 2.0f));
        return true;
    }
};