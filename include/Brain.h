#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

enum class Action { Up, Right, Down, Left, Max };
class Brain {
private:
  std::vector<Action> m_timeline;
  int m_currentAction{0};
  int m_lastDeathAction{-1};
  std::mt19937 *m_gen{nullptr};
  int m_tickAmt{0};
  float m_fitness{0};

  sf::Vector2f m_agentPos;

  float m_mutationRate{0.1};
  float m_mutationRatePerGene{0.01f};
  float m_crossoverRate{0.6};

  float m_targetX{0};
  float m_targetY{0};

public:
  void init(std::mt19937 &gen, int tickAmt);
  void setTargets(float x, float y);
  std::vector<Action> &getTimeline();
  void mutate();
  void setCurrentToDeath();
  void addTick();
  void reset();
  void resetLastDeath();
  void setTimeline(std::vector<Action> &&timeline);
  void hitCoin();
  void setPos(float x, float y);
  void fitnessEval();
  int getCurrentTick();
  float getFitness();
  void setParentTimeline(Brain &parent1);
};
