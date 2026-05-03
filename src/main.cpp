#include "GameEngine.h"
#include <SFML/Graphics.hpp>
#include <random>

int main() {
  sf::RenderWindow window{sf::VideoMode{{800, 600}}, "Coin Finder"};
  std::random_device rd;
  std::mt19937 gen(rd());

  GameEngine game;
  game.startup(gen);
  while (window.isOpen()) {
    while (std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    game.update();
    window.clear();
    game.render(window);
    window.display();
  }
  game.shutdown();
  return 0;
}
