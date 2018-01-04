//
//  Config.hpp
//  CasualGame
//
//  Created by Miretz Dev on 04/01/2018.
//  Copyright © 2018 Miretz. All rights reserved.
//

#pragma once

#include <string>

#include "FontLoader.hpp"

static const auto g_gameTitle = "Casual Game by Miretz";

static const auto g_defaultWidth = 1024;
static const auto g_defaultHeight = 768;

static const int g_textureWidth = 128;
static const int g_textureHeight = 128;

//the constant value is in radians/second
static const double g_lookSpeed = 3.0; //TODO Make configurable in game

// Resources

static const FontLoader* g_fontLoader = new FontLoader("OtherF.ttf");

static const auto g_defaultLevelFile = "level1.txt";
static const auto g_defaultLevelSpriteFile = "level1_sprites.txt";

static const auto g_customLevelDirectory = "";

static const int g_textureCount = 13;

static const std::string g_textureFiles[13] = {
    "stonebricks.png",//1
    "concbase.png",//2
    "metalblocks.png",//3
    "concbricks.png",//4
    "concblocks.png",//5
    "redbricks.png",//6
    "metalwall.png",//7
    "redbricks2.png",//8
    "diagonal.png", //floor 9
    "stoneblocks.png", //ceiling 10
    "barrel.png",//11
    "pillar.png",//12
    "greenlight.png"//13
};

static const std::string g_gunSprite = "gun.png";
static const std::string g_gunSprite_fire = "gun_fire.png";

//Main menu

static const auto g_mainTxtTitle = "Casual Game";
static const auto g_mainTxtStartGame = "Start Game";
static const auto g_mainTxtRestartGame = "Restart Game";
static const auto g_mainTxtLevelEditor = "Level Editor";
static const auto g_mainTxtToggleFullscreen = "Toggle Fullscreen";
static const auto g_mainTxtQuit = "Quit";

// Level Editor

static const float g_editorPlayerArrowScale = 8.0f;
static const int g_editorMenuWidth = 230;

static const auto g_editorTxtSwitchMode = "Switch mode";
static const auto g_editorTxtLoadDefault = "Load Default";
static const auto g_editorTxtLoad = "Load";
static const auto g_editorTxtSave = "Save";
static const auto g_editorTxtQuit = "Back";
static const auto g_editorTxtTexture = "Texture";
static const auto g_editorTxtSprite = "Sprite";

static const auto g_editorTxtModeWall = "Wall Mode (LMB - place, RMB - delete)";
static const auto g_editorTxtModeEntity = "Entities Mode (LMB - Select/Move, RMB - place, Del - delete)";

// Play state

static const float g_playMinimapScale = 8.0f;
static const int g_playMinimapTransparency = 140;

static const double g_Pi = 3.141592653589793238463;
static const int g_playDrawDarkened = 1;
static const int g_playhDrawHighlighted = 2;

static const double g_gunShotTime = 0.15; //seconds
static const double g_gunShotDelayTime = 0.5; //seconds

