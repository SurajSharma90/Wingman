#include"Game.h"

int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(1920, 1080), "Wingman Game", Style::Default);

	Clock clock;
	float dt = 0.f;

	Game game(&window);

	//Game loop
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();
		}

		dt = clock.restart().asSeconds();

		game.Update(dt);
		game.Draw();
	}

	return 0;
}