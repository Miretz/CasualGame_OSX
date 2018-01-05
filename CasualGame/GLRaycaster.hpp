//
//  GLRaycaster.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Game;
class GLRenderer;
class Clickable;
class LevelReaderWriter;
class Player;

class GLRaycaster
{
public:
    GLRaycaster();
    virtual ~GLRaycaster();
    
    void initialize(const int windowWidth, const int windowHeight,
                    std::shared_ptr<Player> player,
                    std::shared_ptr<LevelReaderWriter> levelReader);

    void update();
    void draw(sf::RenderWindow& window);
    
    std::vector<Clickable>& getClickables() { return m_clickables; }
    
private:
    
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    
    sf::Texture m_screen;
    sf::Sprite m_screenSprite;
    
    std::shared_ptr<Player> m_player;
    
    double m_tempRayPosX = 0.0;
    double m_tempRayPosY = 0.0;
    double m_tempRayDirX = 0.0;
    double m_tempRayDirY = 0.0;
    
    std::shared_ptr<LevelReaderWriter> m_levelReader;
    size_t m_spriteSize;
    
    std::vector<double> m_ZBuffer;
    
    //arrays used to sort the sprites
    std::vector<int> m_spriteOrder;
    std::vector<double> m_spriteDistance;
    
    //main rendering buffer
    int m_bufferSize;
    std::vector<sf::Uint8> m_buffer;
    
    // buffer of clickable items in the view
    std::vector<Clickable> m_clickables;
    
    void calculateWalls();
    void calculateSprites();
    void setPixel(int x, int y, const sf::Uint32 colorRgba, int style);
    
};

