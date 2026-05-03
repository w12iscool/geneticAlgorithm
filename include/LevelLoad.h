#pragma once

#include "Block.h"
#include <fstream>
#include <iostream>
#include <memory.h>
#include <sstream>
#include <vector>

class LevelLoad {
private:
  int m_maxCols{40};
  int m_maxRows{30};
  std::unique_ptr<std::vector<std::vector<Block>>> m_blocks =
      std::make_unique<std::vector<std::vector<Block>>>(
          m_maxRows, std::vector<Block>(m_maxCols));

  std::unique_ptr<std::vector<std::vector<int>>> m_blockTypes = nullptr;
  float m_tileSize{20};

public:
  void init();
  std::unique_ptr<std::vector<std::vector<Block>>> &getBlocks();
  void render(sf::RenderWindow &window);
  int getRows();
  int getCols();
};
