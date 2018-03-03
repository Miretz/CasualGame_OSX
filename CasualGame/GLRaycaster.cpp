//
//  GLRaycaster.cpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#include "GLRaycaster.hpp"

#include "LevelReaderWriter.hpp"
#include "Player.hpp"
#include "Sprite.hpp"
#include "Clickable.hpp"
#include "Utils.hpp"
#include "Config.hpp"

#include <cmath>

GLRaycaster::GLRaycaster()
{
}

GLRaycaster::~GLRaycaster()
{
}

void GLRaycaster::initialize(
                             const int windowWidth, const int windowHeight,
                             std::shared_ptr<Player> player,
                             std::shared_ptr<LevelReaderWriter> levelReader)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    m_player = move(player);
    m_levelReader = move(levelReader);
    
    m_spriteSize = m_levelReader->getSprites().size();
    
    m_ZBuffer.resize(windowWidth);
    m_spriteOrder.resize(m_levelReader->getSprites().size());
    m_spriteDistance.resize(m_levelReader->getSprites().size());
    m_clickables.resize(m_levelReader->getSprites().size());
    
    m_bufferSize = windowHeight * windowWidth * 4;
    m_buffer.resize(m_bufferSize);
    
    m_screen.create(m_windowWidth, m_windowHeight);
    
}

void GLRaycaster::update()
{
    std::vector<sf::Uint8>().swap(m_buffer);
    m_buffer.resize(m_bufferSize);
    
    //calculate a new buffer
    calculateWalls();
    calculateSprites();
    
    m_screen.update(&m_buffer[0]);
    m_screenSprite.setTexture(m_screen);
}

void GLRaycaster::draw(sf::RenderWindow& window)
{
    window.draw(m_screenSprite);
}


