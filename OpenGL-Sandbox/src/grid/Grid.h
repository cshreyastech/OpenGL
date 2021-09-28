#pragma once

#include "Node.h"

//#include <glm/glm.hpp>
//#include <GLCore.h>
//#include <glad\glad.h>

/*
    D	N   C
    E		W
    A	S   B
*/
// A -> B -> C -> D
//struct Node
//{
//    glm::vec3 edgePositions[4];	//position of each corner of the grid in world space
////float isoSurfaceVal_[4];	//value of the function at this grid corner
//
//    Node* left = nullptr;
//    Node* right = nullptr;
//    Node* above = nullptr;
//    Node* below = nullptr;
//};

class Grid
{
private:
    Grid(const float quad_size, const float lowestISOvalue, const float highestISOvalue);
    void ConstructGrid();


private:
    static Grid* grid_;

    const float TOPRIGHT = 10.0f;
    const float TOPLEFT = 10.0f;

    int nCells = 0;
    int nRows = 0;
    int nCols = 0;
    const float rows = 10.0f;
    const float cols = 10.0f;

    const float quad_size_ = 0.0f;
    const float highestISOvalue_ = 0.0f;
    const float lowestISOvalue_ = 0.0f;

    Node* head_ = nullptr;
    Node* tail_ = nullptr;

public:
    Grid(Grid& other) = delete;
    void operator=(const Grid&) = delete;

    static Grid* Instance(const float quad_size, const float lowestISOvalue, const float highestISOvalue);

    inline Node* Head() { return head_; }

    void Print();
    void CleanUp();

};

