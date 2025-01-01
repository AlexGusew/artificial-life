#include "constants.h"
#include "environment.cpp"
#include "organism.cpp"
#include "raylib.h"
#include <cstdlib>
#include <raymath.h>

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;
  int frame = 0;
  Environment *env = new Environment(ROWS, COLS);

  std::vector<Organism *> organisms;
  for (int i = 0; i < INITIAL_ORGANISMS; i++) {
    Color color = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
                   (unsigned char)(rand() % 256), 255};
    Vector2 position = {(float)(rand() % COLS), (float)(rand() % ROWS)};
    organisms.emplace_back(new Organism(100, 0, *env, color, position));
  }

  Vector2 mousePosition = {0};
  Vector2 lastMousePosition = {0};

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
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                  Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);

      // mousePosition = GetMousePosition();
      // Vector2 delta = Vector2Subtract(mousePosition, lastMousePosition);
      // camera.target = Vector2Subtract(camera.target, Vector2Scale(delta, 1.0f / camera.zoom));
    }
    lastMousePosition = GetMousePosition();

    BeginDrawing();
    ClearBackground(GRAY);

    BeginMode2D(camera);

    for (int i = 0; i < BATCH_SIZE; i++) {
      env->Update();
      for (auto &org : organisms) {
        org->Update();
      }
    }
    env->Draw();
    for (auto &org : organisms) {
      org->Draw();
    }


    EndMode2D();
    DrawText(TextFormat("Current FPS: %i", GetFPS()), 10, 10, 10, BLACK);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
