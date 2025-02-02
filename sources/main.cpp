#include <raylib.h>
#include <raymath.h>

#include <cstdlib>

#include "constants.h"
#include "environment.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float batchSize = INITIAL_BATCH_SIZE;
int batchSizeInt = INITIAL_BATCH_SIZE;
int pause = 0;
float initialOrganisms = INITIAL_ORGANISMS;
int initialOrganismsInt = INITIAL_ORGANISMS;
int frames = 0;

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;
  Environment *env = new Environment(ROWS, COLS, camera);
  env->initCells(env);

  Vector2 mousePosition = {0};
  Vector2 lastMousePosition = {0};
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

  while (!WindowShouldClose()) {
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      camera.zoom += wheel * 0.1f;
      if (camera.zoom < 0.1f) camera.zoom = 0.1f;
      Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      Vector2 delta = Vector2Subtract(newMouseWorldPos, mouseWorldPos);
      camera.target = Vector2Subtract(camera.target, delta);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);
    }

    lastMousePosition = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);

    if (IsKeyPressed(KEY_P)) {
      pause = !pause;
    }

    BeginMode2D(camera);

    frames++;
    if (IsKeyPressed(KEY_N)) {
      env->Update();
    } else {
      for (int i = 0; frames % 1 == 0 && i < batchSizeInt && !pause; i++) {
        env->Update();
      }
    }

    if (IsKeyPressed(KEY_R)) {
      env->Reset();
      env->initCells(env);
    }

    env->Draw();

    EndMode2D();
    DrawText(TextFormat("Time: %zu\nCurrent FPS: %i\nCells: %zu\n\nReset: "
                        "r\nPause: p\nNext frame: n\n",
                        env->GetTime(), GetFPS(), env->todo.size()),
             10, 10, 10, WHITE);
    GuiSliderBar((Rectangle){GetScreenWidth() - 140.0f, 20, 120, 20},
                 "Skip frames", TextFormat("%d", (int)batchSize), &batchSize, 1,
                 100);
    GuiSliderBar((Rectangle){GetScreenWidth() - 140.0f, 50, 120, 20},
                 "Initial organisms", TextFormat("%d", (int)initialOrganisms),
                 &initialOrganisms, 1, 500);

    initialOrganismsInt = (int)initialOrganisms;
    batchSizeInt = (int)batchSize;

    if (env->selectedCell.x >= 0 && env->selectedCell.y >= 0) {
      env->PrintCellData();
    }

    EndDrawing();
  }

  delete env;

  CloseWindow();

  return 0;
}
