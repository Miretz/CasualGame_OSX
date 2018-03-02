//
//  Game.cpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#include "Game.hpp"

#include "MainMenuState.hpp"
#include "PlayState.hpp"
#include "LevelEditorState.hpp"

#include "Player.hpp"
#include "LevelReaderWriter.hpp"
#include "Config.hpp"

Game::Game():
m_window(sf::VideoMode(g_defaultWidth, g_defaultHeight), g_gameTitle, sf::Style::Close)
{

    m_currentState = std::unique_ptr<GameState>(new MainMenuState(g_defaultWidth, g_defaultHeight));
    
    m_clock = std::unique_ptr<sf::Clock>(new sf::Clock());
    
    m_window.setFramerateLimit(500);
    
    m_levelReader = std::make_shared<LevelReaderWriter>();
    m_player = std::make_shared<Player>();
}

void Game::run()
{
    //Main Loop
    while (m_running)
    {
        update();
        draw();
        updateTimers();
        checkInput();
    }
    m_window.close();
}

void Game::checkInput()
{
    auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
    
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }
        else
        {
            m_currentState->handleInput(event, mousePosition, *this);
        }
    }
}

void Game::update()
{
    m_currentSlice += m_lastFt;
    for (; m_currentSlice >= 1; m_currentSlice -= 1)
    {
        m_currentState->update(1.0f);
    }
}

void Game::draw()
{
    m_currentState->draw(m_window);
}

void Game::updateTimers()
{
    m_lastFt = m_clock->restart().asMilliseconds();
    
    if (m_fpsShowTimer == 0)
    {
        m_fpsShowTimer = 2;
        auto ftSeconds = m_lastFt / 1000.f;
        if (ftSeconds > 0.f)
        {
            m_fps = static_cast<int>(1.f / ftSeconds);
        }
    }
    m_fpsShowTimer--;
}

void Game::changeState(GameStateName newState)
{
    
    const auto sizeX = m_window.getSize().x;
    const auto sizeY = m_window.getSize().y;
    
    switch (newState)
    {
        case GameStateName::MAINMENU:
            m_window.setMouseCursorVisible(true);
            m_currentState.reset(new MainMenuState(sizeX, sizeY));
            break;
        case GameStateName::PLAY:
            m_window.setMouseCursorVisible(false);
            m_currentState.reset(new PlayState(sizeX, sizeY, m_player, m_levelReader));
            break;
        case GameStateName::RESTART:
            m_window.setMouseCursorVisible(false);
            resetLevel();
            m_currentState.reset(new PlayState(sizeX, sizeY, m_player, m_levelReader));
            break;
        case GameStateName::LEVEL_EDITOR:
            m_window.setMouseCursorVisible(true);
            m_currentState.reset(new LevelEditorState(sizeX, sizeY, m_player, m_levelReader));
            break;
        case GameStateName::SWITCH_FULLSCREEN:
            switchFullscreen();
            m_currentState.reset(new MainMenuState(m_window.getSize().x, m_window.getSize().y));
            break;
        case GameStateName::QUIT:
            m_running = false;
            break;
        default:
            m_currentState.reset(new MainMenuState(sizeX, sizeY));
            break;
    }
}

void Game::resetLevel()
{
    //reset player position
    m_player->m_posX = 22.0;
    m_player->m_posY = 11.5;
    m_player->m_dirX = -1.0;
    m_player->m_dirY = 0.0;
    m_player->m_planeX = 0.0;
    m_player->m_planeY = 0.66;
    
    //reload level
    m_levelReader->loadDefaultLevel();

}

void Game::switchFullscreen()
{
    m_window.close();
    if (m_fullscreen)
    {
        m_window.create(sf::VideoMode(g_defaultWidth, g_defaultHeight), g_gameTitle, sf::Style::Close);
    }
    else
    {
        m_window.create(sf::VideoMode::getDesktopMode(), g_gameTitle, sf::Style::Fullscreen);
    }
    m_fullscreen = !m_fullscreen;
    m_window.setFramerateLimit(500);
    
}
