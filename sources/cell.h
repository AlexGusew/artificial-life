#pragma once

#include "raylib.h"

class Environment;  // Forward declaration

class Cell {
 public:
  Cell(int x, int y, int initialHealth, int initialAge, Environment &env,
       Color color, Vector2 direction);
  virtual int GetX();
  virtual int GetY();
  int GetHealth() const;
  int GetAge() const;
  Color GetColor() const;
  void Update();
  virtual void Draw();
  virtual bool IsLeave() const;
  void DrawConnection();

 protected:
  int x;
  int y;
  Color color;
  Vector2 direction;

 private:
  int health;
  int age;
  Environment &env;
};

class Leave : public Cell {
 public:
  Leave(int x, int y, int initialHealth, int initialAge, Environment &env,
        Color color, Vector2 direction);
  void Draw() override;
  bool IsLeave() const override;
};

class Trunk : public Cell {
 public:
  Trunk(int x, int y, int initialHealth, int initialAge, Environment &env,
        Color color, Vector2 direction);
  void Draw() override;
};
