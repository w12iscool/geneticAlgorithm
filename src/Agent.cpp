// Discord: w_12, Roblox: DoubleUTwelve

#include "Agent.h"

// init our brain and place our agent in the correct spot in our tilemap
void Agent::init(std::mt19937 &gen, int tickAmt,
                 std::unique_ptr<std::vector<std::vector<Block>>> &blocks,
                 int rows, int cols) {
  m_brain.init(gen, tickAmt);
  m_rowsAmt = rows;
  m_colsAmt = cols;
  // we navigate through our tilemap through row major order
  for (int i{0}; i < rows; ++i) {
    for (int j{0}; j < cols; ++j) {
      Block &block = (*blocks)[i][j];
      if (block.getType() == BlockType::StartPos) {
        m_startPos = {j * block.getSize(), i * block.getSize()};
        m_rectShape.setPosition(m_startPos);
        m_brain.setPos(m_rectShape.getPosition().x,
                       m_rectShape.getPosition().y);
        m_rectShape.setSize({m_size, m_size});
        m_rectShape.setFillColor(sf::Color::Blue);
      } else if (block.getType() == BlockType::Coin) {
        m_targetX = block.getShape().getPosition().x;
        m_targetY = block.getShape().getPosition().y;
        m_brain.setTargets(m_targetX, m_targetY);
      }
    }
  }
}

void Agent::render(sf::RenderWindow &window) {
  // sfml rendering stuff
  if (m_alive)
    window.draw(m_rectShape);
}

void Agent::reset() {
  m_alive = true;
  m_coined = false;
  m_movementSpeed = m_constMovementSpeed;
  m_brain.reset();
  m_rectShape.setPosition(m_startPos);
}

void Agent::die() {
  if (!m_alive)
    return;

  m_coined = false;
  // brain dead lol
  m_brain.setCurrentToDeath();
}

// we dont truly get rid of our agents because this is easier
void Agent::revive() {
  reset();
  m_coined = false;
  m_brain.resetLastDeath();
  m_movementSpeed = m_constMovementSpeed;
  m_rectShape.setSize({m_size, m_size});
}

// collision handling. literally just checking if our agent's tile is the same
// as a spike's tile
void Agent::handleCollisions(
    std::unique_ptr<std::vector<std::vector<Block>>> &blocks) {
  if (!m_alive)
    return;
  for (int i{0}; i < m_rowsAmt; ++i) {
    for (int j{0}; j < m_colsAmt; ++j) {
      Block &block = (*blocks)[i][j];
      if (m_rectShape.getGlobalBounds().findIntersection(
              block.getShape().getGlobalBounds())) {
        BlockType &type = block.getType();
        if (type == BlockType::Spike) {
          die();
          m_alive = false;
        } else if (type == BlockType::Coin && !m_coined) {
          m_coined = true;
          m_brain.hitCoin();
        }
      }
    }
  }
}

// we're using our brain's timeline to move every tick
void Agent::applyEvents() {
  int indx = m_brain.getCurrentTick();
  if (indx >= m_brain.getTimeline().size())
    return;
  if (!m_alive)
    return;

  Action action = m_brain.getTimeline()[indx];
  if (action == Action::Up) {
    m_rectShape.move({0, -m_movementSpeed});
  } else if (action == Action::Down) {
    m_rectShape.move({0, m_movementSpeed});
  } else if (action == Action::Left) {
    m_rectShape.move({-m_movementSpeed, 0});
  } else if (action == Action::Right) {
    m_rectShape.move({m_movementSpeed, 0});
  }

  m_brain.setPos(m_rectShape.getPosition().x, m_rectShape.getPosition().y);
  m_brain.addTick();
}
Brain &Agent::getBrain() { return m_brain; }

bool Agent::isAlive() { return m_alive; }
