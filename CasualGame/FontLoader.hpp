//
//  FontLoader.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <SFML/Graphics.hpp>

class FontLoader
{
public:
    FontLoader(const char* path);
    virtual ~FontLoader() = default;
    
    const sf::Font& getFont() const { return m_font; }
    
private:
    sf::Font m_font;
};
