#include "Collision.h"

//Checks if two rectangles are overlapping with one another 
bool Collision::CheckRectangleCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB)
{
    return (
        rectangleA.x + rectangleA.w >= rectangleB.x &&
        rectangleB.x + rectangleB.w >= rectangleA.x &&
        rectangleA.y + rectangleA.h >= rectangleB.y &&
        rectangleB.y + rectangleB.h >= rectangleA.y
        );
}
