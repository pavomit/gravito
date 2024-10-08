#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <cmath>
#include <ctime>

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

	//background
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("../assets/images/background.jpg")) {
		std::cerr << "Error loading background texture @assets/images";
	}

	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.3, 1.8);

	sf::Texture rocketPlatformTexture;
	if (!rocketPlatformTexture.loadFromFile("../assets/images/platform.png")) {
		std::cerr << "Error loading rocket platfrm image @assets/images";
	}

	sf::Sprite rocketPlatformSprite(rocketPlatformTexture);
	rocketPlatformSprite.setScale(0.1, 0.04);
	rocketPlatformSprite.setPosition(windowWidth - rocketPlatformSprite.getGlobalBounds().width, windowHeight - rocketPlatformSprite.getGlobalBounds().height);


	//game variables
	float GRAVITY = 0.2f;
	sf::Vector2f velocity(0, 0); 

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
	sf::Vector2f playerInitialPosition(60, 100);
	playerSprite.setScale(0.5, 0.5);
	playerSprite.setPosition(playerInitialPosition);


	//meteors
	const float METEOR_MOVE_SPEED_DOWN = 2.0f;
	const float METEOR_MOVE_SPEED_HORIZONTAL = 1.0f;

	sf::Texture meteorTexture;
	if (!meteorTexture.loadFromFile("../assets/images/meteor.png")) {
		std::cerr << "Error loading meteor texture @assets/images";
	}

	const int numberOfMeteors{ 10 };
	sf::Sprite meteorsSprites[numberOfMeteors];

	for (int i = 0; i < numberOfMeteors; i++) {
		meteorsSprites[i].setTexture(meteorTexture);
		meteorsSprites[i].setScale(0.1, 0.1);
		//meteorsSprites[i].setPosition(i * 80, 20);

		float randomX = static_cast<float>(std::rand()) / RAND_MAX * windowWidth;
		float randomY = static_cast<float>(std::rand()) / RAND_MAX * windowHeight;
		meteorsSprites[i].setPosition(randomX, randomY);

		
		float randomVelocityX = static_cast<float>(std::rand()) / RAND_MAX * 5.0f - 2.5f; 
		float randomVelocityY = static_cast<float>(std::rand()) / RAND_MAX * 3.0f - 1.5f; 
		meteorsSprites[i].move(randomVelocityX, randomVelocityY);
	}

	std::srand(std::time(0));

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

			// moving meteors 
			for (int i = 0; i < numberOfMeteors; i++) {
				meteorsSprites[i].move(0, METEOR_MOVE_SPEED_DOWN);

				float randomHorizontalMovement = std::rand() % 2 == 0 ? -METEOR_MOVE_SPEED_HORIZONTAL : METEOR_MOVE_SPEED_HORIZONTAL;
				meteorsSprites[i].move(randomHorizontalMovement, 0);
			}

			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					velocity.x = 7;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					velocity.x = -7;
				}

				if (sf::Keyboard::isKeyPressed(::sf::Keyboard::Up)) {
					velocity.y = -7;
				}

				if (sf::Keyboard::isKeyPressed(::sf::Keyboard::Down)) {
					velocity.y = 7;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					velocity.y = -6; 
				}

			}

			if (playerSprite.getPosition().y + playerSprite.getGlobalBounds().height <= windowHeight) {
				playerSprite.rotate(5);
				velocity.y += GRAVITY;
				playerSprite.move(velocity);
			}
			else {

				//  landing
				if (!playerSprite.getGlobalBounds().intersects(rocketPlatformSprite.getGlobalBounds())) {
					// If the sprite hits the ground
					playerSprite.setPosition(playerInitialPosition);
					gameState = State::lose;
				}
				else {
					playerSprite.setRotation(0);
					GRAVITY = 0;
					playerSprite.setRotation(0);
					std::this_thread::sleep_for(std::chrono::seconds(2));
					gameState = State::win;
				}
				
			}

			//window collision
			if (playerSprite.getPosition().x - playerSprite.getGlobalBounds().width / 2 < 0 || playerSprite.getPosition().x + playerSprite.getGlobalBounds().width > windowWidth) {
				playerSprite.move(-velocity.x, velocity.y);
			}
			
			if (playerSprite.getPosition().y - playerSprite.getGlobalBounds().height / 2 < 0) {
				playerSprite.move(velocity.x, -velocity.y);
			}

			// collision with meteors
			for (int i = 0; i < numberOfMeteors; i++) {
				sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
				sf::FloatRect meteorBounds = meteorsSprites[i].getGlobalBounds();

				if (playerBounds.intersects(meteorBounds)) {
					gameState = State::lose;
					for (int i = 0; i < numberOfMeteors; i++) {
						meteorsSprites[i].setPosition(static_cast<float>(std::rand()) / RAND_MAX * windowWidth, static_cast<float>(std::rand()) / RAND_MAX * windowHeight);
						meteorsSprites[i].move(0, 0);
					}
					break; 
				}
			}

			window.clear();
			window.draw(backgroundSprite);
			window.draw(rocketPlatformSprite);
			for (int i = 0; i < numberOfMeteors; i++)
			{
				window.draw(meteorsSprites[i]);
			}
			window.draw(playerSprite);
			window.display();
		}

		if (gameState == State::win) {
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					// Reset meteors
					for (int i = 0; i < numberOfMeteors; i++) {
						meteorsSprites[i].setPosition(static_cast<float>(std::rand()) / RAND_MAX * windowWidth, static_cast<float>(std::rand()) / RAND_MAX * windowHeight);
						meteorsSprites[i].move(0, 0);
					}

					playerSprite.setPosition(playerInitialPosition);
					gameState = State::play;
					playerSprite.setPosition(playerInitialPosition);
				}
			}

			sf::Text playAgain;
			playAgain.setFont(font);
			playAgain.setString("yapeee you won .... press Enter to play again");
			playAgain.setFillColor(sf::Color::White);
			playAgain.setCharacterSize(30);

			playAgain.setPosition((windowWidth / 2) - playAgain.getGlobalBounds().width / 2, (windowHeight / 2) - playAgain.getGlobalBounds().height / 2);

			window.clear();
			window.draw(playAgain);
			window.display();
		}

		if (gameState == State::lose) {
			GRAVITY = 0.2f;
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

