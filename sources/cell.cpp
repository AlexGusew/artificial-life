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
  age++;
  health--;
}

void Cell::Draw(Color color) {}

bool Cell::IsLeave() const { return false; }

Leave::Leave(int x, int y, int initialHealth, int initialAge, Environment &env,
             Color color)
    : Cell(x, y, initialHealth, initialAge, env, color) {}

void Leave::Draw(Color color) {
  DrawCircle(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2,
             CELL_SIZE * 0.3f, color);
}

bool Leave::IsLeave() const { return true; }

Trunk::Trunk(int x, int y, int initialHealth, int initialAge, Environment &env,
             Color color)
    : Cell(x, y, initialHealth, initialAge, env, color) {}
