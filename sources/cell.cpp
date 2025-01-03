#include "cell.h"

#include "constants.h"
#include "environment.h"
#include "raylib.h"

Cell::Cell(int x, int y, int initialHealth, int initialAge, Environment &env,
           Color color)
    : x(x),
      y(y),
      health(initialHealth),
      age(initialAge),
      env(env),
      color(color) {}

int Cell::GetX() { return x; }

int Cell::GetY() { return y; }

int Cell::GetHealth() const { return health; }
int Cell::GetAge() const { return age; }
Color Cell::GetColor() const { return color; }

void Cell::Update() {
  if (health <= 0) return;  // Skip update if the cell is dead
  age++;
  health--;
  std::array<std::pair<int, int>, 8> directions = {
      {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}}};
  for (auto &[dx, dy] : directions) {
    int newX = x + dx;
    int newY = y + dy;
    if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS &&
        env.GetGrid()[newY][newX] == nullptr) {
      Cell *newCell;
      if (rand() % 100 < 50) {
        newCell = new Leave(newX, newY, 100, 0, env, color);
      } else {
        newCell = new Trunk(newX, newY, 100, 0, env, color);
      }
      env.GetGrid()[newY][newX] = newCell;
      env.todo.push_back(newCell);
    }
  }
}

void Cell::Draw() {}

bool Cell::IsLeave() const { return false; }

Leave::Leave(int x, int y, int initialHealth, int initialAge, Environment &env,
             Color color)
    : Cell(x, y, initialHealth, initialAge, env, color) {}

void Leave::Draw() {
  DrawCircle(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2,
             CELL_SIZE * 0.3f, color);
}

bool Leave::IsLeave() const { return true; }

Trunk::Trunk(int x, int y, int initialHealth, int initialAge, Environment &env,
             Color color)
    : Cell(x, y, initialHealth, initialAge, env, color) {}

void Trunk::Draw() {
  DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
