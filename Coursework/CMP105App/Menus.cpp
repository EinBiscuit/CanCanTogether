#include "Menus.h"

void Menus::beginDraw()
{
	window->clear(sf::Color(255, 255, 255));
}

void Menus::endDraw()
{
	window->display();
}
