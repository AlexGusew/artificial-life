#include "cell.h"

#include <raymath.h>

#include "constants.h"
#include "environment.h"
#include "raylib.h"

Cell::Cell(int x, int y, int initialHealth, int initialAge, Environment &env,
           Color color, Vector2 direction)
    : x(x),
      y(y),
      health(initialHealth),
      age(initialAge),
      env(env),
      color(color),
      direction(direction) {}

int Cell::GetX() { return x; }

int Cell::GetY() { return y; }

int Cell::GetHealth() const { return health; }
int Cell::GetAge() const { return age; }
Color Cell::GetColor() const { return color; }

void Cell::Update() {
  if (health <= 0) return;  // Skip update if the cell is dead
  age++;
  int nutrients = std::min(env.GetNutrients()[y][x], 10);
  env.GetNutrients()[y][x] -= nutrients;
  health += nutrients - 1;

  std::array<std::pair<int, int>, 4> directions = {
      {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  for (auto &[dx, dy] : directions) {
    int newX = x + dx;
    int newY = y + dy;
    if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS &&
        env.GetGrid()[newY][newX] == nullptr && health > 30 &&
        rand() % 100 < 30) {
      Cell *newCell;
      Vector2 direction = {static_cast<float>(dx), static_cast<float>(dy)};
      if (rand() % 100 < 50) {
        newCell = new Leave(newX, newY, health / 2, 0, env, color, direction);
        health /= 2;
      } else {
        newCell = new Trunk(newX, newY, health / 2, 0, env, color, direction);
        health /= 2;
      }
      env.GetGrid()[newY][newX] = newCell;
      env.todo.push_back(newCell);
    }
  }
}

void Cell::Draw() {}

void Cell::DrawConnection() {
  if (direction.x != 0 || direction.y != 0) {
    Vector2 parentCenter = {(x - direction.x + 0.5f) * CELL_SIZE,
                            (y - direction.y + 0.5f) * CELL_SIZE};
    Vector2 currentCenter = {(x + 0.5f) * CELL_SIZE, (y + 0.5f) * CELL_SIZE};
    DrawLineEx(parentCenter, currentCenter, CELL_SIZE * 0.3f, color);
  }
}

bool Cell::IsLeave() const { return false; }

Leave::Leave(int x, int y, int initialHealth, int initialAge, Environment &env,
             Color color, Vector2 direction)
    : Cell(x, y, initialHealth, initialAge, env, color, direction) {}

void Leave::Draw() {
  DrawCircle(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2,
             CELL_SIZE * 0.3f, color);
}

bool Leave::IsLeave() const { return true; }

Trunk::Trunk(int x, int y, int initialHealth, int initialAge, Environment &env,
             Color color, Vector2 direction)
    : Cell(x, y, initialHealth, initialAge, env, color, direction) {}

void Trunk::Draw() {
  Vector2 parentCenter = {(x - direction.x + 0.5f) * CELL_SIZE,
                          (y - direction.y + 0.5f) * CELL_SIZE};
  Vector2 currentCenter = {(x + 0.5f) * CELL_SIZE, (y + 0.5f) * CELL_SIZE};
  DrawLineEx(parentCenter, currentCenter, CELL_SIZE * 0.3f, color);
}
