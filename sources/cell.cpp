#include "cell.h"

#include <raymath.h>

#include <algorithm>
#include <cmath>
#include <vector>

#include "constants.h"
#include "environment.h"
#include "raylib.h"

Perceptron::Perceptron(int inputSize) : weights(inputSize), bias(1.0f) {
  for (auto &weight : weights) {
    // Initialize weights randomly between -1 and 1
    weight = static_cast<float>(rand()) / RAND_MAX * 2 - 1;
  }
  bias = static_cast<float>(rand()) / RAND_MAX * 2 -
         1;  // Initialize bias randomly between -1 and 1
}

float Perceptron::Activate(const std::vector<float> &inputs) {
  float sum = 0.0f;
  for (size_t i = 0; i < inputs.size(); ++i) {
    sum += inputs[i] * weights[i];
  }
  sum += bias;  // Add bias to the sum
  return Sigmoid(sum);
}

float Perceptron::Sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }

void Perceptron::Mutate() {
  if (rand() % 2 == 0) {
    // Mutate one of the weights
    int index = rand() % weights.size();
    weights[index] += static_cast<float>(rand()) / RAND_MAX * 0.2f - 0.1f;
  } else {
    // Mutate the bias
    bias += static_cast<float>(rand()) / RAND_MAX * 0.2f - 0.1f;
  }
}

Cell::Cell(int x, int y, int initialHealth, int initialAge, Environment &env,
           Color color, Vector2 direction)
    : x(x),
      y(y),
      health(initialHealth),
      age(initialAge),
      env(env),
      color(color),
      direction(direction),
      brain(3) {}  // Initialize perceptron with 3 inputs

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

  // brain with perceptron
  std::vector<float> inputs = {
      age / (float)MAX_AGE, static_cast<float>(health) / MAX_HEALTH,
      static_cast<float>(env.GetNutrients()[y][x]) / MAX_NUTRIENTS};
  float decision = brain.Activate(inputs);

  std::array<std::pair<int, int>, 4> directions = {
      {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  for (auto &[dx, dy] : directions) {
    int newX = (x + dx + COLS) % COLS;  // Wrap around horizontally
    int newY = (y + dy + ROWS) % ROWS;  // Wrap around vertically
    if (env.GetGrid()[newY][newX] == nullptr && decision > 0.5f) {
      Cell *newCell;
      Vector2 direction = {static_cast<float>(dx), static_cast<float>(dy)};
      if (rand() % 100 < 50) {
        newCell = new Leave(newX, newY, health / 2, 0, env, color, direction);
        health /= 2;
      } else {
        newCell = new Trunk(newX, newY, health / 2, 0, env, color, direction);
        health /= 2;
      }
      if (rand() % 100 < 1) {     // 1% chance to mutate
        newCell->brain.Mutate();  // Call mutate on the brain
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
