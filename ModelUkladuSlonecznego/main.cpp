#include "raylib.h"
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
    const int segments = 150;
    for (int i = 0; i < segments; i++) {
        float a1 = (float)i / segments * 2.0f * PI;
        float a2 = (float)(i + 1) / segments * 2.0f * PI;
        Vector3 p1 = { cosf(a1) * radius, 0.0f, sinf(a1) * radius };
        Vector3 p2 = { cosf(a2) * radius, 0.0f, sinf(a2) * radius };
        DrawLine3D(p1, p2, color);
    }
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Uklad Sloneczny");

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 250.0f, 600.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

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

        DrawSphere({ 0, 0, 0 }, 15.0f, YELLOW);
        DrawSphereWires({ 0, 0, 0 }, 15.1f, 12, 12, ORANGE);

        for (const auto& p : solarSystem) {
            DrawOrbit(p.distance, Fade(p.color, 0.2f));

            float px = cosf(p.angle) * p.distance;
            float pz = sinf(p.angle) * p.distance;
            Vector3 pos = { px, 0.0f, pz };

            DrawSphere(pos, p.radius, p.color);
            DrawSphereWires(pos, p.radius + 0.1f, 8, 8, Fade(BLACK, 0.3f));
        }
        EndMode3D();

        DrawRectangle(10, 10, 350, 110, Fade(SKYBLUE, 0.2f));

        std::string viewName = (focusIndex == -1) ? "Uklad Sloneczny" : "Planeta: " + solarSystem[focusIndex].name;
        DrawText(viewName.c_str(), 20, 45, 22, YELLOW);
        DrawText(TextFormat("Predkosc: x%.1f", timeScale), 20, 75, 18, GREEN);

        DrawFPS(10, 770);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}