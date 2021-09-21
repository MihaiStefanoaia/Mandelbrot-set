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
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Mandelbrot Set Visualiser", sf::Style::Default);
	window.setFramerateLimit(60);
	sf::Vector2f center;
	float range, size;
	
	std::cout << "Select your center point:\n x:\n";
	std::cin >> center.x;
	std::cout << "y:\n";
	std::cin >> center.y;
	std::cout << "Select the size of the window to be rendered:\n";
	std::cin >> range;
	std::cout << "Select the canvas size to be rendered (pixels):\n";
	std::cin >> size;
	
	//testing only
	//center = sf::Vector2f(-0.75f, 0.0f);
	//range = 2;
	//size = 1000;

	sf::Vector2f canvasSize(size, size);

	unsigned short int** canvas;
	canvas = new unsigned short int* [canvasSize.x];
	for (int i = 0; i < canvasSize.x; i++)
	{
		canvas[i] = new unsigned short int[canvasSize.y]{ 0 };
	}

	sf::RectangleShape background(canvasSize);
	background.setFillColor(sf::Color(0, 0, 0, 255));

	sf::RenderTexture tex;
	tex.create(canvasSize.x, canvasSize.y);
	tex.draw(background);
	tex.display();

	sf::Sprite sprite(tex.getTexture());
	fitSpriteToWindow(sprite, window);

	sf::Vector2f origin(canvasSize/2.0f);

	std::cout << "processing\n";
	omp_set_num_threads(8);
#pragma omp parallel for
	for (int i = 0; i < (int)canvasSize.x; i++)
	{
		std::cout << i << std::endl;
		for (int j = 0; j < (int)canvasSize.y; j++)
		{
			
			
			complexNumber c(center.x, center.y);

			c.realPart += ((2 * i - canvasSize.x) / canvasSize.x)* range;
			c.imaginaryPart += ((2 * j - canvasSize.y) / canvasSize.y) * range;
			canvas[i][j] = mandelbrotSet(c);
		}
	}
	std::cout << "coloring\n";
	
//#pragma omp parallel for
	for (int i = 0; i < (int)canvasSize.x; i++)
	{
		std::cout << i << std::endl;
		for (int j = 0; j < (int)canvasSize.y; j++)
		{
			sf::Vector3i color;
			sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
			pixel.setPosition(i, j);
			

			if (canvas[i][j] == 0)
				color = sf::Vector3i(0, 0, 0);
			else
				//color = HSVtoRGB(((res + 225) % 90) * 4, 80, 100);
				color = HSVtoRGB(((canvas[i][j] + 225) % 90) * 4 + (canvas[i][j] + 225) % 4, 80, 100);

			pixel.setFillColor(sf::Color(color.x, color.y, color.z, 255));
			tex.draw(pixel);
		}
	
	}

	
	while (window.isOpen())
	{
		sf::Event evnt;
		window.pollEvent(evnt);
		switch (evnt.type)
		{
		case::sf::Event::Closed:

			std::string filename("outputs/Mandelbrot Set");
			filename += std::to_string((int)canvasSize.x);
			filename += "x";
			filename += std::to_string((int)canvasSize.y);
			filename += ".png";
			tex.getTexture().copyToImage().saveToFile(filename);

			window.close();
			break;
		}

		window.draw(sprite);
		window.display();
	}

	return 0;
}