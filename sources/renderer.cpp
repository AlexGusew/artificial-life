#include "renderer.h"

#include <array>
#include <cstdlib>

#include "constants.h"

void Renderer::AddCell(Cell *cell) { todo.push_back(cell); }

void Renderer::Update(Environment &env) {
  if (!todo.empty()) {
    Cell *cell = todo.front();
    todo.pop_front();

    std::array<std::pair<int, int>, 8> directions = {
        {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}}};
    for (auto &[dx, dy] : directions) {
      int newX = cell->GetX() + dx;
      int newY = cell->GetY() + dy;
      if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS &&
          env.GetCells()[newY][newX] == nullptr) {
        Cell *newCell;
        if (rand() % 100 < 100) {
          newCell = new Leave(newX, newY, 100, 0, env, cell->GetColor());
        } else {
          newCell = new Trunk(newX, newY, 100, 0, env, cell->GetColor());
        }
        env.GetCells()[newY][newX] = newCell;
        todo.push_back(newCell);
        edges.emplace_back(cell, newCell);
      }
    }
  }
}

void Renderer::Draw(const std::vector<Cell *> &cells) const {
  for (auto &[source, dest] : edges) {
    Vector2 startPos = {source->GetX() * CELL_SIZE + CELL_SIZE / 2.0f,
                        source->GetY() * CELL_SIZE + CELL_SIZE / 2.0f};

    Vector2 endPos = {dest->GetX() * CELL_SIZE + CELL_SIZE / 2.0f,
                      dest->GetY() * CELL_SIZE + CELL_SIZE / 2.0f};
    DrawLineEx(startPos, endPos, CELL_SIZE / 3.0f, source->GetColor());
  }
  for (auto cell : cells) {
    cell->Draw(cell->GetColor());
  }
}
