// the truly fun stuff. W_12
#include "Brain.h"

void Brain::init(std::mt19937 &gen, int tickAmt) {
  m_gen = &gen;
  m_tickAmt = tickAmt;

  std::uniform_int_distribution<int> randActDist(
      0, static_cast<int>(Action::Max) - 1);

  m_timeline.clear();
  // we're making a completely random timeline
  for (int i{0}; i < m_tickAmt; ++i) {
    Action randAct = static_cast<Action>(randActDist(*m_gen));
    m_timeline.push_back(randAct);
  }
}

void Brain::setTargets(float x, float y) {
  m_targetX = x;
  m_targetY = y;
}

std::vector<Action> &Brain::getTimeline() { return m_timeline; }

void Brain::mutate() {
  std::uniform_int_distribution<int> dist(0, static_cast<int>(Action::Max) - 1);
  std::uniform_real_distribution<float> floatDist(0.0f, 1.0f);
  if (m_lastDeathAction >= 0)
    m_timeline[m_lastDeathAction] = static_cast<Action>(dist((*m_gen)));

  // we randomely mutation a set number of events in our timeline based on our
  // mutation rate
  for (auto &gene : m_timeline) {
    if (floatDist(*m_gen) < m_mutationRatePerGene) {
      Action randAct = static_cast<Action>(dist(*m_gen));
      gene = randAct;
    }
  }
}

void Brain::setCurrentToDeath() { m_lastDeathAction = m_currentAction; }

void Brain::addTick() {
  if (m_currentAction + 1 >= m_tickAmt)
    return;

  m_currentAction += 1;
}

void Brain::reset() {
  m_currentAction = 0;
  m_fitness = 0;
}

void Brain::resetLastDeath() { m_lastDeathAction = -1; }

void Brain::setTimeline(std::vector<Action> &&timeline) {
  m_timeline = std::move(timeline);
}

void Brain::hitCoin() { m_fitness += 99999; }

void Brain::setPos(float x, float y) {
  m_agentPos.x = x;
  m_agentPos.y = y;
}

// basically we calculate fitness based on our distance from the goal
void Brain::fitnessEval() {
  float diffX = m_targetX - m_agentPos.x;
  float diffY = m_targetY - m_agentPos.y;
  float distance = sqrt(pow(diffX, 2) + pow(diffY, 2));
  m_fitness = (distance == 0) ? 9999999 : 1 / distance;
}

int Brain::getCurrentTick() { return m_currentAction; }

float Brain::getFitness() { return m_fitness; }

void Brain::setParentTimeline(Brain &parent1) {
  m_timeline = parent1.getTimeline();
}
