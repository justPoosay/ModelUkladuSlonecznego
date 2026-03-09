#include "raylib.h"
#include <vector>
#include <cmath>

struct Planet {
    float distance;
    float radius;
    float orbitSpeed;
    Color color;
    float angle;
};

void DrawOrbit(float radius, Color color) {
    const int segments = 120;
    for (int i = 0; i < segments; i++) {
        float a1 = (float)i / segments * 2.0f * PI;
        float a2 = (float)(i + 1) / segments * 2.0f * PI;

        Vector3 p1 = { cosf(a1) * radius, 0, sinf(a1) * radius };
        Vector3 p2 = { cosf(a2) * radius, 0, sinf(a2) * radius };

        DrawLine3D(p1, p2, color);
    }
}

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Uklad Sloneczny");

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 280.0f, 0.1f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    std::vector<Planet> solarSystem = {
        { 30.0f,  1.5f, 1.2f, GRAY }, // Merkury
        { 50.0f,  2.8f, 0.9f, ORANGE }, // Wenus
        { 75.0f,  3.0f, 0.7f, BLUE }, // Ziemia
        { 100.0f, 2.2f, 0.5f, RED }, // Mars
        { 140.0f, 7.0f, 0.3f, BEIGE }, // Jowisz
        { 180.0f, 6.0f, 0.25f, GOLD }, // Saturn
        { 220.0f, 4.5f, 0.15f, SKYBLUE }, // Uran
        { 260.0f, 4.4f, 0.1f, DARKBLUE} // Neptun
    };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        for (auto& p : solarSystem) {
            p.angle += p.orbitSpeed * dt;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawSphere({ 0, 0, 0 }, 12.0f, YELLOW);
        DrawSphereWires({ 0, 0, 0 }, 12.2f, 16, 16, GOLD);

        for (const auto& p : solarSystem) {
            DrawOrbit(p.distance, Fade(GRAY, 0.4f));

            float x = cosf(p.angle) * p.distance;
            float z = sinf(p.angle) * p.distance;

            DrawSphere({ x, 0, z }, p.radius, p.color);
        }

        EndMode3D();
        DrawFPS(10, 870);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}