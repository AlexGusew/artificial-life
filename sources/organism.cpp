#pragma once

#include "raylib.h"
#include <array>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

#include "cell.cpp"
#include "constants.h"
#include "environment.cpp"

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
    cells.push_back(new Leave(initialPosition.x, initialPosition.y));
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
          Cell *newCell;
          if (rand() % 100 < 30) {
            newCell = new Leave(newX, newY);
          } else {
            newCell = new Trunk(newX, newY);
          }
          env.GetCells()[newY][newX] = newCell;
          cells.emplace_back(newCell);
          todo.push_back(newCell);
          edges.emplace_back(cell, newCell);
        }
      }
    }
    /*std::cout << std::size(cells) << std::endl;*/
    auto &nutrients = env.GetNutrients();
    for (Cell *cell : cells) {
      if (dynamic_cast<Leave*>(cell)) {
      // change to variable voracity depending on cell type
      int& gridItem = nutrients[cell->GetY()][cell->GetX()];
      gridItem = std::max(0, gridItem - 1);
      }
    }
  }

  void Draw() const {
    for (auto &[source, dest] : edges) {
      Vector2 startPos = {source->GetX() * CELL_SIZE + CELL_SIZE / 2.0f,
                          source->GetY() * CELL_SIZE + CELL_SIZE / 2.0f};

      Vector2 endPos = {dest->GetX() * CELL_SIZE + CELL_SIZE / 2.0f,
                        dest->GetY() * CELL_SIZE + CELL_SIZE / 2.0f};
      DrawLineEx(startPos, endPos, CELL_SIZE / 3.0f, color);
    }
    for (auto cell : cells) {
      /*std::cout << typeid(cell).name() << std::endl;*/
      cell->Draw(color);
    }
    /*DrawText(TextFormat("Health: %d", health), 400, 50, 20, BLACK);*/
    /*DrawText(TextFormat("Age: %d", age), 400, 80, 20, BLACK);*/
  }
};
