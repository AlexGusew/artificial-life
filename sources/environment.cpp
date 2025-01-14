#include "environment.h"

#include <cstdlib>

#include "constants.h"
#include "raylib.h"

Environment::Environment(int rows, int cols, Camera2D& camera)
    : rows(rows),
      cols(cols),
      grid(rows, std::vector<Cell*>(cols, nullptr)),
      nutrients(rows, std::vector<int>(cols)),
      todo(),
      camera(camera) {
  Init();
}

void Environment::initCells(Environment* env) {
  for (int i = 0; i < initialOrganismsInt; i++) {
    Color color = {(unsigned char)(rand() % 256), (unsigned char)(rand() % 256),
                   (unsigned char)(rand() % 256), 255};
    Vector2 position = {(float)(rand() % COLS), (float)(rand() % ROWS)};
    Cell* cell =
        new Leave(position.x, position.y, MAX_HEALTH, 0, *env, color, {0, 0});
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
  unsigned int HELTH_TO_DIE = 20;
  for (Cell* cell : tempTodo) {
    if (cell->GetHealth() > HELTH_TO_DIE && cell->GetAge() < MAX_AGE) {
      cell->Update();
      todo.push_back(cell);
    } else {
      grid[cell->GetY()][cell->GetX()] = nullptr;
      nutrients[cell->GetY()][cell->GetX()] += cell->GetHealth();
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

  Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
  float x = mouseWorldPos.x / CELL_SIZE;
  float y = mouseWorldPos.y / CELL_SIZE;
  if (x >= 0 && x < cols && y >= 0 && y < rows) {
    selectedCell = {x, y};
  } else {
    selectedCell = {-1, -1};
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
    cell = nullptr;
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

void Environment::PrintCellData() {
  int x = selectedCell.x, y = selectedCell.y;
  if (grid[y][x] != nullptr) {
    Cell* cell = grid[y][x];
    std::string cellData =
        "Cell at (" + std::to_string(x) + ", " + std::to_string(y) + "):\n";
    cellData += "  Health: " + std::to_string(cell->GetHealth()) + "\n";
    cellData += "  Age: " + std::to_string(cell->GetAge()) + "\n";
    cellData += "  Color: (" + std::to_string((int)cell->GetColor().r) + ", " +
                std::to_string((int)cell->GetColor().g) + ", " +
                std::to_string((int)cell->GetColor().b) + ")\n";
    cellData += "  Brain weights:\n";
    const auto& weights = cell->brain.GetWeights();
    cellData += "    Age weight: " + std::to_string(weights[0]) + "\n";
    cellData += "    Health weight: " + std::to_string(weights[1]) + "\n";
    cellData += "    Nutrients weight: " + std::to_string(weights[2]) + "\n";
    cellData += "  Brain bias: " + std::to_string(cell->brain.GetBias()) + "\n";

    DrawText(cellData.c_str(), GetScreenWidth() - 200, GetScreenHeight() - 400,
             20, WHITE);
  }
}
