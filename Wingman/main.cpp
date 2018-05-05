#include"Game.h"
#include"GameMapMaker.h"

bool MAPMAKER = false;

int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(1920, 1080), "Wingman Game", Style::Default);

	Clock clock;
	float dt = 0.f;

	Game game(&window);
	GameMapMaker gameMapMaker(&window);

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

		if (MAPMAKER)
		{
			gameMapMaker.update(dt);
			gameMapMaker.draw();
		}
		else
		{
			game.update(dt);
			game.draw();
		}
	}

	return 0;
}