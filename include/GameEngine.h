#pragma once
#include "LevelLoad.h"
#include "Population.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
class GameEngine {
private:
  LevelLoad m_levelLoader;
  std::mt19937 m_gen;
  Population m_population;
  int m_currentGeneration{0};
  sf::Clock m_clock;

public:
  void startup(std::mt19937 &gen);
  void update();
  void render(sf::RenderWindow &window);
  void shutdown();
};
