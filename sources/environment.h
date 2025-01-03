#pragma once

#include <vector>

#include "cell.h"

class Environment {
 public:
  Environment(int rows, int cols);
  void Init();
  void Update();
  void Draw();
  void Reset();
  std::vector<std::vector<Cell*>>& GetCells();
  std::vector<std::vector<int>>& GetNutrients();

 private:
  std::vector<std::vector<Cell*>> cells;
  std::vector<std::vector<int>> nutrients;
  int rows;
  int cols;
};
