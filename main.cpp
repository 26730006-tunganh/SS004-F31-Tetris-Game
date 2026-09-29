#include "conio.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "windows.h"

using namespace std;

#define H 20
#define W 20

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

char board[H][W] = {};

int x, y, b;

char blocks[7][4][4] =
    {
        {{' ', 'I', ' ', ' '},
         {' ', 'I', ' ', ' '},
         {' ', 'I', ' ', ' '},
         {' ', 'I', ' ', ' '}},

        {{' ', ' ', ' ', ' '},
         {' ', 'O', 'O', ' '},
         {' ', 'O', 'O', ' '},
         {' ', ' ', ' ', ' '}},

        {{' ', 'T', ' ', ' '},
         {'T', 'T', 'T', ' '},
         {' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' '}},

        {{' ', 'S', 'S', ' '},
         {'S', 'S', ' ', ' '},
         {' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' '}},

        {{'Z', 'Z', ' ', ' '},
         {' ', 'Z', 'Z', ' '},
         {' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' '}},

        {{'J', ' ', ' ', ' '},
         {'J', 'J', 'J', ' '},
         {' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' '}},

        {{' ', ' ', 'L', ' '},
         {'L', 'L', 'L', ' '},
         {' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' '}}};

bool canMove(int dx, int dy)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (blocks[b][i][j] != ' ')
      {
        int xt = x + j + dx;
        int yt = y + i + dy;

        if (xt < 1 || xt >= W - 1 || yt >= H - 1)
          return false;

        if (board[yt][xt] != ' ')
          return false;
      }
    }
  }

  return true;
}

bool canRotate()
{
  char temp[4][4];

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      temp[j][3 - i] = blocks[b][i][j];
    }
  }

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (temp[i][j] != ' ')
      {
        int xt = x + j;
        int yt = y + i;

        if (xt < 1 || xt >= W - 1 || yt < 0 || yt >= H - 1)
          return false;

        if (board[yt][xt] != ' ')
          return false;
      }
    }
  }

  return true;
}

void rotateBlock()
{
  if (!canRotate())
    return;

  char temp[4][4];

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      temp[j][3 - i] = blocks[b][i][j];
    }
  }

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      blocks[b][i][j] = temp[i][j];
    }
  }
}

void block2Board()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (blocks[b][i][j] != ' ')
      {
        board[y + i][x + j] = blocks[b][i][j];
      }
    }
  }
}

void boardDelBlock()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (blocks[b][i][j] != ' ')
      {
        board[y + i][x + j] = ' ';
      }
    }
  }
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
  system("clear");

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

  x = 5;
  y = 0;
  b = rand() % 7;

  initBoard();

  while (1)
  {
    boardDelBlock();

    if (kbhit())
    {
      char c = getch();

      if (c == 'a' && canMove(-1, 0))
        x--;

      if (c == 'd' && canMove(1, 0))
        x++;

      if (c == 'x' && canMove(0, 1))
        y++;

      if (c == 'w')
        rotateBlock();

      if (c == 'q')
        break;
    }

    if (canMove(0, 1))
    {
      y++;
    }
    else
    {
      block2Board();

      removeLine();

      if (dropSpeed > 100)
        dropSpeed -= 20;

      x = 5;
      y = 0;
      b = rand() % 7;
    }

    block2Board();

    draw();

    Sleep(dropSpeed);
  }

  return 0;
}