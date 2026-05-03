#include "LevelLoad.h"

void LevelLoad::init() {
  std::ifstream file("./resources/resources/Level.csv");
  std::string rowLine;

  int currRow{0};
  if (file.is_open()) {
    std::cout << "file opened\n";
  } else {
    std::cout << "error opening file\n";
  }

  m_blockTypes = std::make_unique<std::vector<std::vector<int>>>(
      m_maxRows, std::vector<int>(m_maxCols, 0));

  while (std::getline(file, rowLine) && currRow < m_maxRows) {
    std::stringstream stream(rowLine);
    std::string cell;
    int currCol{0};
    while (std::getline(stream, cell, ',') && currCol < m_maxCols) {
      (*m_blockTypes)[currRow][currCol] = std::stoi(cell);
      currCol += 1;
    }
    currRow += 1;
  }
  file.close();

  for (int i{0}; i < m_maxRows; ++i) {
    for (int j{0}; j < m_maxCols; ++j) {
      BlockType type = static_cast<BlockType>((*m_blockTypes)[i][j]);
      float posY = i * m_tileSize;
      float posX = j * m_tileSize;
      Block block{posX, posY, type};
      (*m_blocks)[i][j] = block;
    }
  }
}

std::unique_ptr<std::vector<std::vector<Block>>> &LevelLoad::getBlocks() {
  return m_blocks;
}

void LevelLoad::render(sf::RenderWindow &window) {
  for (int i{0}; i < m_maxRows; ++i) {
    for (int j{0}; j < m_maxCols; ++j) {
      (*m_blocks)[i][j].setCorrectColor((*m_blocks)[i][j].getType());
      window.draw((*m_blocks)[i][j].getShape());
    }
  }
}

int LevelLoad::getRows() { return m_maxRows; }

int LevelLoad::getCols() { return m_maxCols; }
