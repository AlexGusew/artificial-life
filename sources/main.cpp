#include "raylib.h"
#include <array>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (800)

#define WINDOW_TITLE "Window title"
#define CELL_SIZE 20.0f
#define ROWS 30
#define COLS 20

class Cell {
private:
  int x;
  int y;

public:
  Cell(int x, int y) : x(x), y(y) {}

  int GetX() { return x; }

  int GetY() { return y; }
};

class Environment {
private:
  std::vector<std::vector<Color>> colors;
  std::vector<std::vector<Cell *>> cells;
  int rows;
  int cols;

public:
  Environment(int rows, int cols)
      : rows(rows), cols(cols), colors(rows, std::vector<Color>(cols)),
        cells(rows, std::vector<Cell *>(cols, nullptr)) {
    for (auto &row : colors) {
      for (Color &color : row) {
        color = {(unsigned char)(rand() % 64), (unsigned char)(rand() % 64),
                 (unsigned char)(rand() % 64), (unsigned char)(rand() % 64)};
      }
    }
  }

  void Update() {}

  void Draw() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        DrawRectangleRec(
            (Rectangle){j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE},
            colors[i][j]);
      }
    }
  }

  std::vector<std::vector<Cell *>> &GetCells() { return cells; }
};

class Organism {
private:
  std::vector<Cell *> cells;
  std::deque<Cell *> todo;
  std::vector<std::pair<Cell *, Cell *>> edges;
  int health;
  int age;
  Environment &env;
  Color color;

public:
  Organism(int initialHealth, int initialAge, Environment &env, Color color,
           Vector2 initialPosition)
      : health(initialHealth), age(initialAge), env(env), color(color) {
    cells.push_back(new Cell(initialPosition.x, initialPosition.y));
    todo.push_back(cells.front());
  }

  ~Organism() {}

  int GetHealth() const { return health; }

  int GetAge() const { return age; }

  const std::vector<Cell *> &GetCells() const { return cells; }

  void Update() {
    age++;
    health--;
    if (!todo.empty()) {
      Cell *cell = todo.front();
      todo.pop_front();

      std::array<std::pair<int, int>, 4> directions = {
          {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
      for (auto &[dx, dy] : directions) {
        int newX = cell->GetX() + dx;
        int newY = cell->GetY() + dy;
        if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS &&
            env.GetCells()[newY][newX] == nullptr) {
          Cell *newCell = new Cell(newX, newY);
          env.GetCells()[newY][newX] = newCell;
          cells.emplace_back(newCell);
          todo.push_back(newCell);
          edges.emplace_back(cell, newCell);
        }
      }
    }
  }

  void Draw() const {
    for (auto &[source, dest] : edges) {
      Vector2 startPos = {source->GetX() * CELL_SIZE + CELL_SIZE / 2.0f,
                          source->GetY() * CELL_SIZE + CELL_SIZE / 2.0f};

      Vector2 endPos = {dest->GetX() * CELL_SIZE + CELL_SIZE / 2.0f,
                        dest->GetY() * CELL_SIZE + CELL_SIZE / 2.0f};
      DrawLineEx(startPos, endPos, 3.0f, color);
    }
    for (auto cell : cells) {
      DrawCircle(cell->GetX() * CELL_SIZE + CELL_SIZE / 2,
                 cell->GetY() * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE * 0.3f,
                 color);
    }
    DrawText(TextFormat("Health: %d", health), 400, 50, 20, BLACK);
    DrawText(TextFormat("Age: %d", age), 400, 80, 20, BLACK);
  }
};

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;

  Environment *env = new Environment(ROWS, COLS);

  std::vector<Organism *> organisms;
  for (int i = 0; i < 10; i++) {
    Color color = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
                   (unsigned char)(rand() % 256), 255};
    Vector2 position = {(float)(rand() % COLS), (float)(rand() % ROWS)};
    organisms.emplace_back(new Organism(100, 0, *env, color, position));
  }

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GRAY);
    BeginMode2D(camera);

    env->Update();

    for (auto &org : organisms) {
      org->Update();
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
