
class Renderer
{
public:
    static void DrawText(ImDrawList* canvas, const Vector2D& pos, const char* text, ImColor color, bool outline, bool centered, bool adjustHeight) {
        const auto textColor = color;
        const auto outlineColor = ImColor(0, 0, 0);

        const auto textSize = ImGui::CalcTextSize(text);
        const auto horizontalOffset = centered ? textSize.x / 2 : 0.0f;
        const auto verticalOffset = adjustHeight ? textSize.y : 0.0f;

        if (outline) {
            canvas->AddText({ pos.x - horizontalOffset, pos.y - verticalOffset - 1 }, outlineColor, text);
            canvas->AddText({ pos.x - horizontalOffset, pos.y - verticalOffset + 1 }, outlineColor, text);
            canvas->AddText({ pos.x - horizontalOffset - 1, pos.y - verticalOffset }, outlineColor, text);
            canvas->AddText({ pos.x - horizontalOffset + 1, pos.y - verticalOffset }, outlineColor, text);
        }

        canvas->AddText({ pos.x - horizontalOffset, pos.y - verticalOffset }, textColor, text);
    }

    static void DrawCircle(ImDrawList* canvas, const Vector2D& pos, float radius, int segments, const ImColor& color, float thickness) {
        canvas->AddCircle(ImVec2(pos.x, pos.y), radius, color, segments, thickness);
    }

    static void DrawCircleFilled(ImDrawList* canvas, const Vector2D& pos, float radius, int segments, const ImColor& color) {
        canvas->AddCircleFilled(ImVec2(pos.x, pos.y), radius, color, segments);
    }

