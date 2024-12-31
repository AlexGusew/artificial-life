#include "constants.h"
#include "environment.cpp"
#include "organism.cpp"
#include "raylib.h"
#include <cstdlib>

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

  while (!WindowShouldClose()) {
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

    DrawText(TextFormat("Current FPS: %i", GetFPS()), 10, 10, 10, BLACK);

    EndMode2D();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
