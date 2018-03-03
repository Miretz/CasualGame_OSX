//
//  PlayerInputManager.cpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#include "PlayerInputManager.hpp"

#include "Game.hpp"
#include "Player.hpp"
#include "Config.hpp"

#include <cmath>


PlayerInputManager::PlayerInputManager() {}

void PlayerInputManager::handleInput(const sf::Event & event, const sf::Vector2f mousePosition, Game& game)
{
    
    if (event.type == sf::Event::MouseButtonPressed)
    {
        handleShot();
    }
    
    if (event.type == sf::Event::MouseMoved)
    {
        //handleMouselook(event, game.getWindow());
    }
    
    //escape go to main menu
    if (event.type == sf::Event::KeyPressed)
    {
        // handle controls
        if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A))
        {
            m_left = true;
        }
        else if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D))
        {
            m_right = true;
        }
        else if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W))
        {
            m_forward = true;
        }
        else if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S))
        {
            m_backward = true;
        }
    }
    
    if (event.type == sf::Event::KeyReleased)
    {
        // handle controls
        if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A))
        {
            m_left = false;
        }
        if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D))
        {
            m_right = false;
        }
        if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W))
        {
            m_forward = false;
        }
        if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S))
        {
            m_backward = false;
        }
        if ((event.key.code == sf::Keyboard::Space) || (event.key.code == sf::Keyboard::LControl))
        {
            handleShot();
        }
    }
}

void PlayerInputManager::updatePlayerMovement(const double fts, std::shared_ptr<Player> m_player, const std::vector<std::vector<int> >& m_levelRef)
{
    calculateShotTime(fts);
    
    //update position
    if (isMoving())
    {
        // convert ms to seconds
        double moveSpeed = fts * 5.0; //the constant value is in squares/second
        double rotSpeed = fts * g_lookSpeed;
        
        if (m_left)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = m_player->dirX;
            m_player->dirX = m_player->dirX * std::cos(rotSpeed) - m_player->dirY * std::sin(rotSpeed);
            m_player->dirY = oldDirX * std::sin(rotSpeed) + m_player->dirY * std::cos(rotSpeed);
            double oldPlaneX = m_player->planeX;
            m_player->planeX = m_player->planeX * std::cos(rotSpeed) - m_player->planeY * std::sin(rotSpeed);
            m_player->planeY = oldPlaneX * std::sin(rotSpeed) + m_player->planeY * std::cos(rotSpeed);
        }
        if (m_right)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = m_player->dirX;
            m_player->dirX = m_player->dirX * std::cos(-rotSpeed) - m_player->dirY * std::sin(-rotSpeed);
            m_player->dirY = oldDirX * std::sin(-rotSpeed) + m_player->dirY * std::cos(-rotSpeed);
            double oldPlaneX = m_player->planeX;
            m_player->planeX = m_player->planeX * std::cos(-rotSpeed) - m_player->planeY * std::sin(-rotSpeed);
            m_player->planeY = oldPlaneX * sin(-rotSpeed) + m_player->planeY * std::cos(-rotSpeed);
        }
        if (m_forward)
        {
            if (m_levelRef[int(m_player->x + m_player->dirX * moveSpeed)][int(m_player->y)] == 0)
                m_player->x += m_player->dirX * moveSpeed;
            if (m_levelRef[int(m_player->x)][int(m_player->y + m_player->dirY * moveSpeed)] == 0)
                m_player->y += m_player->dirY * moveSpeed;
        }
        if (m_backward)
        {
            if (m_levelRef[int(m_player->x - m_player->dirX * moveSpeed)][int(m_player->y)] == 0)
                m_player->x -= m_player->dirX * moveSpeed;
            if (m_levelRef[int(m_player->x)][int(m_player->y - m_player->dirY * moveSpeed)] == 0)
                m_player->y -= m_player->dirY * moveSpeed;
        }
        if (m_stepLeft)
        {
            // rotate vector in 90 ccw use Vector(-y, x)
            auto dirX = -m_player->dirY;
            auto dirY = m_player->dirX;
            
            if (m_levelRef[int(m_player->x + dirX * moveSpeed)][int(m_player->y)] == 0)
                m_player->x += dirX * moveSpeed;
            if (m_levelRef[int(m_player->x)][int(m_player->y + dirY * moveSpeed)] == 0)
                m_player->y += dirY * moveSpeed;
        }
        if (m_stepRight)
        {
            // rotate vector in 90 cw use Vector(y, -x)
            auto dirX = m_player->dirY;
            auto dirY = -m_player->dirX;
            
            if (m_levelRef[int(m_player->x + dirX * moveSpeed)][int(m_player->y)] == 0)
                m_player->x += dirX * moveSpeed;
            if (m_levelRef[int(m_player->x)][int(m_player->y + dirY * moveSpeed)] == 0)
                m_player->y += dirY * moveSpeed;
        }
        
        //stop rotating on mouselook
        //m_left = false;
        //m_right = false;
    }
}

void PlayerInputManager::calculateShotTime(double fts)
{
    if (m_shotTime < 0.0)
    {
        m_shooting = false;
    }
    else
    {
        m_shotTime -= fts;
    }
    if (m_gunShotDelay > 0.0)
    {
        m_gunShotDelay -= fts;
    }
}

void PlayerInputManager::handleShot()
{
    if (m_shotTime > 0.0 || m_gunShotDelay > 0.0)
    {
        m_shooting = false;
        return;
    }
    m_shotTime = g_gunShotTime;
    m_gunShotDelay = g_gunShotDelayTime;
    m_shooting = true;
}


void PlayerInputManager::handleMouselook(const sf::Event & event, const sf::RenderWindow& window)
{
    int mouseX = event.mouseMove.x;
    int mouseY = event.mouseMove.y;
    
    m_mouseDelta = mouseX - m_lastMouseX;
    m_lastMouseX = mouseX;
    
    int windowX = static_cast<int>(window.getSize().x);
    int windowY = static_cast<int>(window.getSize().y);
    
    //if mouse is out of screen, put it to the center
    if (mouseX <= m_mouseBorder || mouseX >= windowX - m_mouseBorder ||
        mouseY <= m_mouseBorder || mouseY >= windowY - m_mouseBorder)
    {
        auto centerX = windowX / 2;
        auto centerY = windowY / 2;
        sf::Mouse::setPosition(sf::Vector2i(centerX, centerY), window);
        m_lastMouseX = centerX;
    }
    
    if (m_mouseDelta < 0)
    {
        m_right = false;
        m_left = true;
    }
    else if (m_mouseDelta > 0)
    {
        m_left = false;
        m_right = true;
    }
    else
    {
        m_left = false;
        m_right = false;
    }
    
}