    static void DrawLine(ImDrawList* canvas, const Vector2D& start, const Vector2D& end, float thickness, const ImColor& color) {
        canvas->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color), thickness);
    }

    static void DrawQuadFilled(ImDrawList* canvas, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImColor color) {
        canvas->AddQuadFilled(p1, p2, p3, p4, color);
    }

    static void DrawHexagon(ImDrawList* canvas, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p5, const ImVec2& p6, ImU32 col, float thickness) {
        ImVec2 points[6] = { p1, p2, p3, p4, p5, p6 };
        canvas->AddPolyline(points, 6, col, true, thickness);
    }

    static void DrawHexagonFilled(ImDrawList* canvas, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p5, const ImVec2& p6, ImU32 col) {
        ImVec2 points[6] = { p1, p2, p3, p4, p5, p6 };
        canvas->AddConvexPolyFilled(points, 6, col);
    }

    static void DrawCorneredBox(ImDrawList* canvas, float X, float Y, float W, float H, ImColor color, float thickness) {
        float lineW = (W / 4);
        float lineH = (H / 5.5);
        float lineT = -thickness;
        // Corners
        DrawLine(canvas, Vector2D(X, Y + thickness / 2), Vector2D(X, Y + lineH), thickness, color); // bot right vert
        DrawLine(canvas, Vector2D(X + thickness / 2, Y), Vector2D(X + lineW, Y), thickness, color);
        DrawLine(canvas, Vector2D(X + W - lineW, Y), Vector2D(X + W - thickness / 2, Y), thickness, color); // bot left hor
        DrawLine(canvas, Vector2D(X + W, Y + thickness / 2), Vector2D(X + W, Y + lineH), thickness, color);
        DrawLine(canvas, Vector2D(X, Y + H - lineH), Vector2D(X, Y + H - (thickness / 2)), thickness, color); // top right vert
        DrawLine(canvas, Vector2D(X + thickness / 2, Y + H), Vector2D(X + lineW, Y + H), thickness, color);
        DrawLine(canvas, Vector2D(X + W - lineW, Y + H), Vector2D(X + W - thickness / 2, Y + H), thickness, color); // top left hor
        DrawLine(canvas, Vector2D(X + W, Y + H - lineH), Vector2D(X + W, Y + H - (thickness / 2)), thickness, color);
    }

    static void Draw2DBox(ImDrawList* canvas, int Type, int Style, bool Outline, Vector2D& foot, const Vector2D& head, const ImColor& color2D, const ImColor& Filledcolor, float thickness) {
        if (Type == 0) { // 2D Box
            if (Style == 0) {
                float height = head.y - foot.y;
                float width = height / 2.0f;
                if (Outline) {
                    canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 2.5f);
                }
                canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), color2D, 0.0f, 0, thickness);
            }
            if (Style == 1) {
                float Height = (head.y - foot.y);
                Vector2D rectTop = Vector2D(head.x - Height / 3, head.y);
                Vector2D rectBottom = Vector2D(foot.x + Height / 3, foot.y);
                if (Outline) {
                    canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 2.5f);
                }
                canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), color2D, 0.0f, 0, thickness);
            }
        }
        if (Type == 1) { 
            if (Style == 0) {
                float height = head.y - foot.y;
                float width = height / 2.0f;
                if (Outline) {
                    canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 1);
                }
                canvas->AddRect(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), color2D, 0.0f, 0, thickness);
                canvas->AddRectFilled(ImVec2(foot.x - (width / 2), foot.y), ImVec2(head.x + (width / 2), head.y + (height * 0.2)), Filledcolor, 0.0f, 0);
            }
            if (Style == 1) {
                float Height = (head.y - foot.y);
                Vector2D rectTop = Vector2D(head.x - Height / 3, head.y);
                Vector2D rectBottom = Vector2D(foot.x + Height / 3, foot.y);
                if (Outline) {
                    canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), ImColor(0, 0, 0), 0.0f, 0, thickness + 1);
                }
                canvas->AddRect(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), color2D, 0.0f, 0, thickness);
                canvas->AddRectFilled(ImVec2(rectBottom.x, rectBottom.y), ImVec2(rectTop.x, rectTop.y + (Height * 0.2)), Filledcolor, thickness);
            }
        }
        if (Type == 2) { // 2D Corners
            float height = head.y - foot.y;
            float width = height / 2.0f;
            float x = foot.x - (width / 2.f);

            if (Outline) {
                Renderer::DrawCorneredBox(canvas, x, foot.y, width, height + (height * 0.2), ImColor(0, 0, 0), thickness + 1);
            }
            Renderer::DrawCorneredBox(canvas, x, foot.y, width, height + (height * 0.2), color2D, thickness);
        }
    }

    static void DrawRectFilled(ImDrawList* canvas, float x, float y, float x2, float y2, ImColor color, float rounding, int rounding_corners_flags) {
        canvas->AddRectFilled(ImVec2(x, y), ImVec2(x2, y2), color, rounding, rounding_corners_flags);
    }

    static void DrawProgressBar(ImDrawList* canvas, float x, float y, float w, float h, int value, int v_max, ImColor barColor) {
        DrawRectFilled(canvas, x, y, x + w, y - ((h / float(v_max)) * (float)value), barColor, 0.0f, 0);
        canvas->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y - h + 1), ImColor(0, 0, 0), 0, 1);
    }

    static void Draw2DBarTest(ImDrawList* canvas, bool DrawHealth, bool DrawShield, int BarStyle, int ColorMode, Vector2D& Foot, Vector2D& Head, int health, int maxHealth, int shield, int maxShield, float thickness, float thickness2, float BarWidth, float BarHeight) {
        // Shield Color
        ImColor shieldBarColor;
        if (ColorMode == 0) { // MaxShield
            if (maxShield == 50) { // white
                shieldBarColor = ImColor(168, 168, 168, 255);
            }
            else if (maxShield == 75) { // blue
                shieldBarColor = ImColor(39, 178, 255, 255);
            }
            else if (maxShield == 100) { // purple
                shieldBarColor = ImColor(206, 59, 255, 255);
            }
            else if (maxShield == 125) { // red
                shieldBarColor = ImColor(219, 2, 2, 255);
            }
        }

        if (ColorMode == 1) { // Current Shield
            if (shield <= 50) { // white
                shieldBarColor = ImColor(168, 168, 168, 255);
            }
            else if (shield <= 75) { // blue
                shieldBarColor = ImColor(39, 178, 255, 255);
            }
            else if (shield <= 100) { // purple
                shieldBarColor = ImColor(206, 59, 255, 255);
            }
            else if (shield <= 125) { // red
                shieldBarColor = ImColor(219, 2, 2, 255);
            }
        }

        if (BarStyle == 0) { // Sides
            float height = Head.y - Foot.y;
            float width = height / 2.f;
            float width2 = width / 10;
            if (width2 < 2.f)
                width2 = 2.;
            if (width2 > 3)
                width2 = 3.;

            float entityHeight = Foot.y - Head.y;
            float boxLeft = Foot.x - entityHeight / 3;
            float boxRight = Head.x + entityHeight / 3;
            float barPercentWidth = thickness2;
            float barPixelWidth = barPercentWidth * (boxRight - boxLeft);
            float barHeight = entityHeight * (health / 100.0f);
            Vector2D barTop = Vector2D(boxLeft - barPixelWidth, Foot.y - barHeight);
            Vector2D barBottom = Vector2D(boxLeft, Foot.y);

            if (DrawHealth)
                Renderer::DrawProgressBar(canvas, barBottom.x - 3.5f, barBottom.y, width2, barBottom.y - Head.y + (entityHeight * 0.2), health, 100, ImColor(0, 255, 0));
            if (DrawShield)
                Renderer::DrawProgressBar(canvas, barBottom.x - 7.0f, barBottom.y, width2, barBottom.y - Head.y + (entityHeight * 0.2), shield, maxShield, shieldBarColor);
        }
    }

    // Draw 2D Bars
    static void Draw2DBar(ImDrawList* canvas, int BarMode, int BarStyle, int ColorMode, Vector2D& Foot, Vector2D& Head, /*Vector2D& AboveHead, */int health, int maxHealth, int shield, int maxShield, float thickness, float thickness2, float BarWidth, float BarHeight) {
        // Pre-Stuff
        // Shield Color
        ImColor shieldBarColor;
        if (ColorMode == 0) { // MaxShield
            if (maxShield == 50) { // white
                shieldBarColor = ImColor(168, 168, 168, 255);
            }
            else if (maxShield == 75) { // blue
                shieldBarColor = ImColor(39, 178, 255, 255);
            }
            else if (maxShield == 100) { // purple
                shieldBarColor = ImColor(206, 59, 255, 255);
            }
            else if (maxShield == 125) { // red
                shieldBarColor = ImColor(219, 2, 2, 255);
            }
        }

        if (ColorMode == 1) { // Current Shield
            if (shield <= 50) { // white
                shieldBarColor = ImColor(168, 168, 168, 255);
            }
            else if (shield <= 75) { // blue
                shieldBarColor = ImColor(39, 178, 255, 255);
            }
            else if (shield <= 100) { // purple
                shieldBarColor = ImColor(206, 59, 255, 255);
            }
            else if (shield <= 125) { // red
                shieldBarColor = ImColor(219, 2, 2, 255);
            }
        }

        // Bars
        if (BarMode == 0) { // Health Only
            if (BarStyle == 0) { // Side Bar
                float height = Head.y - Foot.y;
                float width = height / 2.f;
                float width2 = width / 10;
                if (width2 < 2.f)
                    width2 = 2.;
                if (width2 > 3)
                    width2 = 3.;

                float entityHeight = Foot.y - Head.y;
                float boxLeft = Foot.x - entityHeight / 3;
                float boxRight = Head.x + entityHeight / 3;
                float barPercentWidth = thickness2;
                float barPixelWidth = barPercentWidth * (boxRight - boxLeft);
                float barHeight = entityHeight * (health / 100.0f);
                Vector2D barTop = Vector2D(boxLeft - barPixelWidth, Foot.y - barHeight);
                Vector2D barBottom = Vector2D(boxLeft, Foot.y);

                Renderer::DrawProgressBar(canvas, barBottom.x - 3.5f /* + (width / 2) - width2*/, barBottom.y, width2, barBottom.y - Head.y + (entityHeight * 0.2), health, 100, ImColor(0, 255, 0));
            }
        }
        if (BarMode == 1) { // Shield Only
            if (BarStyle == 0) { // Side Bar
                float height = Head.y - Foot.y;
                float width = height / 2.f;
                float width2 = width / 10;
                if (width2 < 2.f)
                    width2 = 2.;
                if (width2 > 3)
                    width2 = 3.;

                float entityHeight = Foot.y - Head.y;
                float boxLeft = Foot.x - entityHeight / 3;
                float boxRight = Head.x + entityHeight / 3;
                float barPercentWidth = thickness2;
                float barPixelWidth = barPercentWidth * (boxRight - boxLeft);
                float barHeight = entityHeight * (health / 100.0f);
                Vector2D barTop = Vector2D(boxLeft - barPixelWidth, Foot.y - barHeight);
                Vector2D barBottom = Vector2D(boxLeft, Foot.y);

                Renderer::DrawProgressBar(canvas, barBottom.x - 3.5f /* + (width / 2) - width2*/, barBottom.y, width2, barBottom.y - Head.y + (entityHeight * 0.2), shield, maxShield, shieldBarColor);
            }
        }

        if (BarMode == 2) { // Health & Shield
            if (BarStyle == 0) { // Side Bar
                float height = Head.y - Foot.y;
                float width = height / 2.f;
                float width2 = width / 10;
                if (width2 < 2.f)
                    width2 = 2.;
                if (width2 > 3)
                    width2 = 3.;

                float entityHeight = Foot.y - Head.y;
                float boxLeft = Foot.x - entityHeight / 3;
                float boxRight = Head.x + entityHeight / 3;
                float barPercentWidth = thickness2;
                float barPixelWidth = barPercentWidth * (boxRight - boxLeft);
                float barHeight = entityHeight * (health / 100.0f);
                Vector2D barTop = Vector2D(boxLeft - barPixelWidth, Foot.y - barHeight);
                Vector2D barBottom = Vector2D(boxLeft, Foot.y);

                Renderer::DrawProgressBar(canvas, barBottom.x - 3.5f /* + (width / 2) - width2*/, barBottom.y, width2, barBottom.y - Head.y + (entityHeight * 0.2), health, 100, ImColor(0, 255, 0));
                Renderer::DrawProgressBar(canvas, barBottom.x - 8.5f /* + (width / 2) - width2*/, barBottom.y, width2, barBottom.y - Head.y + (entityHeight * 0.2), shield, maxShield, shieldBarColor);
            }
        }
    }

    static void DrawSeer(ImDrawList* Canvas, float x, float y, int shield, int max_shield, int health) {
        int bg_offset = 3;
        int bar_width = 158;
        float max_health = 100.0f;
        float shield_step = 25.0f;
        ImVec2 bg1(x - bar_width / 2 - bg_offset, y);
        ImVec2 bg2(bg1.x - 10, bg1.y - 16);
        ImVec2 bg3(bg2.x + 5, bg2.y - 7);
        ImVec2 bg4(bg3.x + bar_width + bg_offset, bg3.y);
        ImVec2 bg5(bg4.x + 11, bg4.y + 18);
        ImVec2 bg6(x + bar_width / 2 + bg_offset, y);
        Renderer::DrawHexagonFilled(Canvas, bg1, bg2, bg3, bg4, bg5, bg6, ImColor(0, 0, 0, 120));

        ImVec2 h1(bg1.x + 3, bg1.y - 4);
        ImVec2 h2(h1.x - 5, h1.y - 8);
        ImVec2 h3(h2.x + (float)health / max_health * bar_width, h2.y);
        ImVec2 h4(h1.x + (float)health / max_health * bar_width, h1.y);
        ImVec2 h3m(h2.x + bar_width, h2.y);
        ImVec2 h4m(h1.x + bar_width, h1.y);
        Renderer::DrawQuadFilled(Canvas, h1, h2, h3m, h4m, ImColor(10, 10, 30, 60));
        Renderer::DrawQuadFilled(Canvas, h1, h2, h3, h4, ImColor(255, 255, 255, 255));

        // Shield
        ImColor shieldCracked(97, 97, 97);
        ImColor shieldCrackedDark(67, 67, 67);

        ImColor shieldCol;
        ImColor shieldColDark;
        if (max_shield == 50) { // white
            shieldCol = ImColor(247, 247, 247);
            shieldColDark = ImColor(164, 164, 164);
        }
        else if (max_shield == 75) { // blue
            shieldCol = ImColor(39, 178, 255);
            shieldColDark = ImColor(27, 120, 210);
        }
        else if (max_shield == 100) { // purple
            shieldCol = ImColor(206, 59, 255);
            shieldColDark = ImColor(136, 36, 220);
        }
        else if (max_shield == 125) { // red
            shieldCol = ImColor(219, 2, 2);
            shieldColDark = ImColor(219, 2, 2);
        }
        else {
            shieldCol = ImColor(247, 247, 247);
            shieldColDark = ImColor(164, 164, 164);
        }

        int shield_25 = 30;
        int shield_tmp = shield;
        int shield1 = 0;
        int shield2 = 0;
        int shield3 = 0;
        int shield4 = 0;
        int shield5 = 0;
        if (shield_tmp > 25) {
            shield1 = 25;
            shield_tmp -= 25;
            if (shield_tmp > 25) {
                shield2 = 25;
                shield_tmp -= 25;
                if (shield_tmp > 25) {
                    shield3 = 25;
                    shield_tmp -= 25;
                    if (shield_tmp > 25) {
                        shield4 = 25;
                        shield_tmp -= 25;
                        shield5 = shield_tmp;
                    }
                    else {
                        shield4 = shield_tmp;
                    }
                }
                else {
                    shield3 = shield_tmp;
                }
            }
            else {
                shield2 = shield_tmp;
            }
        }
        else {
            shield1 = shield_tmp;
        }

        ImVec2 s1(h2.x - 1, h2.y - 2);
        ImVec2 s2(s1.x - 3, s1.y - 5);
        ImVec2 s3(s2.x + shield1 / shield_step * shield_25, s2.y);
        ImVec2 s4(s1.x + shield1 / shield_step * shield_25, s1.y);
        ImVec2 s3m(s2.x + shield_25, s2.y);
        ImVec2 s4m(s1.x + shield_25, s1.y);

        ImVec2 ss1(s4m.x + 2, s1.y);
        ImVec2 ss2(s3m.x + 2, s2.y);
        ImVec2 ss3(ss2.x + shield2 / shield_step * shield_25, s2.y);
        ImVec2 ss4(ss1.x + shield2 / shield_step * shield_25, s1.y);
        ImVec2 ss3m(ss2.x + shield_25, s2.y);
        ImVec2 ss4m(ss1.x + shield_25, s1.y);

        ImVec2 sss1(ss4m.x + 2, s1.y);
        ImVec2 sss2(ss3m.x + 2, s2.y);
        ImVec2 sss3(sss2.x + shield3 / shield_step * shield_25, s2.y);
        ImVec2 sss4(sss1.x + shield3 / shield_step * shield_25, s1.y);
        ImVec2 sss3m(sss2.x + shield_25, s2.y);
        ImVec2 sss4m(sss1.x + shield_25, s1.y);

        ImVec2 ssss1(sss4m.x + 2, s1.y);
        ImVec2 ssss2(sss3m.x + 2, s2.y);
        ImVec2 ssss3(ssss2.x + shield4 / shield_step * shield_25, s2.y);
        ImVec2 ssss4(ssss1.x + shield4 / shield_step * shield_25, s1.y);
        ImVec2 ssss3m(ssss2.x + shield_25, s2.y);
        ImVec2 ssss4m(ssss1.x + shield_25, s1.y);

        ImVec2 sssss1(ssss4m.x + 2, s1.y);
        ImVec2 sssss2(ssss3m.x + 2, s2.y);
        ImVec2 sssss3(sssss2.x + shield5 / shield_step * shield_25, s2.y);
        ImVec2 sssss4(sssss1.x + shield5 / shield_step * shield_25, s1.y);
        ImVec2 sssss3m(sssss2.x + shield_25, s2.y);
        ImVec2 sssss4m(sssss1.x + shield_25, s1.y);
        if (max_shield == 50) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield != 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
        }
        else if (max_shield == 75) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield < 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
            else if (shield <= 75) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                if (shield < 75) {
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
            }
        }
        else if (max_shield == 100) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield < 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
            else if (shield <= 75) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                if (shield < 75) {
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
            }
            else if (shield <= 100) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
                if (shield < 100) {
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3, ssss4, shieldCol);
            }
        }
        else if (max_shield == 125) {
            if (shield <= 25) {
                if (shield < 25) {
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3m, s4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
            }
            else if (shield <= 50) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                if (shield < 50) {
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3m, ss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
            }
            else if (shield <= 75) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                if (shield < 75) {
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3m, sss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
            }
            else if (shield <= 100) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
                if (shield < 100) {
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3, ssss4, shieldCol);
            }
            else if (shield <= 125) {
                Renderer::DrawQuadFilled(Canvas, s1, s2, s3, s4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ss1, ss2, ss3, ss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, sss1, sss2, sss3, sss4, shieldCol);
                Renderer::DrawQuadFilled(Canvas, ssss1, ssss2, ssss3, ssss4, shieldCol);
                if (shield < 125) {
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
                }
                if (shield != 0)
                    Renderer::DrawQuadFilled(Canvas, sssss1, sssss2, sssss3, sssss4, shieldCol);
            }
        }
    }

    static Vector3D RotatePoint(Vector3D EntityPos, Vector3D LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck) {
        float r_1, r_2;
        float x_1, y_1;

        r_1 = -(EntityPos.y - LocalPlayerPos.y);
        r_2 = EntityPos.x - LocalPlayerPos.x;

        float yawToRadian = angle * (float)(M_PI / 180.0F);
        x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
        y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

        *viewCheck = y_1 < 0;

        x_1 *= zoom;
        y_1 *= zoom;

        int sizX = sizeX / 2;
        int sizY = sizeY / 2;

        x_1 += sizX;
        y_1 += sizY;

        if (x_1 < 5)
            x_1 = 5;

        if (x_1 > sizeX - 5)
            x_1 = sizeX - 5;

        if (y_1 < 5)
            y_1 = 5;

        if (y_1 > sizeY - 5)
            y_1 = sizeY - 5;

        x_1 += posX;
        y_1 += posY;

        return Vector3D(x_1, y_1, 0);
    }

    static void TeamMiniMap_Circle(int x, int y, int radius, int teamID, float targetyaw, int dotSize, int outlineSize) {
        ImColor teamColors[60] = {
            ImColor(255, 182, 193, 242), // Light Pink
            ImColor(255, 105, 180, 242), // Hot Pink
            ImColor(255, 192, 203, 242), // Pink
            ImColor(255, 20, 147, 242),  // Deep Pink
            ImColor(255, 160, 122, 242), // Light Salmon
            ImColor(255, 127, 80, 242),  // Coral
            ImColor(255, 99, 71, 242),   // Tomato
            ImColor(255, 69, 0, 242),    // Orange Red
            ImColor(255, 140, 0, 242),   // Dark Orange
            ImColor(255, 165, 0, 242),   // Orange
            ImColor(255, 215, 0, 242),   // Gold
            ImColor(255, 255, 0, 242),   // Yellow
            ImColor(240, 230, 140, 242), // Khaki
            ImColor(238, 232, 170, 242), // Pale Goldenrod
            ImColor(189, 183, 107, 242), // Dark Khaki
            ImColor(173, 255, 47, 242),  // Green Yellow
            ImColor(127, 255, 0, 242),   // Chartreuse
            ImColor(124, 252, 0, 242),   // Lawn Green
            ImColor(0, 255, 0, 242),     // Lime
            ImColor(50, 205, 50, 242),   // Lime Green
            ImColor(144, 238, 144, 242), // Light Green
            ImColor(152, 251, 152, 242), // Pale Green
            ImColor(0, 255, 127, 242),   // Spring Green
            ImColor(60, 179, 113, 242),  // Medium Sea Green
            ImColor(32, 178, 170, 242),  // Light Sea Green
            ImColor(0, 255, 255, 242),   // Cyan
            ImColor(0, 206, 209, 242),   // Dark Turquoise
            ImColor(64, 224, 208, 242),  // Turquoise
            ImColor(72, 209, 204, 242),  // Medium Turquoise
            ImColor(175, 238, 238, 242), // Pale Turquoise
            ImColor(127, 255, 212, 242), // Aquamarine
            ImColor(176, 224, 230, 242), // Powder Blue
            ImColor(173, 216, 230, 242), // Light Blue
            ImColor(135, 206, 235, 242), // Sky Blue
            ImColor(135, 206, 250, 242), // Light Sky Blue
            ImColor(0, 191, 255, 242),   // Deep Sky Blue
            ImColor(30, 144, 255, 242),  // Dodger Blue
            ImColor(70, 130, 180, 242),  // Steel Blue
            ImColor(100, 149, 237, 242), // Cornflower Blue
            ImColor(65, 105, 225, 242),  // Royal Blue
            ImColor(138, 43, 226, 242),  // Blue Violet
            ImColor(75, 0, 130, 242),    // Indigo
            ImColor(153, 50, 204, 242),  // Dark Orchid
            ImColor(186, 85, 211, 242),  // Medium Orchid
            ImColor(221, 160, 221, 242), // Plum
            ImColor(238, 130, 238, 242), // Violet
            ImColor(218, 112, 214, 242), // Orchid
            ImColor(255, 0, 255, 242),   // Magenta
            ImColor(255, 20, 147, 242),  // Deep Pink
            ImColor(219, 112, 147, 242), // Pale Violet Red
            ImColor(255, 105, 180, 242), // Hot Pink
            ImColor(255, 182, 193, 242), // Light Pink
            ImColor(250, 128, 114, 242), // Salmon
            ImColor(233, 150, 122, 242), // Dark Salmon
            ImColor(240, 128, 128, 242)  // Light Coral
        };
        auto colOutline = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0, 0.0, 0.0, 1.0));
        ImVec2 center(x, y);
        if (teamID == 97)
            ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, ImColor(255, 55, 0));
        else
            ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, teamColors[teamID]);
        ImGui::GetWindowDrawList()->AddCircle(center, outlineSize, colOutline, 12, radius);
    }

    static void TeamMiniMap_ViewAngles(int x, int y, int radius, float targetyaw, float lineLength, const ImColor& lineColor) {
        ImVec2 center(x, y);
        const int numPlayers = 3;
        for (int i = 0; i < numPlayers; i++) {
            float angle = (360.0 - targetyaw) * (M_PI / 180.0);
            ImVec2 endpoint(center.x + radius * cos(angle) + lineLength, center.y + radius * sin(angle) + lineLength);
            ImGui::GetWindowDrawList()->AddLine(center, endpoint, lineColor);
        }
    }

    static void TeamMiniMap_Arrow(int x, int y, int size, int teamID, float targetyaw) {
        ImColor teamColors[60] = {
            ImColor(255, 182, 193, 242), // Light Pink
            ImColor(255, 105, 180, 242), // Hot Pink
            ImColor(255, 192, 203, 242), // Pink
            ImColor(255, 20, 147, 242),  // Deep Pink
            ImColor(255, 160, 122, 242), // Light Salmon
            ImColor(255, 127, 80, 242),  // Coral
            ImColor(255, 99, 71, 242),   // Tomato
            ImColor(255, 69, 0, 242),    // Orange Red
            ImColor(255, 140, 0, 242),   // Dark Orange
            ImColor(255, 165, 0, 242),   // Orange
            ImColor(255, 215, 0, 242),   // Gold
            ImColor(255, 255, 0, 242),   // Yellow
            ImColor(240, 230, 140, 242), // Khaki
            ImColor(238, 232, 170, 242), // Pale Goldenrod
            ImColor(189, 183, 107, 242), // Dark Khaki
            ImColor(173, 255, 47, 242),  // Green Yellow
            ImColor(127, 255, 0, 242),   // Chartreuse
            ImColor(124, 252, 0, 242),   // Lawn Green
            ImColor(0, 255, 0, 242),     // Lime
            ImColor(50, 205, 50, 242),   // Lime Green
            ImColor(144, 238, 144, 242), // Light Green
            ImColor(152, 251, 152, 242), // Pale Green
            ImColor(0, 255, 127, 242),   // Spring Green
            ImColor(60, 179, 113, 242),  // Medium Sea Green
            ImColor(32, 178, 170, 242),  // Light Sea Green
            ImColor(0, 255, 255, 242),   // Cyan
            ImColor(0, 206, 209, 242),   // Dark Turquoise
            ImColor(64, 224, 208, 242),  // Turquoise
            ImColor(72, 209, 204, 242),  // Medium Turquoise
            ImColor(175, 238, 238, 242), // Pale Turquoise
            ImColor(127, 255, 212, 242), // Aquamarine
            ImColor(176, 224, 230, 242), // Powder Blue
            ImColor(173, 216, 230, 242), // Light Blue
            ImColor(135, 206, 235, 242), // Sky Blue
            ImColor(135, 206, 250, 242), // Light Sky Blue
            ImColor(0, 191, 255, 242),   // Deep Sky Blue
            ImColor(30, 144, 255, 242),  // Dodger Blue
            ImColor(70, 130, 180, 242),  // Steel Blue
            ImColor(100, 149, 237, 242), // Cornflower Blue
            ImColor(65, 105, 225, 242),  // Royal Blue
            ImColor(138, 43, 226, 242),  // Blue Violet
            ImColor(75, 0, 130, 242),    // Indigo
            ImColor(153, 50, 204, 242),  // Dark Orchid
            ImColor(186, 85, 211, 242),  // Medium Orchid
            ImColor(221, 160, 221, 242), // Plum
            ImColor(238, 130, 238, 242), // Violet
            ImColor(218, 112, 214, 242), // Orchid
            ImColor(255, 0, 255, 242),   // Magenta
            ImColor(255, 20, 147, 242),  // Deep Pink
            ImColor(219, 112, 147, 242), // Pale Violet Red
            ImColor(255, 105, 180, 242), // Hot Pink
            ImColor(255, 182, 193, 242), // Light Pink
            ImColor(250, 128, 114, 242), // Salmon
            ImColor(233, 150, 122, 242), // Dark Salmon
            ImColor(240, 128, 128, 242)  // Light Coral
        };

        ImVec2 center(x, y);
        ImVec2 points[4];

        float angle0 = ((360.0 - targetyaw)) * (M_PI / 180.0);
        float angle1 = ((360.0 - targetyaw + 135)) * (M_PI / 180.0);
        float angle2 = ((360.0 - targetyaw + 180)) * (M_PI / 180.0);
        float angle3 = ((360.0 - targetyaw - 135)) * (M_PI / 180.0);

        points[0] = ImVec2(center.x + size * cos(angle0), center.y + size * sin(angle0));
        points[1] = ImVec2(center.x + size * cos(angle1), center.y + size * sin(angle1));
        points[2] = ImVec2(center.x + size / 4 * cos(angle2), center.y + size / 4 * sin(angle2));
        points[3] = ImVec2(center.x + size * cos(angle3), center.y + size * sin(angle3));
        if (teamID == 97)
            ImGui::GetWindowDrawList()->AddConvexPolyFilled(points, 4, ImColor(255, 55, 0));
        else
            ImGui::GetWindowDrawList()->AddConvexPolyFilled(points, 4, teamColors[teamID]);
    }
};