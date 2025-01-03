#include <raylib.h>
#include <raymath.h>

#include <cstdlib>
#include <vector>

#include "constants.h"
#include "environment.h"
#include "renderer.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float batchSize = INITIAL_BATCH_SIZE;
int batchSizeInt = INITIAL_BATCH_SIZE;
int pause = 0;
float initialOrganisms = INITIAL_ORGANISMS;
int initialOrganismsInt = INITIAL_ORGANISMS;

void initCells(Environment *env, std::vector<Cell *> &cells,
               Renderer &renderer) {
  for (int i = 0; i < initialOrganismsInt; i++) {
    Color color = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
                   (unsigned char)(rand() % 256), 255};
    Vector2 position = {(float)(rand() % COLS), (float)(rand() % ROWS)};
    Cell *cell = new Leave(position.x, position.y, 100, 0, *env, color);
    cells.push_back(cell);
    renderer.AddCell(cell);
  }
}

void cleanupCells(std::vector<Cell *> &cells) {
  for (auto &cell : cells) {
    delete cell;
  }
  cells.clear();
}

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;
  Environment *env = new Environment(ROWS, COLS);

  std::vector<Cell *> cells;
  Renderer renderer;
  initCells(env, cells, renderer);
  Vector2 mousePosition = {0};
  Vector2 lastMousePosition = {0};
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

  while (!WindowShouldClose()) {
    // Camera zooming
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      camera.zoom += wheel * 0.1f;
      if (camera.zoom < 0.1f) camera.zoom = 0.1f;
      Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      Vector2 delta = Vector2Subtract(newMouseWorldPos, mouseWorldPos);
      camera.target = Vector2Subtract(camera.target, delta);
    }

    // Camera moving
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);
    }

    lastMousePosition = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);

    if (IsKeyPressed(KEY_R)) {
      cleanupCells(cells);
      env->Reset();
      initCells(env, cells, renderer);
    }

    if (IsKeyPressed(KEY_P)) {
      pause = !pause;
    }

    BeginMode2D(camera);
    for (int i = 0; i < batchSizeInt && !pause; i++) {
      env->Update();
      renderer.Update(*env);
      for (auto &cell : cells) {
        cell->Update();
      }
    }

    env->Draw();
    renderer.Draw(cells);

    EndMode2D();
    DrawText(TextFormat("Current FPS: %i\nReset: r\nPause: p", GetFPS()), 10,
             10, 10, WHITE);
    GuiSliderBar((Rectangle){GetScreenWidth() - 140.0f, 20, 120,
                             20},  // Calculate x position dynamically
                 "Skip frames", TextFormat("%d", (int)batchSize), &batchSize, 0,
                 100);
    GuiSliderBar((Rectangle){GetScreenWidth() - 140.0f, 50, 120,
                             20},  // Calculate x position dynamically
                 "Initial organisms", TextFormat("%d", (int)initialOrganisms),
                 &initialOrganisms, 0, 100);
    // Convert float to int after using the slider
    initialOrganismsInt = (int)initialOrganisms;
    batchSizeInt = (int)batchSize;

    EndDrawing();
  }

  // Clean up
  cleanupCells(cells);
  delete env;

  CloseWindow();

  return 0;
}
