#pragma once

#include "Block.h"
#include "Brain.h"
#include <SFML/Graphics.hpp>
#include <memory.h>
#include <random>
#include <vector>

class Agent {
private:
  sf::RectangleShape m_rectShape;
  sf::Vector2f m_startPos;
  Brain m_brain;
  float m_size{20.0f};
  float m_movementSpeed{20.0f};
  float m_constMovementSpeed{20.0f};

  float m_rowsAmt{0};
  float m_colsAmt{0};

  float m_targetX{0};
  float m_targetY{0};
  bool m_alive{true};
  bool m_coined{false};

public:
  void init(std::mt19937 &gen, int tickAmt,
            std::unique_ptr<std::vector<std::vector<Block>>> &blocks, int rows,
            int cols);

  void render(sf::RenderWindow &window);
  void reset();
  void die();
  void revive();

  void
  handleCollisions(std::unique_ptr<std::vector<std::vector<Block>>> &blocks);
  void applyEvents();
  Brain &getBrain();
  bool isAlive();
};
