#pragma once

#include "Node.h"

class Grid
{
private:
    Grid(const float quad_size, const float lowestISOvalue, const float highestISOvalue);
    void ConstructGrid();

    float GetRandomIsoValueInRange(const float low, const float high);
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
    inline Node* Tail() { return tail_; }

    void GenerateIsoValues(const float lowestISOvalue, const float highestISOvalue);
    void PrintFromHead();
    void PrintFromTail();
    void CleanUp();

};

