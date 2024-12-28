#include "raylib.h"
#include <array>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <stdatomic.h>
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
    DrawText(TextFormat("test"), 400, 50, 20, BLACK);
  }

  std::vector<std::vector<Cell *>> GetCells() { return cells; }
};

class Organism {
private:
  std::vector<Cell> cells;
  std::deque<Cell> todo;
  int health;
  int age;
  Environment env;

public:
  Organism(int initialHealth, int initialAge, Environment &env)
      : health(initialHealth), age(initialAge), env(env) {
    cells.push_back(Cell(10, 10));
    todo.push_back(cells.front());
  }

  ~Organism() {}

  int GetHealth() const { return health; }

  int GetAge() const { return age; }

  const std::vector<Cell> &GetCells() const { return cells; }

  void Update() {
    age++;
    health--;
    if (!todo.empty()) {
      Cell cell = todo.front();
      todo.pop_front();

      std::array<std::pair<int, int>, 4> dirs = {
          {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

      for (auto &pair : dirs) {
        int x = cell.GetX() + pair.first;
        int y = cell.GetY() + pair.second;
        std::cout << env.GetCells()[x][y] << std::endl;
        if (x >= 0 && x < ROWS && y >= 0 && y < COLS &&
            env.GetCells()[x][y] == nullptr) {
          std::cout << std::size(todo) << " " << x << " " << y << " " << env.GetCells()[x][y] << std::endl;
          Cell* newCell = new Cell(x, y);
          env.GetCells()[x][y] = newCell;
          cells.push_back(*newCell);
          todo.push_back(*newCell);
        }
      }
    }
  }

  void Draw() const {
    for (auto cell : cells) {
      DrawCircle(cell.GetX() * CELL_SIZE + CELL_SIZE / 2,
                 cell.GetY() * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE * 0.3f,
                 BLUE);
    }
    DrawText(TextFormat("Health: %d", health), 200, 50, 20, LIGHTGRAY);
    DrawText(TextFormat("Age: %d", age), 200, 80, 20, LIGHTGRAY);
  }
};

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.zoom = 1.0f;

  Environment *env = new Environment(ROWS, COLS);
  Organism *organism = new Organism(100, 0, *env);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GREEN);
    BeginMode2D(camera);

    env->Update();
    organism->Update();

    env->Draw();
    organism->Draw();

    EndMode2D();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
