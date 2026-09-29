
#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
using namespace std;

class Block
{
protected:
    char shape[4][4];
    int x, y;
    char symbol;

    // Hàm hỗ trợ xoay ma trận 90 độ theo chiều kim đồng hồ
    void rotateMatrix()
    {
        char temp[4][4];

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                shape[i][j] = temp[i][j];
            }
        }
    }

public:
    Block(char s, int startX = 5, int startY = 0)
        : x(startX), y(startY), symbol(s)
    {
        // Khởi tạo ma trận rỗng
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                shape[i][j] = ' ';
            }
        }
    }

    virtual ~Block() = default;

    // Hàm thuần ảo: bắt buộc lớp con triển khai
    virtual void rotate() = 0;

    // Lấy dữ liệu block
    char getCell(int row, int col) const
    {
        return shape[row][col];
    }

    int getX() const { return x; }
    int getY() const { return y; }

    void setPosition(int newX, int newY)
    {
        x = newX;
        y = newY;
    }

    char getSymbol() const { return symbol; }

    void move(int dx, int dy)
    {
        x += dx;
        y += dy;
    }

    // Dùng khi kiểm tra xoay trước khi cập nhật ma trận
    virtual void getRotatedShape(char out[4][4]) const
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                out[j][3 - i] = shape[i][j];
            }
        }
    }

    void applyRotation()
    {
        rotateMatrix();
    }
};

#endif