#pragma once

#include <SFML/Graphics.hpp>

enum class BlockType { Tile, Spike, StartPos, Coin };

class Block {
private:
  sf::RectangleShape m_rectShape{{0, 0}};
  BlockType m_type{BlockType::Tile};
  float m_size{20};

public:
  Block() = default;
  Block(float xPos, float yPos, BlockType type);

  sf::RectangleShape &getShape();
  BlockType &getType();
  float getSize();

  void setCorrectColor(BlockType type);
};
