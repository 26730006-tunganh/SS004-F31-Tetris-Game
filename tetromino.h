
#ifndef TETROMINO_H
#define TETROMINO_H

#include "block.h"

// ================= I BLOCK =================
class IBlock : public Block
{
public:
    IBlock() : Block('I')
    {
        shape[0][1] = 'I';
        shape[1][1] = 'I';
        shape[2][1] = 'I';
        shape[3][1] = 'I';
    }

    void rotate() override
    {
        rotateMatrix();
    }
};

// ================= O BLOCK =================
class OBlock : public Block
{
public:
    OBlock() : Block('O')
    {
        shape[1][1] = 'O';
        shape[1][2] = 'O';
        shape[2][1] = 'O';
        shape[2][2] = 'O';
    }

    void rotate() override
    {
        // Khối O không cần xoay
    }

    void getRotatedShape(char out[4][4]) const override
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                out[i][j] = shape[i][j];
    }
};

// ================= T BLOCK =================
class TBlock : public Block
{
public:
    TBlock() : Block('T')
    {
        shape[0][1] = 'T';
        shape[1][0] = 'T';
        shape[1][1] = 'T';
        shape[1][2] = 'T';
    }

    void rotate() override
    {
        rotateMatrix();
    }
};

// ================= S BLOCK =================
class SBlock : public Block
{
public:
    SBlock() : Block('S')
    {
        shape[0][1] = 'S';
        shape[0][2] = 'S';
        shape[1][0] = 'S';
        shape[1][1] = 'S';
    }

    void rotate() override
    {
        rotateMatrix();
    }
};

// ================= Z BLOCK =================
class ZBlock : public Block
{
public:
    ZBlock() : Block('Z')
    {
        shape[0][0] = 'Z';
        shape[0][1] = 'Z';
        shape[1][1] = 'Z';
        shape[1][2] = 'Z';
    }

    void rotate() override
    {
        rotateMatrix();
    }
};

// ================= J BLOCK =================
class JBlock : public Block
{
public:
    JBlock() : Block('J')
    {
        shape[0][0] = 'J';
        shape[1][0] = 'J';
        shape[1][1] = 'J';
        shape[1][2] = 'J';
    }

    void rotate() override
    {
        rotateMatrix();
    }
};

// ================= L BLOCK =================
class LBlock : public Block
{
public:
    LBlock() : Block('L')
    {
        shape[0][2] = 'L';
        shape[1][0] = 'L';
        shape[1][1] = 'L';
        shape[1][2] = 'L';
    }

    void rotate() override
    {
        rotateMatrix();
    }
};

#endif