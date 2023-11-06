#pragma once
#include "Button.h"
class InputField :
    public Button
{
    bool active = false;
    bool only_integer;
    int input_limit;
    std::string startTag;

public:
    InputField(sf::Vector2f size, sf::Vector2f position, std::string start_tag, sf::Font* font, int input_limit = 5 , bool only_integer = true);
    void handleInput(Input* input, char input_txt);
    
    sf::Vector2f getSize() { return body.getSize(); };
    std::string inString = "INPUT HERE";
 
};

