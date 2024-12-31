#pragma once

#include "constants.h"
#include "raylib.h"

class Cell {
protected:
  int x;
  int y;

public:
  Cell(int x, int y) : x(x), y(y) {}

  virtual int GetX() { return x; }

  virtual int GetY() { return y; }

  virtual void Draw(Color color) {}

  virtual bool IsLeave() const { return false; }
};

class Leave : public Cell {
public:
  Leave(int x, int y) : Cell(x, y) {}

  void Draw(Color color) override {
    DrawCircle(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2,
               CELL_SIZE * 0.3f, color);
  };

  bool IsLeave() const override { return true; }
};

class Trunk : public Cell {
public:
  Trunk(int x, int y) : Cell(x, y) {}
};
