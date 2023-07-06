#include "Character.h"

Character::Character(const char* texturePath, Vector2 startPos)
{
    sprite = LoadTexture(texturePath); // load texture using the provided texture path
    position = startPos; // set the position for character according to current position
}

void Character::Draw()
{
    DrawTextureEx(sprite, position, 0.0f, 3.0f, WHITE); // render out the character sprite at the current position
}