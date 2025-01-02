#pragma once

#include "cell.cpp"
#include "constants.h"
#include "raylib.h"
#include <vector>

class Environment {
private:
  std::vector<std::vector<Cell *>> cells;
  std::vector<std::vector<int>> nutrients;
  int rows;
  int cols;

public:
  Environment(int rows, int cols)
      : rows(rows), cols(cols), cells(rows, std::vector<Cell *>(cols, nullptr)),
        nutrients(rows, std::vector<int>(cols)) {
    Init();
  }

  void Init() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        nutrients[i][j] =
            rand() % (MAX_NUTRIENTS - MIN_NUTRIENTS) + MIN_NUTRIENTS;
        delete cells[i][j];
        cells[i][j] = nullptr;
      }
    }
  }

  void Update() {}

  void Draw() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        Color color = {static_cast<unsigned char>(0.0f),
                       static_cast<unsigned char>(255.0f),
                       static_cast<unsigned char>(0.0f),
                       static_cast<unsigned char>((float)nutrients[i][j] /
                                                  MAX_NUTRIENTS * 255)};
        DrawRectangleRec(
            (Rectangle){j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE},
            color);
      }
    }
  }

  void Reset() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (cells[i][j] != nullptr) {
          delete cells[i][j];
          cells[i][j] = nullptr;
        }
      }
    }
    Init();
  }

  std::vector<std::vector<Cell *>> &GetCells() { return cells; }
  std::vector<std::vector<int>> &GetNutrients() { return nutrients; }
};
