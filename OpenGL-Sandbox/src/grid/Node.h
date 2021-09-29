#pragma once

#include <glm/glm.hpp>
#include <GLCore.h>
#include <glad\glad.h>

/*
    D	N   C
    E		W
    A	S   B
*/
// A -> B -> C -> D

/* This cannot be a public class as any external class can
* runin the grid. Continuing with this implementation for 
* now for simplicity. Needs restructuring
*/

class Node
{
public:
    glm::vec3 edgePositions[4];	//position of each corner of the grid in world space
    float isoSurfaceVal[4];	//value of the function at this grid corner
    int cellID;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* above = nullptr;
    Node* below = nullptr;
};