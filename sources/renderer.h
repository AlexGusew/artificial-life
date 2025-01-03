#pragma once

#include <deque>
#include <utility>
#include <vector>

#include "cell.h"
#include "environment.h"

class Renderer {
 private:
  std::deque<Cell *> todo;
  std::vector<std::pair<Cell *, Cell *>> edges;

 public:
  void AddCell(Cell *cell);
  void Update(Environment &env);
  void Draw(const std::vector<Cell *> &cells) const;
};
