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

    Character(const char* texturePath, Vector2 startPos)
    {
        sprite = LoadTexture(texturePath); // load texture using the provided texture path
        position = startPos; // set the position for character according to current position
    }

    void Draw()
    {
        DrawTextureEx(sprite, position, 0.0f, 3.0f, WHITE); // render out the character sprite at the current position
    }
};

#endif // CHARACTER_H