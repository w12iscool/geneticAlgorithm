#include "GameEngine.h"

void GameEngine::startup(std::mt19937 &gen) {
  m_levelLoader.init();
  m_population.init(gen, m_levelLoader.getBlocks(), m_levelLoader.getRows(),
                    m_levelLoader.getCols());
}

void GameEngine::update() {
  static float tickAccumulator = 0.0f;
  const float tickInterval = 0.005f; // 50ms

  tickAccumulator += m_clock.restart().asSeconds(); // SFML clock
  if (tickAccumulator >= tickInterval) {
    tickAccumulator = 0.0f;
    bool running = m_population.stepGeneration(m_levelLoader.getBlocks());
    if (!running) {
      m_population.evolveNextGen(m_levelLoader.getBlocks(),
                                 m_levelLoader.getRows(),
                                 m_levelLoader.getCols());
      m_currentGeneration += 1;
      std::cout << "Generation: " << m_currentGeneration << "\n";
      m_population.resetTicks();
    }
  }
}

void GameEngine::render(sf::RenderWindow &window) {
  m_levelLoader.render(window);
  m_population.render(window);
}

void GameEngine::shutdown() {}
