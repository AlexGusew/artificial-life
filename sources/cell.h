#pragma once

#include <vector>

#include "raylib.h"

class Environment;

class Perceptron {
 public:
  Perceptron(int inputSize);
  float Activate(const std::vector<float> &inputs);
  void Mutate();
  const std::vector<float> &GetWeights() const;
  float GetBias() const;

 private:
  float Sigmoid(float x);
  std::vector<float> weights;
  float bias;
};

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
  Perceptron brain;

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
