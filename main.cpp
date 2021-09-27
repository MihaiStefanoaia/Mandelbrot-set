#include <SFML/Graphics.hpp>
#include "HSVtoRGB.h"
#include "complexNumber.h"
#include <iostream>
#include <list>
#include <omp.h>
#define MAX_ITERATIONS 255
#define OUT_OF_BOUNDS 8.0f

void fitSpriteToWindow(sf::Sprite& sprite, sf::Window& window) //sets scale of the sprite to fit perfectly inside the screen
{
	sf::Vector2u windowSize = window.getSize(), spriteSize = sprite.getTexture()->getSize();
	sf::Vector2f scl((float)windowSize.x, (float)windowSize.y);
	scl.x /= spriteSize.x;
	scl.y /= spriteSize.y;
	sprite.setScale(scl);
}

bool isOutOfBounds(complexNumber c)
{
	if (abs(c.realPart) > OUT_OF_BOUNDS || abs(c.imaginaryPart) > OUT_OF_BOUNDS)
		return true;
	else return false;
}

complexNumber mandelbrotSetIteration(complexNumber z, complexNumber c)
{
	return z * z + c;
}

unsigned short int mandelbrotSet(complexNumber c) ///returns 0 if c is part of the mandelbrot set, or otherwise, the number of iterations the point took to get out of bounds
{

	complexNumber z(0,0);
	int it = 0;
	while (it < MAX_ITERATIONS)
	{


		if (isOutOfBounds(z))
			return it;
		z = mandelbrotSetIteration(z,c);
		it++;
	}
	return 0;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Mandelbrot Set Visualiser", sf::Style::Default);
	window.setFramerateLimit(60);
	sf::Vector2f center;
	float range, size;
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
	//testing only
	center = sf::Vector2f(-0.75f, 0.0f);
	range = 2;
	size = 250;

	sf::Vector2f canvasSize(size, size);
	/*
	unsigned short int** canvas;
	canvas = new unsigned short int* [canvasSize.x];
	for (int i = 0; i < canvasSize.x; i++)
	{
		canvas[i] = new unsigned short int[canvasSize.y]{ 0 };
	}
	*/
	sf::RectangleShape background(canvasSize);
	background.setFillColor(sf::Color(0, 0, 0, 255));

	sf::RenderTexture tex;
	tex.create(canvasSize.x, canvasSize.y);
	tex.draw(background);
	//tex.clear(sf::Color(0, 0, 0, 255));
	tex.display();

	sf::Sprite sprite(tex.getTexture());
	fitSpriteToWindow(sprite, window);

	sf::Vector2f origin(canvasSize/2.0f);

	omp_set_num_threads(4);

	bool needsUpdate = true;
	
	while (window.isOpen())
	{
		sf::Event evnt;
		window.pollEvent(evnt);
		switch (evnt.type)
		{
		case sf::Event::Closed:
		{
			std::string filename("outputs/Mandelbrot Set");
			filename += std::to_string((int)canvasSize.x);
			filename += "x";
			filename += std::to_string((int)canvasSize.y);
			filename += ".png";
			tex.getTexture().copyToImage().saveToFile(filename);

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

			if (evnt.text.unicode == 73 || evnt.text.unicode == 18)//down
			{
				center.y += (range / 10);
				needsUpdate = true;
			}

			if (evnt.text.unicode == 74 || evnt.text.unicode == 22)//up
			{
				center.y -= (range / 10);
				needsUpdate = true;
			}
			break;
		}
		}

		if (needsUpdate)
		{
			bool* order = new bool[omp_get_num_threads() + 1] {false};
			order[0] = true;
#pragma omp parallel
			{
				unsigned short int threadNum = omp_get_thread_num();
				sf::RenderTexture tmpTexture;
				tmpTexture.create(canvasSize.x, canvasSize.y);
				//tmpTexture.clear(sf::Color(0,0,0,0));
#pragma omp for
				for (int i = 0; i < (int)canvasSize.x; i++)
				{
					//std::cout << i << std::endl;
					for (int j = 0; j < (int)canvasSize.y; j++)
					{


						complexNumber c(center.x, center.y);

						c.realPart += ((2 * i - canvasSize.x) / canvasSize.x) * range;
						c.imaginaryPart += ((2 * j - canvasSize.y) / canvasSize.y) * range;
						int res = mandelbrotSet(c);


						sf::Vector3i color;
						sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
						pixel.setPosition(i, j);


						if (res == 0)
							color = sf::Vector3i(0, 0, 0);
						else
							//color = HSVtoRGB(((res + 225) % 90) * 4, 80, 100);
							color = HSVtoRGB(((res + 225) % 90) * 4 + (res + 225) % 4, 80, 100);

						pixel.setFillColor(sf::Color(color.x, color.y, color.z, 255));
						//tex.draw(pixel);
						tmpTexture.draw(pixel);
					}

				}


				std::cout<<"thread "<<threadNum<<"done\n";
				while (order[threadNum] == false)	//wait until the threads before it draw their part onto the texture
					continue;

				sf::Sprite tmp(tmpTexture.getTexture());
				tex.draw(tmp);

				std::cout << "thread " << threadNum << "done drawing\n";
				for (int i = 0; i < 1000; i++)	//give some time for synchronisation
					continue;
				/*
				the app was prone to crashes and unreliable frame drawing (specifically stitching the texture chunks), caused by issues with simultaneous OpenGL calls (error 0xC0000005 in nvoglv64.dll)
				the way that I fixed this was by spacing the drawing instructions by using an empty for loop (see the 2 lines above)
				it worked surprisingly well
				this is not a perfect fix, and the program still occasionally ends up with crashes and rendering artifacts, especially at the beginning, but it definitely helped a lot (went to right around the limit of 32bit floating point number limit for the zoom)
				*/
				
				order[threadNum + 1] = true;
			}
			needsUpdate = false;

		}

		window.draw(sprite);
		window.display();
	}

	return 0;
}