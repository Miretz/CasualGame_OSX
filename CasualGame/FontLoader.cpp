//
//  FontLoader.cpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#include "FontLoader.hpp"
#include "ResourcePath.hpp"

FontLoader::FontLoader(const char* path)
{
    m_font.loadFromFile(resourcePath() + path);
}
