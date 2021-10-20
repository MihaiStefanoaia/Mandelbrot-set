#pragma once
#include <SFML/Graphics.hpp>
#include "compute.h"
#include "button.h"
#include "textInput.h"
#include "textBox.h"
#include <array>
#include <thread>
#include <list>

void startRenderer(sf::Vector2f center, float range, int numThreads = 4)
{
	omp_set_num_threads(numThreads);


	sf::RenderWindow renderer(sf::VideoMode(500, 300), "Render the scene");

	button exit(sf::Vector2f(25, 200), sf::Vector2f(200, 100), "Exit", "CC");
	button start(sf::Vector2f(275, 200), sf::Vector2f(200, 100), "Start", "CC");

	std::string centerText, rangeText;
	sf::Vector2f resolution;

	centerText = "Center point (" + std::to_string(center.x) + "," + std::to_string(center.y) + ")";
	rangeText = "Range " + std::to_string(range);

	textBox point(sf::Vector2f(0, 100), sf::Vector2f(500, 50), centerText,"LC");
	textBox rang(sf::Vector2f(0, 150), sf::Vector2f(500, 50), rangeText, "LC");

	textInput resolution_x(sf::Vector2f(0, 0), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Render resolution X:");
	textInput resolution_y(sf::Vector2f(0, 50), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Render resolution Y:");

	std::array<textInput*,2> textInputs = { &resolution_x, &resolution_y};

	bool continueToken = false;

	while (renderer.isOpen())
	{
		sf::Event evnt;
		renderer.pollEvent(evnt);
		switch (evnt.type)
		{
		case sf::Event::Closed:
			renderer.close();
			break;
		case sf::Event::KeyPressed:

			if (evnt.text.unicode >= 26 && evnt.text.unicode <= 35)	//regular number keys
			{
				char tmp = (char)('0' + evnt.text.unicode - 26);

				std::string add(1, tmp);

				for (int i = 0; i < textInputs.size(); i++)
					if (textInputs[i]->active == true)
						textInputs[i]->addText(add);
			}

			if (evnt.text.unicode >= 75 && evnt.text.unicode <= 84)	//numpad
			{
				char tmp = (char)('0' + evnt.text.unicode - 75);


				std::string add(1, tmp);

				for (int i = 0; i < textInputs.size(); i++)
					if (textInputs[i]->active == true)
						textInputs[i]->addText(add);
			}

			if (evnt.text.unicode == 36) //esc key to leave the config menu without starting the generator
			{
				renderer.close();
			}

			if (evnt.text.unicode == 58) //enter to leave the config menu and start the generator with the inputted variables
			{
				continueToken = true;
				resolution.x = resolution_x.getValue();
				resolution.y = resolution_y.getValue();

				//std::cout << (std::string)resolution_x.getText() << ' ' << (std::string)resolution_y.getText();

				renderer.close();
			}

			if (evnt.text.unicode == 59) //backspace - works better than ::Keyboard::isKeyPressed
			{
				for (int i = 0; i < textInputs.size(); i++)
					if (textInputs[i]->active == true && textInputs[i]->isEmptyString() == false)
						textInputs[i]->removeLastLetter();
			}

			if (evnt.text.unicode == 60) //tab to loop through the text inputs
			{
				for (int i = 0; i < textInputs.size(); i++)
					if (textInputs[i]->active == true)
					{
						textInputs[i]->active = false;
						textInputs[(i + 1) % textInputs.size()]->active = true;
						break;
					}
			}
			break;

		case sf::Event::MouseButtonPressed:
			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				for (int i = 0; i < textInputs.size(); i++)
					textInputs[i]->active = false;

				if (exit.isPressed(renderer))
				{
					renderer.close();
				}
				if (start.isPressed(renderer))
				{
					continueToken = true;
					renderer.close();
				}
				for (int i = 0; i < textInputs.size(); i++)
					textInputs[i]->isPresed(renderer);
			}

		}
		
		renderer.draw(exit);
		renderer.draw(start);
		renderer.draw(point);
		renderer.draw(rang);
		renderer.draw(resolution_x);
		renderer.draw(resolution_y);

		renderer.display();
	}

	if (continueToken)
	{

		sf::RenderWindow progress(sf::VideoMode(500, 200), "Wait for the processing to finish", sf::Style::Default & ~sf::Style::Resize);

		bool processed = false;

		textBox line0(sf::Vector2f(0, 50), sf::Vector2f(500, 50), "Please wait for image processing", "CC");
		textBox line1(sf::Vector2f(0, 0), sf::Vector2f(500, 50), "Image has been processed", "CC");
		textBox line2(sf::Vector2f(0, 50), sf::Vector2f(500, 50), "you can close this window", "CC");

		while (progress.isOpen())
		{

			sf::Event evnt;
			progress.pollEvent(evnt);

			switch (evnt.type)
			{
			case sf::Event::Closed:
				progress.close();
			}

			if (!processed)
			{
				progress.draw(line0);
				progress.display();

				std::cout << resolution.x << ' ' << resolution.y;

				sf::RenderTexture tex;
				tex.create(resolution.x, resolution.y);



				computeImage(center, range, resolution, tex, numThreads, 1);

				progress.draw(line1);
				progress.draw(line2);
				progress.display();

				std::string filename("outputs/");
				filename += "center_(";
				filename += std::to_string(center.x);
				filename += "_,_";
				filename += std::to_string(center.y);
				filename += ")_range_";
				filename += std::to_string(range);
				filename += ".png";
				tex.getTexture().copyToImage().saveToFile(filename);

				processed = true;
			}


			progress.draw(line1);
			progress.draw(line2);
			progress.display();
		}

	}

}