#include "textbox.h"

textBox::textBox(const textBox& cpy)
{
	this->topLeft = cpy.topLeft;
	this->bottomRight = cpy.bottomRight;
	this->text = cpy.text;
	this->fnt = cpy.fnt;

	this->updateSprite();
}


textBox::textBox(sf::Vector2f position, sf::Vector2f size, std::string text, const char alignment[], bool debugMode)
{
	this->topLeft = position;
	this->bottomRight = position + size;

	this->fnt.loadFromFile("ARI.ttf");

	this->text.setString(text);
	this->text.setFont(fnt);

	this->texture.create(size.x, size.y);

	sf::FloatRect rect;
	rect = this->text.getGlobalBounds();

	sf::Vector2f originAlign;
	sf::Vector2f positionAlign(size.x / 2, size.y / 2);

	switch (alignment[0])
	{
	case 'L':
		originAlign.x = rect.left;
		positionAlign.x = 0.0f;
		break;
	case 'C':
		originAlign.x = rect.left + rect.width / 2;
		positionAlign.x = size.x / 2;
		break;
	case 'R':
		originAlign.x = rect.left + rect.width;
		positionAlign.x = size.x;
		break;
	default:
		originAlign.x = rect.left + rect.width / 2;
		positionAlign.x = size.x / 2;
		break;
	}

	switch (alignment[1])
	{
	case 'T':
		originAlign.y = rect.top;
		positionAlign.y = 0.0f;
		break;
	case 'C':
		originAlign.y = rect.top + rect.height / 2;
		positionAlign.y = size.y / 2;
		break;
	case 'B':
		originAlign.y = rect.top + rect.height;
		positionAlign.y = size.y;
		break;
	default:
		originAlign.y = rect.top + rect.height / 2;
		positionAlign.y = size.y / 2;
		break;
	}

	this->text.setOrigin(originAlign);
	this->text.setPosition(positionAlign);

	this->updateSprite();

}

void textBox::updateSprite()
{

	this->texture.clear();

	sf::Vector2f size(bottomRight - topLeft);
	sf::RectangleShape rim(size);

	rim.setFillColor(sf::Color(31, 31, 31, 0));
	rim.setOutlineColor(sf::Color(255, 255, 255, 255));
	rim.setOutlineThickness(-0.1f);

	texture.draw(rim);
	texture.draw(this->text);

	texture.display();
	sprt.setPosition(topLeft);
	sprt.setTexture(texture.getTexture());

}

void textBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprt);
}

void textBox::setText(std::string input)
{
	this->text.setString(input);
	this->updateSprite();
}

void textBox::setColor(sf::Color color)
{
	this->text.setFillColor(color);
	this->updateSprite();
}

const sf::Vector2f textBox::getPosition()
{
	return this->topLeft;
}

const sf::Vector2f textBox::getSize()
{
	return this->bottomRight - this->topLeft;
}

const std::string textBox::getText()
{
	return (std::string)this->text.getString();
}