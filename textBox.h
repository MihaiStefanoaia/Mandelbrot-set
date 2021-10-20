#pragma once
#include <SFML/Graphics.hpp>

class textBox : public sf::Drawable
{
private:
	sf::Vector2f topLeft, bottomRight;
	sf::RenderTexture texture;
	sf::Sprite sprt;
	sf::Text text;
	sf::Font fnt;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateSprite();

public:
	textBox(const textBox& cpy);
	textBox(sf::Vector2f position = sf::Vector2f(0,0), sf::Vector2f size = sf::Vector2f(0,0), std::string text = "", const char alignment[] = "CC", bool debugMode = 0);
	void setText(std::string input);
	void setColor(sf::Color color);

	const sf::Vector2f getPosition();
	const sf::Vector2f getSize();
	const std::string getText();


};
