#pragma once
#include <SFML/Graphics.hpp>
#include "HSVtoRGB.h"
#include "complexNumber.h"
#include <omp.h>

#define MAX_ITERATIONS 255
#define OUT_OF_BOUNDS 2.0e20f

bool isOutOfBounds(complexNumber c)
{
	if (abs(c.realPart) > OUT_OF_BOUNDS || abs(c.imaginaryPart) > OUT_OF_BOUNDS)
		return true;
	else return false;
}

unsigned short int mandelbrotSet(complexNumber c) ///returns 0 if c is part of the mandelbrot set, or otherwise, the number of iterations the point took to get out of bounds
{

	complexNumber z(0, 0);
	int it = 0;
	while (it < MAX_ITERATIONS)
	{
		if (isOutOfBounds(z))
			return it;
		z = z * z + c;
		it++;
	}
	return 0;
}

void computeImage(sf::Vector2f center, float range, sf::Vector2f canvasSize, sf::RenderTexture& tex, int numThreads = 4, bool progress = false)
{
	omp_set_num_threads(numThreads);

	bool* order = new bool[numThreads + 1]{ false };
	order[0] = true;

	if(progress)
		std::cout << "range: " << range << '\n';

#pragma  omp parallel
	{
		int prg = 0;
		int prg_perc = 0;
		unsigned short int threadNum = omp_get_thread_num();
		sf::RenderTexture tmpTexture;
		tmpTexture.create(canvasSize.x, canvasSize.y);

#pragma omp for
		for (int i = 0; i < (int)canvasSize.x; i++)
		{
			if (progress)
			{
				prg++;

				if ((int)(100 * (prg * numThreads / canvasSize.x)) >= prg_perc)
				{
					std::cout << "Thread "<< threadNum<< " progress: " << prg_perc << "%\n";
					prg_perc++;
				}



				/*
				if (threadNum == 0)
				{
					int totalLines = 0;
					for (int i = 0; i < omp_get_num_threads(); i++)
					{
						totalLines += prg[i];
					}
					
					if ((int)(100 * (totalLines / canvasSize.x)) >= prg_perc)
					{
						std::cout << "Current progress: " << prg_perc << "%\n";
						prg_perc++;
					}
				}
				*/


			}

			for (int j = 0; j < (int)canvasSize.y; j++)
			{


				complexNumber c(center.x, center.y);

				if (canvasSize.x > canvasSize.y)
				{
					c.realPart += ((2 * i - canvasSize.x) / canvasSize.x) * range;
					c.imaginaryPart += ((2 * j - canvasSize.y) / canvasSize.y) * range * (canvasSize.y / canvasSize.x);
				}
				else
				{
					c.realPart += ((2 * i - canvasSize.x) / canvasSize.x) * range * (canvasSize.x / canvasSize.y);
					c.imaginaryPart += ((2 * j - canvasSize.y) / canvasSize.y) * range;
				}
				
				
				int res = mandelbrotSet(c);


				sf::Vector3i color;
				sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
				pixel.setPosition(i, j);


				if (res == 0)
					color = sf::Vector3i(0, 0, 0);
				else
					color = HSVtoRGB(((res + 225) % 90) * 4 + (res + 225) % 4, 80, 100);

				pixel.setFillColor(sf::Color(color.x, color.y, color.z, 255));
				tmpTexture.draw(pixel);
			}

		}

		if(progress)
			std::cout << "Thread " << threadNum << " waiting \n";

		while (true)
		{
			if (order[threadNum] == true)
			{
				//std::cout << "drawing: thread " << threadNum << "\n";
				sf::Sprite tmp(tmpTexture.getTexture());
				tex.draw(tmp);
				order[threadNum + 1] = true;
				//std::cout << order[threadNum + 1] << '\n';
				if(progress)
					std::cout << "done drawing: thread " << threadNum << "\n";
				break;
			}
		}
	}
}