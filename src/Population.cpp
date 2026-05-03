#include "Population.h"

// Init function so we have all of our agents
void Population::init(std::mt19937 &gen,
                      std::unique_ptr<std::vector<std::vector<Block>>> &blocks,
                      int rowCount, int colCount) {
  m_gen = &gen;
  m_population.reserve(m_popAmount);
  for (int i{0}; i < m_popAmount; ++i) {
    Agent newAgent;
    newAgent.init(gen, m_tickAmt, blocks, rowCount, colCount);
    m_population.emplace_back(std::move(newAgent));
  }
}

Agent &Population::tournamentSelect() {
  // We use tournament select to randomely find the agent with the best fintness
  int k = 3;
  float bestFitness{-1};
  int bestIndx{-1};
  std::uniform_int_distribution<int> dist(0, m_population.size() - 1);

  // we choose k random ppl and compare them with each other. obviously k is 3
  // lower k = more random, higher k = less random
  // we use 3 because I think its a good balance imo
  for (int i{0}; i < k; ++i) {
    int indx = dist(*m_gen);
    if (m_population[indx].getBrain().getFitness() > bestFitness) {
      bestFitness = m_population[indx].getBrain().getFitness();
      bestIndx = indx;
    }
  }
  return m_population[bestIndx];
}

// we're breeding our agents
std::vector<Action> Population::crossover(Agent &agentParent1,
                                          Agent &agentParent2) {
  // an empty vector which will be our new child's timeline
  std::vector<Action> childTimeline;
  std::uniform_int_distribution<int> dist(
      1, agentParent1.getBrain().getTimeline().size() - 1);
  int crossoverPoint = dist(*m_gen);

  std::vector<Action> &parent1 = agentParent1.getBrain().getTimeline();
  std::vector<Action> &parent2 = agentParent2.getBrain().getTimeline();
  childTimeline.reserve(parent1.size());

  // anything left of the crossover point is parent 1 and then anything right of
  // the crossover point is parent 2
  for (int i{0}; i < crossoverPoint; ++i) {
    Action action = parent1[i];
    childTimeline.push_back(action);
  }
  for (int i = crossoverPoint; i < parent2.size(); ++i) {
    Action action = parent2[i];
    childTimeline.push_back(action);
  }

  return childTimeline;
}

void Population::evolveNextGen(
    std::unique_ptr<std::vector<std::vector<Block>>> &blocks, int rowCount,
    int colCount) {
  for (auto &agent : m_population) {
    // we have to get our fitness eval before we evolve
    agent.getBrain().fitnessEval();
  }
  std::vector<Agent> newPop;
  std::sort(m_population.begin(), m_population.end(),
            [](Agent &agent1, Agent &agent2) {
              return agent1.getBrain().getFitness() >
                     agent2.getBrain().getFitness();
            });

  // our top 3 best agents
  for (int i{0}; i < 3; ++i) {
    newPop.push_back(m_population[i]);
  }

  std::uniform_real_distribution<float> distPerc(0.0f, 1.0f);
  while (newPop.size() < m_population.size()) {
    Agent &parent1 = tournamentSelect();
    Agent &parent2 = tournamentSelect();
    Agent child;
    child.init(*m_gen, m_tickAmt, blocks, rowCount, colCount);
    if (distPerc(*m_gen) < m_crossoverPercentage) {
      child.getBrain().setTimeline(crossover(parent1, parent2));
    } else {
      child.getBrain().setParentTimeline(parent1.getBrain());
    }

    // we can mutate randomely to make sure we evolve throughout generations
    if (distPerc(*m_gen) < m_mutationRate) {
      child.getBrain().mutate();
    }
    child.getBrain().fitnessEval();
    newPop.push_back(std::move(child));
  }
  m_population = std::move(newPop);
}

// the fun stuff
void Population::runGeneration(
    std::unique_ptr<std::vector<std::vector<Block>>> &blocks, int rowCount,
    int colCount) {
  for (Agent &agent : m_population) {
    // reset all of our agents
    agent.reset();
  }

  for (int tick{0}; tick < m_tickAmt; ++tick) {
    bool anyAlive{false};
    for (Agent &agent : m_population) {
      if (!agent.isAlive()) {
        continue;
      }
      agent.applyEvents();
      agent.handleCollisions(blocks);

      if (agent.isAlive()) {
        anyAlive = true;
      }
    }
    if (!anyAlive) {
      break;
    }
  }
  for (Agent &agent : m_population) {
    agent.getBrain().fitnessEval();
  }
}

void Population::render(sf::RenderWindow &window) {
  for (auto &agent : m_population) {
    agent.render(window);
  }
}

bool Population::stepGeneration(
    std::unique_ptr<std::vector<std::vector<Block>>> &blocks) {
  bool anyAlive = false;

  for (Agent &agent : m_population) {
    if (!agent.isAlive())
      continue;

    agent.applyEvents();
    agent.handleCollisions(blocks);

    if (agent.isAlive())
      anyAlive = true;
  }

  m_currentTick++;

  return anyAlive && m_currentTick < m_tickAmt;
}

void Population::resetTicks() { m_currentTick = 0; }
