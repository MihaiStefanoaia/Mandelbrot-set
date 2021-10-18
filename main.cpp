#include <SFML/Graphics.hpp>
#include "HSVtoRGB.h"
#include "complexNumber.h"
#include "compute.h"
#include <iostream>
#include <list>
#include <omp.h>



void fitSpriteToWindow(sf::Sprite& sprite, sf::Window& window) //sets scale of the sprite to fit perfectly inside the screen
{
	sf::Vector2u windowSize = window.getSize(), spriteSize = sprite.getTexture()->getSize();
	sf::Vector2f scl((float)windowSize.x, (float)windowSize.y);
	scl.x /= spriteSize.x;
	scl.y /= spriteSize.y;
	sprite.setScale(scl);
}




int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Mandelbrot Set Visualiser", sf::Style::Default);
	window.setFramerateLimit(60);
	sf::Vector2f center;
	float range, size;

	center = sf::Vector2f(-0.75f, 0.0f);
	range = 2;
	size = 250;

	/*
	std::cout << "Select your center point:\n x:\n";
	std::cin >> center.x;
	std::cout << "y:\n";
	std::cin >> center.y;
	std::cout << "Select the size of the window to be rendered:\n";
	std::cin >> range;
	std::cout << "Select the canvas size to be rendered (pixels):\n";
	std::cin >> size;
	*/
	//^^^ leftover from old versions


	sf::Vector2f canvasSize(size, size);
	sf::RectangleShape background(canvasSize);
	background.setFillColor(sf::Color(0, 0, 0, 255));

	sf::RenderTexture tex;
	tex.create(canvasSize.x, canvasSize.y);
	tex.draw(background);
	tex.display();

	sf::Sprite sprite(tex.getTexture());
	fitSpriteToWindow(sprite, window);

	sf::Vector2f origin(canvasSize/2.0f);

	omp_set_num_threads(6);

	bool needsUpdate = true;
	
	while (window.isOpen())
	{
		sf::Event evnt;
		window.pollEvent(evnt);
		switch (evnt.type)
		{
		case sf::Event::Closed:
		{
			/*
			std::string filename("outputs/Mandelbrot Set");
			filename += std::to_string((int)canvasSize.x);
			filename += "x";
			filename += std::to_string((int)canvasSize.y);
			filename += ".png";
			tex.getTexture().copyToImage().saveToFile(filename);
			*/
			window.close();
			break;
		}
		case sf::Event::KeyPressed:
		{
			std::cout << evnt.text.unicode << '\n';
			if (evnt.text.unicode == 68 || evnt.text.unicode == 56) //zoom in
			{
				range *= 1.25;
				needsUpdate = true;
			}

			if (evnt.text.unicode == 67 || evnt.text.unicode == 55) //zoom out
			{
				range *= 0.8;
				needsUpdate = true;
			}

			if (evnt.text.unicode == 71 || evnt.text.unicode == 0)//left
			{
				center.x -= (range / 10);
				needsUpdate = true;
			}

			if (evnt.text.unicode == 72 || evnt.text.unicode == 3)//right
			{
				center.x += (range / 10);
				needsUpdate = true;
			}

			if (evnt.text.unicode == 73 || evnt.text.unicode == 22)//down
			{
				center.y += (range / 10);
				needsUpdate = true;
			}

			if (evnt.text.unicode == 74 || evnt.text.unicode == 18)//up
			{
				center.y -= (range / 10);
				needsUpdate = true;
			}

			if (evnt.text.unicode == 15)//save
			{
				std::string confirm;
				std::cout << "Do you want to render point (" << center.x << " , " << center.y << ") with range " << range << "? (Y/N)\n";
				std::getline(std::cin, confirm);

				if (confirm == "Y" || confirm == "y")
				{
					sf::Vector2f resolution;
					std::cout << "Select the resolution at which you want to render\nx: ";
					std::cin >> resolution.x;
					std::cout << "y: ";
					std::cin >> resolution.y;
					
					sf::RectangleShape bkg(resolution);
					bkg.setFillColor(sf::Color(0, 0, 0, 255));

					sf::RenderTexture save;
					save.create((int)resolution.x, (int)resolution.y);
					save.draw(bkg);
					save.display();

					computeImage(center, range, resolution, save, true);

					std::string filename("outputs/");
					filename += "center_(";
					filename += std::to_string(center.x);
					filename += "_,_";
					filename += std::to_string(center.y);
					filename += ")_range_";
					filename += std::to_string(range);
					filename += ".png";
					save.getTexture().copyToImage().saveToFile(filename);

				}

			}
			break;
		}
		}

		if (needsUpdate)
		{
			computeImage(center, range, canvasSize, tex);
			needsUpdate = false;
		}

		window.draw(sprite);
		window.display();
	}

	return 0;
}