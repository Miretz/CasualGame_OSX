//
//  Clickable.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <SFML/Graphics.hpp>

class Clickable
{
public:
    Clickable();
    virtual ~Clickable() = default;
    
    void update(const sf::Vector2f& size, const sf::Vector2f& position);
    void draw(sf::RenderWindow* window);
    bool containsVector(const sf::Vector2f& position) const;
    bool isVisible() const { return m_visible; };
    void setVisible(bool visible = true) { m_visible = visible; }
    
    void setSpriteIndex(int index) { m_spriteIndex = index; }
    int getSpriteIndex() const { return m_spriteIndex; }
    
    void setDestructible(bool destructible) { m_destructible = destructible; }
    bool getDestructible() const { return m_destructible; }
    
private:
    sf::RectangleShape m_shape;
    bool m_visible;
    bool m_destructible;
    int m_spriteIndex;
};

