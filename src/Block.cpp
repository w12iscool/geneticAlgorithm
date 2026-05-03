#include "Block.h"

Block::Block(float xPos, float yPos, BlockType type) : m_type(type) {
  m_rectShape.setPosition({xPos, yPos});
  m_rectShape.setSize({m_size, m_size});
}

sf::RectangleShape &Block::getShape() { return m_rectShape; }

BlockType &Block::getType() { return m_type; }

float Block::getSize() { return m_size; }

void Block::setCorrectColor(BlockType type) {
  if (type == BlockType::Tile || type == BlockType::StartPos) {
    m_rectShape.setFillColor(sf::Color::Black);
  } else if (type == BlockType::Spike) {
    m_rectShape.setFillColor(sf::Color::Red);
  } else if (type == BlockType::Coin) {
    m_rectShape.setFillColor(sf::Color::Yellow);
  }
}
