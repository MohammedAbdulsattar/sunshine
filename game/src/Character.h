#ifndef CHARACTER_H
#define CHARACTER_H

//#include "rlImGui.h"
//#include "raymath.h"
#include "raylib.h"
//#include "Math.h"
#include <vector>

// Lab 4 - Homework Part 2
class Character
{
public:
    Texture2D sprite; // Texture to hold character sprite
    Vector2 position; // position vector for the character

    Character(const char* texturePath, Vector2 startPos);

    virtual void Draw(); // changed to virtual function so "Draw()" in Monster.h is allowed to override 
};

#endif // CHARACTER_H