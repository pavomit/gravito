#include <iostream>
#include <SFML/Graphics.hpp>


//game states 
enum class State {
	undefined, // default value(0)
	mainMenu,
	play,
	win,
	lose
};


int main() {

	// window 
	sf::RenderWindow window(sf::VideoMode(1400, 800), "Gravito");
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);
	int windowWidth = window.getSize().x;
	int windowHeight = window.getSize().y;

	const float GRAVITY = 5.0f;

	// game state
	State gameState{ State::mainMenu };

	// displayed text
	sf::Font font;
	if (!font.loadFromFile("../assets/fonts/font.ttf")) {
		std::cerr << "Error loading font @assets/fonts";
	}

	sf::Text mainText;
	mainText.setFont(font);

	// player
	sf::Texture playerTexture;
	if (!playerTexture.loadFromFile("../assets/images/rocket.png")) {
		std::cerr << "Error loading player texture @assets/images/";
	}

	sf::Sprite playerSprite(playerTexture);
	sf::Vector2f playerInitialPosition(100, 100);
	playerSprite.setScale(0.5, 0.5);
	playerSprite.setPosition(playerInitialPosition);


	while (window.isOpen()) {

		if (gameState == State::mainMenu) {

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					gameState = State::play;
				}
			}

			sf::Text GameName;
			GameName.setFont(font);
			GameName.setCharacterSize(100);
			GameName.setString("Gravito");
			GameName.setFillColor(sf::Color(211, 84, 0));

			mainText.setString("Press Enter to start ..... ");
			mainText.setFillColor(sf::Color::White);
			mainText.setCharacterSize(35);
			// positioning 
			GameName.setPosition((windowWidth / 2) - GameName.getGlobalBounds().width / 2, (windowHeight / 2) - 300 - GameName.getGlobalBounds().height / 2);
			mainText.setPosition((windowWidth / 2) - mainText.getGlobalBounds().width / 2, (windowHeight / 2) - mainText.getGlobalBounds().height / 2);

			window.clear();
			window.draw(GameName);
			window.draw(mainText);
			window.display();
		}

		if (gameState == State::play) {

			playerSprite.setOrigin(playerSprite.getGlobalBounds().width / 2, playerSprite.getGlobalBounds().height / 2);


			if (playerSprite.getPosition().y + playerSprite.getGlobalBounds().height <= windowHeight) {
				playerSprite.move(0, GRAVITY); 
			}
			else {
				// If the sprite hits the ground, keep its position fixed or lose 
				//playerSprite.setPosition(100, windowHeight - playerSprite.getGlobalBounds().height);
				playerSprite.setPosition(playerInitialPosition);
				gameState = State::lose;
			}

			playerSprite.rotate(5);



			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}


			window.clear();
			window.draw(playerSprite);
			window.display();
		}

		if (gameState == State::win) {
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(sf::Color::Green);
			window.display();
		}

		if (gameState == State::lose) {
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					gameState = State::play;

			}
			sf::Text playAgain;
			playAgain.setFont(font);
			playAgain.setString("press Enter to play again");
			playAgain.setFillColor(sf::Color::White);
			playAgain.setCharacterSize(30);

			mainText.setString("You Lost ..... ");
			mainText.setFillColor(sf::Color::White);
			mainText.setCharacterSize(35);
			// positioning 
			mainText.setPosition((windowWidth / 2) - mainText.getGlobalBounds().width / 2, (windowHeight / 2) - 100 -  mainText.getGlobalBounds().height / 2);
			playAgain.setPosition((windowWidth / 2) - playAgain.getGlobalBounds().width / 2, (windowHeight / 2) - playAgain.getGlobalBounds().height / 2);

			window.clear();
			window.draw(mainText);
			window.draw(playAgain);
			window.display();
		}
	}

	return 0;
}

