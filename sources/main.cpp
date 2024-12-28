#include "raylib.h"
#include <array>
#include <cstdlib>

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "Window title"

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;

  Color grid[20][30];
  for (auto &row : grid) {
    for (Color &cell : row) {
      cell = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
              (unsigned char)(rand() % 256), (unsigned char)(rand() % 256)};
    }
  }

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GREEN);
    BeginMode2D(camera);

    for (int i = 0; i < std::size(grid); i++) {
      for (int j = 0; j < std::size(grid[i]); j++) {
        DrawRectangleRec((Rectangle){i * 20.0f, j * 20.0f, 20, 20}, grid[i][j]);
      }
    }

    EndMode2D();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
