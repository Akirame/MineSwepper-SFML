#include <SFML/Graphics.hpp>
#include <iostream>;
#include <time.h>
#include <SFML/Audio.hpp>
#ifdef DEBUG
#include "vld.h"
#endif
using namespace sf;

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(400, 400), "Minesweeper!");

	int width = 32;
	int grid[12][12];	// lo que hay realmente
	int subGrid[12][12]; // lo que se ve
	int countMines = 0;
	int countMinesAux = 0;
	int countFlags = 0;
	Font fuente;
	bool _gameOver = false;
	bool _win = false;
	bool playing = false;

	SoundBuffer soundFlagBuffer;
	SoundBuffer soundMineBuffer;
	SoundBuffer soundWinBuffer;
		
	if (!soundFlagBuffer.loadFromFile("sfx/flag.wav"))
	{
		printf("no se pudo cargar flag.wav");
	}

	if (!soundMineBuffer.loadFromFile("sfx/explosion.wav"))
	{
		printf("no se pudo cargar explosion.wav");
	}
	if (!soundWinBuffer.loadFromFile("sfx/win.wav"))
	{
		printf("no se pudo cargar win.wav");
	}

	Sound soundFlag;
	Sound soundMine;
	Sound soundWin;
	soundFlag.setBuffer(soundFlagBuffer);
	soundWin.setBuffer(soundWinBuffer);
	soundMine.setBuffer(soundMineBuffer);

	if (!fuente.loadFromFile("arial.ttf"))
	{
		printf("No se pudo cargar fuente");
	}

	Texture textureGrid;
	if (!textureGrid.loadFromFile("images/tiles.png"))
	{
		printf("No se pudo cargar image");
	}
	Sprite spriteGrid(textureGrid);

	Text finalText;
	finalText.setString("Victory!");
	finalText.setCharacterSize(24);
	finalText.setPosition(170, 1);
	finalText.setFont(fuente);
	finalText.setFillColor(Color::Black);





	for (int i = 1;i <= 10;i++)
		for (int j = 1;j <= 10;j++)
		{
			subGrid[i][j] = 10;
			if (rand() % 7 == 0)
			{
				grid[i][j] = 9;
				countMines++;
			}
			else
				grid[i][j] = 0;
		}

	Text minesText;
	minesText.setString("Mines:" + std::to_string(countMines));
	minesText.setCharacterSize(24);
	minesText.setPosition(1, 1);
	minesText.setFont(fuente);
	minesText.setFillColor(Color::Black);

	for (int i = 1;i <= 10;i++)
		for (int j = 1;j <= 10;j++)
		{
			int n = 0;
			if (!(grid[i][j] == 9))
			{
				if (grid[i + 1][j] == 9) n++;
				if (grid[i][j + 1] == 9) n++;
				if (grid[i - 1][j] == 9) n++;
				if (grid[i][j - 1] == 9) n++;
				if (grid[i + 1][j + 1] == 9) n++;
				if (grid[i - 1][j - 1] == 9) n++;
				if (grid[i - 1][j + 1] == 9) n++;
				if (grid[i + 1][j - 1] == 9) n++;
				grid[i][j] = n;
			}
		}

	while (window.isOpen())
	{
		Vector2i posMouse = Mouse::getPosition(window);
		int x = posMouse.x / width;
		int y = posMouse.y / width;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed || sf::Keyboard::isKeyPressed(Keyboard::Key::Escape))
				window.close();
			if (!_gameOver)
			{
				if (e.type == Event::MouseButtonPressed)
					if (e.key.code == Mouse::Left)
						subGrid[x][y] = grid[x][y];
					else
						if (e.key.code == Mouse::Right)
							if (subGrid[x][y] == 10)
							{
								subGrid[x][y] = 11;
								soundFlag.play();
							}
							else
								if (subGrid[x][y] == 11)
									subGrid[x][y] = 10;
			}
		}


		window.clear(Color::White);
		countMinesAux = countMines;
		countFlags = 0;

		for (int i = 1;i <= 10;i++)
			for (int j = 1;j <= 10;j++)
			{
				if (subGrid[x][y] == 9)
				{
					subGrid[i][j] = grid[i][j];
					_gameOver = true;
					_win = false;
				}

				spriteGrid.setTextureRect(IntRect(subGrid[i][j] * width, 0, width, width));
				spriteGrid.setPosition(i*width, j*width);
				window.draw(spriteGrid);
			}

		if (!_gameOver)
		{
			for (int i = 1;i <= 10;i++)
				for (int j = 1;j <= 10;j++)
				{
					if (subGrid[i][j] == 11)
					{
						countFlags++;
						if (grid[i][j] == 9)
							countMinesAux--;
					}
				}
		}
		else
			if (_win == true)
			{
				if (!playing)
				{
					soundWin.setVolume(50);
					soundWin.play();
					playing = true;
				}
				window.draw(finalText);

			}
			else
			{
				if (!playing)
				{
					soundMine.setVolume(50);
					soundMine.play();
					playing = true;
				}

				finalText.setString("Boom!");
				window.draw(finalText);
			}

		if (countFlags == countMines && countMinesAux == 0)
		{
			_gameOver = true;
			_win = true;
		}

		window.draw(minesText);
		window.display();
	}

	return 0;
}


