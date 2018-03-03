//
//  Player.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once
struct Player
{
    double x = 22.0;
    double y = 11.5;
    
    double dirX = -1.0;
    double dirY = 0.0;
    
    double planeX = 0.0;
    double planeY = 0.66;
    
    int health = 100;
};
