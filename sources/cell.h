#pragma once

#include "raylib.h"

class Environment;  // Forward declaration

class Cell {
 public:
  Cell(int x, int y, int initialHealth, int initialAge, Environment &env,
       Color color);
  virtual int GetX();
  virtual int GetY();
  int GetHealth() const;
  int GetAge() const;
  Color GetColor() const;
  void Update();
  virtual void Draw(Color color);
  virtual bool IsLeave() const;

 protected:
  int x;
  int y;

 private:
  int health;
  int age;
  Color color;
  Environment &env;
};

class Leave : public Cell {
 public:
  Leave(int x, int y, int initialHealth, int initialAge, Environment &env,
        Color color);
  void Draw(Color color) override;
  bool IsLeave() const override;
};

class Trunk : public Cell {
 public:
  Trunk(int x, int y, int initialHealth, int initialAge, Environment &env,
        Color color);
};
