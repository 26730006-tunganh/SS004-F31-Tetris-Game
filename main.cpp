#include "conio.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "windows.h"

#include "tetromino.h"

using namespace std;

#define H 20
#define W 20

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

char board[H][W] = {};
Block* currentBlock = nullptr;


// Tạo block ngẫu nhiên
Block* createBlock()
{
    int type = rand() % 7;

    switch (type)
    {
    case 0: return new IBlock();
    case 1: return new OBlock();
    case 2: return new TBlock();
    case 3: return new SBlock();
    case 4: return new ZBlock();
    case 5: return new JBlock();
    case 6: return new LBlock();
    }

    return nullptr;
}

// Kiểm tra di chuyển
bool canMove(int dx, int dy)
{
  if (currentBlock == nullptr)
    return false;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (currentBlock->getCell(i, j) != ' ')
      {
        int xt = currentBlock->getX() + j + dx;
        int yt = currentBlock->getY() + i + dy;

        if (xt < 1 || xt >= W - 1 ||
            yt < 0 || yt >= H - 1)
          return false;

        if (board[yt][xt] != ' ')
          return false;
      }
    }
  }

  return true;
}

// Kiểm tra vị trí sau khi xoay
bool canRotate()
{
  if (currentBlock == nullptr)
    return false;
  char temp[4][4];

  currentBlock->getRotatedShape(temp);

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (temp[i][j] != ' ')
        continue;
      int xt = currentBlock->getX() + j;
      int yt = currentBlock->getY() + i;

      if (xt < 1 || xt >= W - 1 || yt < 0 || yt >= H - 1)
        return false;

      if (board[yt][xt] != ' ')
        return false;
      
    }
  }

  return true;
}

// Xoay block thông qua đa hình
void rotateBlock()
{
  if (currentBlock == nullptr)
    return;
  if (canRotate())
    {
        currentBlock->rotate();
    }
}

// Ghi block vào board
void block2Board()
{
  if (currentBlock == nullptr)
    return;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      char cell = currentBlock->getCell(i, j);

      if (cell != ' ')
      {
        int xt = currentBlock->getX() + j;
        int yt = currentBlock->getY() + i;

        if (xt >= 0 && xt < W &&
            yt >= 0 && yt < H)
        {
          board[yt][xt] = cell;
        }
      }
    }
  }
}

// Xóa block đang rơi khỏi board
void boardDelBlock()
{
  if (currentBlock == nullptr)
    return;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (currentBlock->getCell(i, j) != ' ')
      {
        int xt = currentBlock->getX() + j;
        int yt = currentBlock->getY() + i;

        if (xt >= 1 && xt < W - 1 &&
            yt >= 0 && yt < H - 1)
        {
          board[yt][xt] = ' ';
        }
      }
    }
  }
}

// Khóa block hiện tại vào board
void lockBlock()
{
    if (currentBlock == nullptr)
        return;

    delete currentBlock;
    currentBlock = nullptr;
}

void initBoard()
{
  for (int i = 0; i < H; i++)
  {
    for (int j = 0; j < W; j++)
    {
      if (i == 0 || i == H - 1 || j == 0 || j == W - 1)
        board[i][j] = '#';
      else
        board[i][j] = ' ';
    }
  }
}

void draw()
{
  system("cls");

  // Viền trên
  SetConsoleTextAttribute(
      hConsole,
      FOREGROUND_RED |
          FOREGROUND_GREEN |
          FOREGROUND_BLUE);

  cout << "┌";

  for (int j = 0; j < W; j++)
    cout << "──";

  cout << "┐" << endl;

  // Board
  for (int i = 0; i < H; i++)
  {
    cout << "│";

    for (int j = 0; j < W; j++)
    {
      char cell = board[i][j];

      // Tường
      if (cell == '#')
      {
        cout << "  ";
      }

      // Ô trống
      else if (cell == ' ')
      {
        cout << "  ";
      }

      // Block
      else
      {
        switch (cell)
        {
        case 'I':
          cout << "\033[96m";
          break;

        case 'O':
          cout << "\033[93m";
          break;

        case 'T':
          cout << "\033[95m";
          break;

        case 'S':
          cout << "\033[92m";
          break;

        case 'Z':
          cout << "\033[91m";
          break;

        case 'J':
          cout << "\033[94m";
          break;

        case 'L':
          cout << "\033[38;5;208m";
          break;
        }

        cout << "██";

        cout << "\033[0m";
      }
    }

    cout << "│" << endl;
  }

  // Viền dưới
  cout << "└";

  for (int j = 0; j < W; j++)
    cout << "──";

  cout << "┘" << endl;

  cout << endl;

  cout << "A: Left   ";
  cout << "D: Right   ";
  cout << "X: Down   ";
  cout << "W: Rotate   ";
  cout << "Q: Quit";

  cout << endl;
}

void removeLine()
{
  for (int i = H - 2; i > 0; i--)
  {
    bool full = true;

    for (int j = 1; j < W - 1; j++)
    {
      if (board[i][j] == ' ')
      {
        full = false;
        break;
      }
    }

    if (full)
    {
      for (int ii = i; ii > 1; ii--)
      {
        for (int jj = 1; jj < W - 1; jj++)
        {
          board[ii][jj] = board[ii - 1][jj];
        }
      }

      for (int jj = 1; jj < W - 1; jj++)
      {
        board[1][jj] = ' ';
      }

      i++;
    }
  }
}

int main()
{
  srand((unsigned int)time(0));

  int dropSpeed = 500;

  

  initBoard();

  currentBlock = createBlock();

  while (1)
  {
    boardDelBlock();

    if (kbhit())
    {
      char c = _getch();

      if (c == 'a' && canMove(-1, 0))
        currentBlock->move(-1, 0);

      if (c == 'd' && canMove(1, 0))
        currentBlock->move(1, 0);

      if (c == 'x' && canMove(0, 1))
        currentBlock->move(0, 1);

      if (c == 'w')
        rotateBlock();

      if (c == 'q')
        break;
    }

    if (canMove(0, 1))
    {
      currentBlock->move(0, 1);
    }
    else
    {
      block2Board();
      lockBlock();
      removeLine();

      if (dropSpeed > 100)
        dropSpeed -= 20;

      currentBlock = createBlock();

      if (!canMove(0, 0))
      {
        cout << "GAME OVER!" << endl;
        break;
      }

    }

    block2Board();

    draw();

    Sleep(dropSpeed);
  }
  delete currentBlock;
  currentBlock = nullptr;

  system("pause");

  return 0;
}