#pragma once

#include "Agent.h"
#include <algorithm>

class Population {
private:
  int m_popAmount{25};
  std::vector<Agent> m_population;
  std::mt19937 *m_gen;
  int m_tickAmt{300};
  int m_genAmt{50};

  float m_crossoverPercentage{0.9};
  float m_mutationRate{0.8};

  int m_currentTick{0};

public:
  void init(std::mt19937 &gen,
            std::unique_ptr<std::vector<std::vector<Block>>> &blocks,
            int rowCount, int colCount);

  Agent &tournamentSelect();
  std::vector<Action> crossover(Agent &agentParent1, Agent &agentParent2);

  void evolveNextGen(std::unique_ptr<std::vector<std::vector<Block>>> &blocks,
                     int rowCount, int colCount);

  void runGeneration(std::unique_ptr<std::vector<std::vector<Block>>> &blocks,
                     int rowCount, int colCount);

  void render(sf::RenderWindow &window);

  bool stepGeneration(std::unique_ptr<std::vector<std::vector<Block>>> &blocks);

  void resetTicks();
};
