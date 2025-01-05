#pragma once

#include <deque>
#include <vector>

#include "cell.h"

class Environment {
 public:
  Environment(int rows, int cols, Camera2D camera);
  void Init();
  void Update();
  void Draw();
  void Reset();
  std::vector<std::vector<Cell*>>& GetGrid();
  std::vector<std::vector<int>>& GetNutrients();
  void initCells(Environment* env);
  void Cleanup();
  void SafeUpdate();
  unsigned int GetTime();
  void PrintCellData();
  std::deque<Cell*> todo;
  ~Environment();
  Vector2 selectedCell;

 private:
  std::vector<std::vector<Cell*>> grid;
  std::vector<std::vector<int>> nutrients;
  int rows;
  int cols;
  unsigned int time;
  Camera2D camera;
};
