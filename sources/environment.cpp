#include "environment.h"

#include <cstdlib>

#include "constants.h"
#include "raylib.h"

Environment::Environment(int rows, int cols)
    : rows(rows),
      cols(cols),
      grid(rows, std::vector<Cell*>(cols, nullptr)),
      nutrients(rows, std::vector<int>(cols)),
      todo() {
  Init();
}

void Environment::initCells(Environment* env) {
  for (int i = 0; i < initialOrganismsInt; i++) {
    Color color = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
                   (unsigned char)(rand() % 256), 255};
    Vector2 position = {(float)(rand() % COLS), (float)(rand() % ROWS)};
    Cell* cell = new Leave(position.x, position.y, 100, 0, *env, color, {0, 0});
    todo.push_back(cell);
  }
}

void Environment::Init() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      nutrients[i][j] =
          rand() % (MAX_NUTRIENTS - MIN_NUTRIENTS) + MIN_NUTRIENTS;
    }
  }
}

void Environment::SafeUpdate() {
  time++;
  std::deque<Cell*> tempTodo = std::move(todo);
  todo.clear();
  for (Cell* cell : tempTodo) {
    if (cell->GetHealth() > 0) {
      cell->Update();
      todo.push_back(cell);
    } else {
      grid[cell->GetY()][cell->GetX()] = nullptr;
      nutrients[cell->GetY()][cell->GetX()] += 20;
      delete cell;
    }
  }
}

void Environment::Update() { SafeUpdate(); }

void Environment::Draw() {
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
  for (auto& cell : todo) {
    cell->DrawConnection();
    cell->Draw();
  }
}

void Environment::Cleanup() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j] != nullptr) {
        delete grid[i][j];
        grid[i][j] = nullptr;
      }
    }
  }
  for (auto& cell : todo) {
    cell = nullptr;  // Avoid double free by setting pointers to nullptr
  }
  time = 0;
  todo.clear();
}

unsigned int Environment::GetTime() { return time; }

void Environment::Reset() {
  Cleanup();
  Init();
}

Environment::~Environment() { Cleanup(); }

std::vector<std::vector<Cell*>>& Environment::GetGrid() { return grid; }
std::vector<std::vector<int>>& Environment::GetNutrients() { return nutrients; }
