//
//  Game.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "GameState.hpp"

struct Player;
class LevelReaderWriter;

class Game
{
public:
    Game();
    virtual ~Game() = default;
    
    void run();
    void changeState(GameStateName newState);
    bool isRunning() const { return m_running; };
    void switchFullscreen();
    
    int getFps() const { return m_fps; }
    
    const sf::RenderWindow& getWindow() const { return m_window; }
    
private:
    
    int m_lastFt = 0;
    int m_currentSlice = 0;
    bool m_running = true;
    int m_fpsShowTimer = 0;
    bool m_fullscreen = false;
    int m_fps = 0;
    
    sf::RenderWindow m_window;
    std::unique_ptr<sf::Clock> m_clock;
    
    std::unique_ptr<GameState> m_currentState;
    
    std::shared_ptr<LevelReaderWriter> m_levelReader;
    std::shared_ptr<Player> m_player;
    
    void checkInput();
    void update();
    void draw();
    void resetLevel();
    void updateTimers();
    
};

