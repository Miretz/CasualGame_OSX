//
//  Utils.cpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#include "Utils.hpp"

#include <cmath>

const sf::Vector2f Utils::normalize(const sf::Vector2f& source)
{
    sf::Vector2f result(source.x, source.y);
    auto vecLen = length(result);
    if (vecLen != 0)
    {
        result.x = result.x / vecLen;
        result.y = result.y / vecLen;
    }
    return result;
}

float Utils::length(const sf::Vector2f& source)
{
    return std::sqrt((source.x * source.x) + (source.y * source.y));
}

//sort algorithm
void Utils::combSort(std::vector<int>& order, std::vector<double>& dist, int amount)
{
    
    int gap = amount;
    bool swapped = false;
    while (gap > 1 || swapped)
    {
        //shrink factor 1.3
        gap = (gap * 10) / 13;
        if (gap == 9 || gap == 10) gap = 11;
        if (gap < 1) gap = 1;
        swapped = false;
        for (int i = 0; i < amount - gap; i++)
        {
            int j = i + gap;
            if (dist[i] < dist[j])
            {
                std::swap(dist[i], dist[j]);
                std::swap(order[i], order[j]);
                swapped = true;
            }
        }
    }
}

//check string suffix
bool Utils::hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}