void GLRaycaster::calculateWalls()
{
    const sf::Vector2<double> rayPos(m_player->x, m_player->y);
    
    auto& tex8 = m_levelReader->getTexture(8);//floor
    auto& tex9 = m_levelReader->getTexture(9);//ceiling
    
    for (int x = 0; x < m_windowWidth; x+=g_wallPixelSize)
    {
        
        //calculate ray position and direction
        const double cameraX = 2.0 * x / m_windowWidth - 1.0; //x-coordinate in camera space
        
        const sf::Vector2<double> rayDir(
            m_player->dirX + m_player->planeX * cameraX,
            m_player->dirY + m_player->planeY * cameraX
        );
        
        //length of ray from one x or y-side to next x or y-side
        const sf::Vector2<double> deltaDist(
            std::sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
            std::sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
        );
        
        //length of ray from current position to next x or y-side
        sf::Vector2<double> sideDist(0.0, 0.0);
        
        //what direction to step in x or y-direction (either +1 or -1)
        sf::Vector2i step(0, 0);
        
        //which box of the map we're in
        sf::Vector2i map(static_cast<int>(rayPos.x), static_cast<int>(rayPos.y));
        
        //calculate step and initial sideDist
        if (rayDir.x < 0)
        {
            step.x = -1;
            sideDist.x = (rayPos.x - map.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (map.x + 1.0 - rayPos.x) * deltaDist.x;
        }
        
        if (rayDir.y < 0)
        {
            step.y = -1;
            sideDist.y = (rayPos.y - map.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (map.y + 1.0 - rayPos.y) * deltaDist.y;
        }
        
        int side = 0; //was a NS or a EW wall hit?
        
        //perform DDA
        while (m_levelReader->getLevel()[map.x][map.y] <= 0)
        {
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                map.x += step.x;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                map.y += step.y;
                side = 1;
            }
        }
        
        //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
        double perpWallDist;
        double wallX; //where exactly the wall was hit
        if (side == 0)
        {
            perpWallDist = std::abs((map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x);
            wallX = rayPos.y + ((map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x) * rayDir.y;
        }
        else
        {
            perpWallDist = std::abs((map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y);
            wallX = rayPos.x + ((map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y) * rayDir.x;
        }
        wallX -= floor(wallX);
        
        
        //Calculate height of line to draw on screen
        const int lineHeight = static_cast<int>(std::abs(m_windowHeight / perpWallDist));
        
        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + m_windowHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + m_windowHeight / 2;
        if (drawEnd >= m_windowHeight) drawEnd = m_windowHeight - 1;
        
        //x coordinate on the texture
        int texX = static_cast<int>(wallX * g_textureWidth);
        if (side == 0 && rayDir.x > 0) texX = g_textureWidth - texX - 1;
        if (side == 1 && rayDir.y < 0) texX = g_textureWidth - texX - 1;
        
        const int texNum = m_levelReader->getLevel()[map.x][map.y] - 1; //1 subtracted from it so that texture 0 can be used!
        const std::vector<sf::Uint32>& texture = m_levelReader->getTexture(texNum);
        const int texSize = static_cast<int>(texture.size());
        
        for (int y = drawStart; y < drawEnd; y+=g_wallPixelSize)
        {
            
            int d = y * 256 - m_windowHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
            int texY = ((d * g_textureHeight) / lineHeight) / 256;
            int texNumY = g_textureHeight * texX + texY;
            
            if (texNumY < texSize)
            {
                auto color = texture[texNumY];
                setPixel(x, y, color, side, g_wallPixelSize);
            }
        }
        
        //SET THE ZBUFFER FOR THE SPRITE CASTING
        for(int a = 0; a < g_wallPixelSize; ++a)
        {
            m_ZBuffer[x + a] = perpWallDist; //perpendicular distance is used
        }
            
        //FLOOR CASTING
        double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall
        
        if (side == 0 && rayDir.x > 0)
        {
            floorXWall = map.x;
            floorYWall = map.y + wallX;
        }
        else if (side == 0 && rayDir.x < 0)
        {
            floorXWall = map.x + 1.0;
            floorYWall = map.y + wallX;
        }
        else if (side == 1 && rayDir.y > 0)
        {
            floorXWall = map.x + wallX;
            floorYWall = map.y;
        }
        else
        {
            floorXWall = map.x + wallX;
            floorYWall = map.y + 1.0;
        }
        
        if (drawEnd < 0) drawEnd = m_windowHeight - 1; //becomes < 0 when the integer overflows
        
        //draw the floor from drawEnd to the bottom of the screen
        for (int y = drawEnd + 1; y <= m_windowHeight; y+=g_wallPixelSize)
        {
            
            const double currentDist = m_windowHeight / (2.0 * y - m_windowHeight); //you could make a small lookup table for this instead
            const double weight = currentDist / perpWallDist;
            
            const double currentFloorX = weight * floorXWall + (1.0 - weight) * rayPos.x;
            const double currentFloorY = weight * floorYWall + (1.0 - weight) * rayPos.y;
            
            const int floorTexX = static_cast<int>(currentFloorX * g_textureWidth) % g_textureWidth;
            const int floorTexY = static_cast<int>(currentFloorY * g_textureHeight) % g_textureHeight;
            
            //floor textures
            sf::Uint32 color1 = tex8[g_textureWidth * floorTexY + floorTexX];
            sf::Uint32 color2 = tex9[g_textureWidth * floorTexY + floorTexX];
            
            setPixel(x, y, color1, 0, g_wallPixelSize);
            setPixel(x, m_windowHeight - y, color2, 0, g_wallPixelSize);
        }
    }
}


void GLRaycaster::calculateSprites()
{
    
    //SPRITE CASTING
    //sort sprites from far to close
    
    auto sprites = m_levelReader->getSprites();
    
    for (size_t i = 0; i < m_spriteSize; i++)
    {
        m_spriteOrder[i] = i;
        m_spriteDistance[i] = (
                               (m_player->x - sprites[i].x) * (m_player->x - sprites[i].x) +
                               (m_player->y - sprites[i].y) * (m_player->y - sprites[i].y)); //sqrt not taken, unneeded
    }
    Utils::combSort(m_spriteOrder, m_spriteDistance, sprites.size());
    
    //after sorting the sprites, do the projection and draw them
    for (size_t i = 0; i < sprites.size(); i++)
    {
        
        //translate sprite position to relative to camera
        const double spriteX = sprites[m_spriteOrder[i]].x - m_player->x;
        const double spriteY = sprites[m_spriteOrder[i]].y - m_player->y;
        
        const double invDet = 1.0 / (m_player->planeX * m_player->dirY - m_player->dirX * m_player->planeY); //required for correct matrix multiplication
        const double transformX = invDet * (m_player->dirY * spriteX - m_player->dirX * spriteY);
        const double transformY = invDet * (-m_player->planeY * spriteX + m_player->planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D
        const int spriteScreenX = int((m_windowWidth / 2) * (1 + transformX / transformY));
        
        //calculate height of the sprite on screen
        const int spriteHeight = abs(int(m_windowHeight / (transformY))); //using "transformY" instead of the real distance prevents fisheye
        
        //calculate lowest and highest pixel to fill in current stripe
        int drawStartY = -spriteHeight / 2 + m_windowHeight / 2;
        int drawEndY = spriteHeight / 2 + m_windowHeight / 2;
        
        //calculate width of the sprite
        int spriteWidth = spriteHeight;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        
        const int texNr = sprites[m_spriteOrder[i]].texture;
        const std::vector<sf::Uint32>& textureData = m_levelReader->getTexture(texNr);
        const int texSize = textureData.size();
        
        //setup clickables
        m_clickables[i].update(
                               sf::Vector2f(float(spriteWidth / 2.0f), float(spriteHeight)),
                               sf::Vector2f(float(drawStartX + spriteWidth / 4.0f), float(drawStartY)));
        m_clickables[i].setSpriteIndex(m_spriteOrder[i]);
        
        //limit drawstart and drawend
        if (drawStartY < 0) drawStartY = 0;
        if (drawEndY >= m_windowHeight) drawEndY = m_windowHeight - 1;
        if (drawStartX < 0) drawStartX = 0;
        if (drawEndX >= m_windowWidth) drawEndX = m_windowWidth - 1;
        
        //loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            
            const int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * g_textureWidth / spriteWidth) / 256;
            
            //the conditions in the if are:
            //1) it's in front of camera plane so you don't see things behind you
            //2) it's on the screen (left)
            //3) it's on the screen (right)
            //4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < m_windowWidth && transformY < m_ZBuffer[stripe])
            {
                
                //for every pixel of the current stripe
                for (int y = drawStartY; y < drawEndY; ++y)
                {
                    
                    m_clickables[i].setVisible(texNr != 12);
                    m_clickables[i].setDestructible(texNr != 12);
                    
                    const int d = (y) * 256 - m_windowHeight * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
                    const int texY = ((d * g_textureHeight) / spriteHeight) / 256;
                    const int texPix = g_textureWidth * texX + texY;
                    
                    // prevent exception when accessing tex pixel out of range
                    if (texPix < texSize)
                    {
                        sf::Uint32 color = textureData[texPix]; //get current color from the texture
                        
                        // black is invisible!!!
                        if ((color & 0x00FFFFFF) != 0)
                        {
                            setPixel(stripe, y, color, 0, 1);
                        }
                    }
                }
                
            }
        }
        
    }
}


void GLRaycaster::setPixel(int x, int y, const sf::Uint32 colorRgba, int style, int pixelSize)
{
    auto colors = (sf::Uint8*)&colorRgba;
    for(int i = 0; i < pixelSize; ++i)
    {
        for(int j = 0; j < pixelSize; ++j)
        {
            auto index = ((y+i) * m_windowWidth + (x+j)) * 4;
            if(index > m_bufferSize) return;
            
            if (style == g_playDrawDarkened)
            {
                m_buffer[index] = colors[0] / 2;
                m_buffer[index + 1] = colors[1] / 2;
                m_buffer[index + 2] = colors[2] / 2;
                m_buffer[index + 3] = colors[3] / 2;
            }
            else if (style == g_playhDrawHighlighted)
            {
                m_buffer[index] = std::min(colors[0] + 25, 255);
                m_buffer[index + 1] = std::min(colors[1] + 25, 255);
                m_buffer[index + 2] = std::min(colors[2] + 25, 255);
                m_buffer[index + 3] = std::min(colors[3] + 25, 255);
                
            }
            else
            {
                m_buffer[index] = colors[0];
                m_buffer[index + 1] = colors[1];
                m_buffer[index + 2] = colors[2];
                m_buffer[index + 3] = colors[3];
            }
        }
    }
    
}

