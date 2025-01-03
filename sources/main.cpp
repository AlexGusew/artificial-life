#include "constants.h"
#include "environment.cpp"
#include "organism.cpp"
#include "raylib.h"
#include <cstdlib>
#include <raymath.h>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

float batchSize = INITIAL_BATCH_SIZE;
int batchSizeInt = INITIAL_BATCH_SIZE;
int pause = 0;
float initialOrganisms = INITIAL_ORGANISMS;
int initialOrganismsInt = INITIAL_ORGANISMS;

void initOrganisms(Environment *env, std::vector<Organism *> &organisms)
{
  for (int i = 0; i < initialOrganismsInt; i++)
  {
    Color color = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
                   (unsigned char)(rand() % 256), 255};
    Vector2 position = {(float)(rand() % COLS), (float)(rand() % ROWS)};
    organisms.emplace_back(new Organism(100, 0, *env, color, position));
  }
}

void cleanupOrganisms(std::vector<Organism *> &organisms)
{
  for (auto &org : organisms)
  {
    delete org;
  }
  organisms.clear();
}

int main(void)
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;
  Environment *env = new Environment(ROWS, COLS);

  std::vector<Organism *> organisms;
  initOrganisms(env, organisms);
  Vector2 mousePosition = {0};
  Vector2 lastMousePosition = {0};
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

  while (!WindowShouldClose())
  {
    // Camera zooming
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      camera.zoom += wheel * 0.1f;
      if (camera.zoom < 0.1f)
        camera.zoom = 0.1f;
      Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      Vector2 delta = Vector2Subtract(newMouseWorldPos, mouseWorldPos);
      camera.target = Vector2Subtract(camera.target, delta);
    }

    // Camera moving
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);

      // mousePosition = GetMousePosition();
      // Vector2 delta = Vector2Subtract(mousePosition, lastMousePosition);
      // camera.target = Vector2Subtract(camera.target, Vector2Scale(delta, 1.0f
      // / camera.zoom));
    }

    lastMousePosition = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);

    if (IsKeyPressed(KEY_R))
    {
      cleanupOrganisms(organisms);
      env->Reset();
      initOrganisms(env, organisms);
    }

    if (IsKeyPressed(KEY_P))
    {
      pause = !pause;
    }

    BeginMode2D(camera);
    for (int i = 0; i < batchSizeInt && !pause; i++)
    {
      env->Update();
      for (auto &org : organisms)
      {
        org->Update();
      }
    }

    env->Draw();
    for (auto &org : organisms)
    {
      org->Draw();
    }

    EndMode2D();
    DrawText(
        TextFormat("Current FPS: %i\nReset: r\nPause: p", GetFPS()),
        10,
        10,
        10,
        WHITE);
    GuiSliderBar(
        (Rectangle){GetScreenWidth() - 140.0f, 20, 120, 20}, // Calculate x position dynamically
        "Skip frames",
        TextFormat("%d", (int)batchSize),
        &batchSize,
        0,
        100);
    GuiSliderBar(
        (Rectangle){GetScreenWidth() - 140.0f, 50, 120, 20}, // Calculate x position dynamically
        "Initial organisms",
        TextFormat("%d", (int)initialOrganisms),
        &initialOrganisms,
        0,
        100);
    // Convert float to int after using the slider
    initialOrganismsInt = (int)initialOrganisms;
    batchSizeInt = (int)batchSize;

    EndDrawing();
  }

  // Clean up
  cleanupOrganisms(organisms);
  delete env;

  CloseWindow();

  return 0;
}
