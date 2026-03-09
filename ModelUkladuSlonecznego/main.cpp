#include "raylib.h"

int main() {
    InitWindow(800, 450, "raylib [core]");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 190, 200, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}