#include "raylib.h"
#include "rlgl.h"
#include "stars_data.h"
#include <vector>
#include <cmath>
#include <string>

struct Planet {
    std::string name;
    float distance;
    float radius;
    float orbitSpeed;
    Color color;
    float angle;
};

void DrawOrbit(float radius, Color color) {
    const int segments = 120;
    float lineThickness = 0.5f;

    for (int i = 0; i < segments; i++) {
        float a1 = (float)i / segments * (float)2.0f * PI;
        float a2 = (float)(i + 1) / segments * (float)2.0f * PI;

        Vector3 p1 = { cosf(a1) * radius, 0.0f, sinf(a1) * radius };
        Vector3 p2 = { cosf(a2) * radius, 0.0f, sinf(a2) * radius };

        DrawCylinderEx(p1, p2, lineThickness, lineThickness, 4, color);
    }
}

void DrawSaturnRings(Vector3 position, float radius, Color color) {
    const int segments = 100;
    float innerRadius = radius + 2.0f;
    float outerRadius = radius + 10.0f;

    for (float r = innerRadius; r < outerRadius; r += 0.5f) {
        for (int i = 0; i < segments; i++) {
            float a1 = (float)i / segments * (float)2.0f * PI;
            float a2 = (float)(i + 1) / segments * (float)2.0f * PI;

            Vector3 p1 = { position.x + cosf(a1) * r, position.y, position.z + sinf(a1) * r };
            Vector3 p2 = { position.x + cosf(a2) * r, position.y, position.z + sinf(a2) * r };

            DrawLine3D(p1, p2, Fade(color, 0.4f));
        }
    }
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Uklad Sloneczny");
    ToggleBorderlessWindowed();

    Font fontConsolas = LoadFont("C:\\Windows\\Fonts\\consola.ttf");
    if (fontConsolas.texture.id == 0) fontConsolas = GetFontDefault();

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 250.0f, 600.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Image starsImg = LoadImageFromMemory(".png", stars_png, stars_png_len);
    Texture2D starsTex = LoadTextureFromImage(starsImg);
    UnloadImage(starsImg);

    Mesh skyMesh = GenMeshSphere(1000.0f, 32, 32);
    Model skySphere = LoadModelFromMesh(skyMesh);
    skySphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = starsTex;

    std::vector<Planet> solarSystem = {
        { "Merkury", 40.0f,  2.4f,  47.36f, GRAY, 0.0f },
        { "Wenus",   65.0f,  6.0f,  35.02f, ORANGE, 0.0f },
        { "Ziemia",  95.0f,  6.3f,  29.78f, BLUE, 0.0f },
        { "Mars",    125.0f, 3.3f,  24.00f, RED, 0.0f },
        { "Jowisz",  180.0f, 18.0f, 13.07f, BEIGE, 0.0f },
        { "Saturn",  240.0f, 15.0f, 9.68f,  GOLD, 0.0f },
        { "Uran",    300.0f, 10.0f, 6.81f,  SKYBLUE, 0.0f },
        { "Neptun",  350.0f, 9.8f,  5.43f,  DARKBLUE, 0.0f }
    };

    float timeScale = 0.1f;
    int focusIndex = -1;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_UP)) timeScale += 0.1f;
        if (IsKeyPressed(KEY_DOWN)) {
            timeScale -= 0.1f;
            if (timeScale < 0.1f) timeScale = 0.1f;
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            focusIndex++;
            if (focusIndex > 7) focusIndex = -1;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            focusIndex--;
            if (focusIndex < -1) focusIndex = 7;
        }

        for (auto& p : solarSystem) {
            p.angle += (p.orbitSpeed / p.distance) * timeScale * dt * 20.0f;
        }

        if (focusIndex == -1) {
            camera.target = { 0.0f, 0.0f, 0.0f };
            camera.position = { 0.0f, 250.0f, 600.0f };
        }
        else {
            float x = cosf(solarSystem[focusIndex].angle) * solarSystem[focusIndex].distance;
            float z = sinf(solarSystem[focusIndex].angle) * solarSystem[focusIndex].distance;
            Vector3 planetPos = { x, 0.0f, z };
            camera.target = planetPos;
            camera.position = { planetPos.x + 50.0f, 40.0f, planetPos.z + 50.0f };
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        DrawModel(skySphere, camera.position, 1.0f, WHITE);
        rlEnableDepthMask();
        rlEnableBackfaceCulling();

        // Slonce
        DrawSphere({ 0, 0, 0 }, 15.0f, YELLOW);

        // Planety
        for (const auto& p : solarSystem) {
            DrawOrbit(p.distance, Fade(p.color, 0.2f));
            float px = cosf(p.angle) * p.distance;
            float pz = sinf(p.angle) * p.distance;
            Vector3 pos = { px, 0.0f, pz };

            DrawSphere(pos, p.radius, p.color);

            if (p.name == "Saturn") {
                DrawSaturnRings(pos, p.radius, p.color);
            }
        }

        EndMode3D();

        // Napisy nad planetami
        for (const auto& p : solarSystem) {
            float px = cosf(p.angle) * p.distance;
            float pz = sinf(p.angle) * p.distance;

            Vector3 worldPos = { px, p.radius + 5.0f, pz };
            Vector2 screenPos = GetWorldToScreen(worldPos, camera);

            if (screenPos.x > 0 && screenPos.x < screenWidth && screenPos.y > 0 && screenPos.y < screenHeight) {
                int fontSize = 16;
                int padding = 4;
                int textWidth = MeasureText(p.name.c_str(), fontSize);

                DrawRectangle(
                    (int)screenPos.x - textWidth / 2 - padding,
                    (int)screenPos.y - fontSize / 2 - padding,
                    textWidth + padding * 2,
                    fontSize + padding * 2,
                    Fade(BLACK, 0.8f)
                );

                DrawTextEx(fontConsolas, p.name.c_str(), { (float)screenPos.x - textWidth / 2, (float)screenPos.y - fontSize / 2 }, (float)fontSize, 1.0f, WHITE);
            }
        }

        // UI
        DrawRectangle(10, 10, 350, 110, Fade(SKYBLUE, 0.2f));
        std::string viewName = (focusIndex == -1) ? "Uklad Sloneczny" : "Planeta: " + solarSystem[focusIndex].name;
        DrawTextEx(fontConsolas, viewName.c_str(), { 20, 30 }, 30.0f, 1.0f, YELLOW);
        DrawTextEx(fontConsolas, TextFormat("Predkosc: x%.1f", timeScale), { 20, 75 }, 25, 1.0f, GREEN);
        DrawFPS(10, 770);
        EndDrawing();
    }

    UnloadTexture(starsTex);
    UnloadModel(skySphere);
    CloseWindow();
    return 0;
}