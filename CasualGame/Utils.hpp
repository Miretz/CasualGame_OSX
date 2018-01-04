//
//  Utils.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <SFML/Graphics.hpp>

class Utils
{
public:
    static const sf::Vector2f normalize(const sf::Vector2f& source);
    static float length(const sf::Vector2f& source);
    static void combSort(std::vector<int>& order, std::vector<double>& dist, int amount);
    static bool hasEnding (std::string const &fullString, std::string const &ending);
};
