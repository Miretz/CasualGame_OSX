//
//  PlayState.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <memory>

#include "GameState.hpp"

class Player;
class PlayerInputManager;
class GLRaycaster;
class LevelReaderWriter;

class PlayState : public GameState
{
public:
    PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
    virtual ~PlayState();
    
    void update(const float ft) override;
    void draw(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event, const sf::Vector2f mousePosition, Game& game) override;
    
private:
    
    std::shared_ptr<Player> m_player;
    std::shared_ptr<LevelReaderWriter> m_levelReader;
    
    PlayerInputManager* m_inputManager;
    GLRaycaster* m_glRaycaster;
    
    double m_runningTime = 0.0;
    
    //Gui
    sf::Text m_fpsDisplay;
    sf::Text m_playerHealthDisplay;
    sf::RectangleShape m_gunDisplay;
    sf::CircleShape m_crosshair;
    sf::Texture m_textureGun;
    sf::Texture m_textureGun_fire;
    std::vector<sf::RectangleShape> m_minimapWallBuffer;
    std::vector<sf::CircleShape> m_minimapEntityBuffer;
    sf::RectangleShape m_minimapBackground;
    sf::ConvexShape m_minimapPlayer;
    
    void generateMinimap();
    void updateMinimapEntities();
    void drawMinimap(sf::RenderWindow* window) const;
    void drawGui(sf::RenderWindow* window);
    
    void destroyAimedAtSprite();
    void moveAimedAtSprite(const double fts);
    
};
