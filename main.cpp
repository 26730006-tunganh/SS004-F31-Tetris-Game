#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>

using namespace std;

// ============================================================
// TETRIS CONFIGURATION
// ============================================================

const int BOARD_WIDTH  = 10;
const int BOARD_HEIGHT = 20;

const int SCREEN_WIDTH  = 45;
const int SCREEN_HEIGHT = 25;

// ============================================================
// COLORS
// ============================================================

enum Color
{
    BLACK        = 0,
    BLUE         = 1,
    GREEN        = 2,
    CYAN         = 3,
    RED          = 4,
    MAGENTA      = 5,
    YELLOW       = 6,
    WHITE        = 7,
    GRAY         = 8,
    LIGHT_BLUE   = 9,
    LIGHT_GREEN  = 10,
    LIGHT_CYAN   = 11,
    LIGHT_RED    = 12,
    LIGHT_MAGENTA= 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// ============================================================
// PIECE TYPES
// ============================================================

enum PieceType
{
    I = 0,
    O = 1,
    T = 2,
    S = 3,
    Z = 4,
    J = 5,
    L = 6
};

// ============================================================
// PIECE
// ============================================================

struct Piece
{
    PieceType type;

    char shape[4][4];

    int x;
    int y;
};

// ============================================================
// COLORS FOR PIECES
// ============================================================

int getPieceColor(PieceType type)
{
    switch (type)
    {
        case I: return LIGHT_CYAN;
        case O: return LIGHT_YELLOW;
        case T: return LIGHT_MAGENTA;
        case S: return LIGHT_GREEN;
        case Z: return LIGHT_RED;
        case J: return LIGHT_BLUE;
        case L: return YELLOW;
    }

    return WHITE;
}

// ============================================================
// ORIGINAL SHAPES
// ============================================================

const char SHAPES[7][4][4] =
{
    // ========================================================
    // I
    // ========================================================
    {
        {' ', 'I', ' ', ' '},
        {' ', 'I', ' ', ' '},
        {' ', 'I', ' ', ' '},
        {' ', 'I', ' ', ' '}
    },

    // ========================================================
    // O
    // ========================================================
    {
        {' ', 'O', 'O', ' '},
        {' ', 'O', 'O', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },

    // ========================================================
    // T
    // ========================================================
    {
        {' ', 'T', ' ', ' '},
        {'T', 'T', 'T', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },

    // ========================================================
    // S
    // ========================================================
    {
        {' ', 'S', 'S', ' '},
        {'S', 'S', ' ', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },

    // ========================================================
    // Z
    // ========================================================
    {
        {'Z', 'Z', ' ', ' '},
        {' ', 'Z', 'Z', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },

    // ========================================================
    // J
    // ========================================================
    {
        {'J', ' ', ' ', ' '},
        {'J', 'J', 'J', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    },

    // ========================================================
    // L
    // ========================================================
    {
        {' ', ' ', 'L', ' '},
        {'L', 'L', 'L', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    }
};

// ============================================================
// GLOBAL RANDOM
// ============================================================

mt19937 rng(
    static_cast<unsigned int>(
        chrono::steady_clock::now()
        .time_since_epoch()
        .count()
    )
);

// ============================================================
// TETRIS CLASS
// ============================================================

class Tetris
{
private:

    // --------------------------------------------------------
    // BOARD
    // --------------------------------------------------------

    char board[BOARD_HEIGHT][BOARD_WIDTH];

    // --------------------------------------------------------
    // CURRENT / NEXT / HOLD
    // --------------------------------------------------------

    Piece current;
    Piece next;

    int holdType;

    bool canHold;

    // --------------------------------------------------------
    // BAG
    // --------------------------------------------------------

    vector<int> bag;

    // --------------------------------------------------------
    // GAME STATE
    // --------------------------------------------------------

    bool gameOver;

    // --------------------------------------------------------
    // SCORE
    // --------------------------------------------------------

    int score;
    int lines;
    int level;

    // --------------------------------------------------------
    // TIMING
    // --------------------------------------------------------

    chrono::steady_clock::time_point lastFall;

public:

    Tetris()
    {
        initializeGame();
    }

    // ========================================================
    // INITIALIZE GAME
    // ========================================================

    void initializeGame()
    {
        score = 0;
        lines = 0;
        level = 1;

        holdType = -1;
        canHold = true;

        gameOver = false;

        bag.clear();

        initializeBoard();

        current = createPiece(getNextPiece());
        next = createPiece(getNextPiece());

        lastFall = chrono::steady_clock::now();

        hideCursor();

        draw();
    }

    // ========================================================
    // INITIALIZE BOARD
    // ========================================================

    void initializeBoard()
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                board[y][x] = ' ';
            }
        }
    }

    // ========================================================
    // HIDE CURSOR
    // ========================================================

    void hideCursor()
    {
        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(
            hConsole,
            &cursorInfo
        );

        cursorInfo.bVisible = FALSE;

        SetConsoleCursorInfo(
            hConsole,
            &cursorInfo
        );
    }

    // ========================================================
    // SHOW CURSOR
    // ========================================================

    void showCursor()
    {
        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(
            hConsole,
            &cursorInfo
        );

        cursorInfo.bVisible = TRUE;

        SetConsoleCursorInfo(
            hConsole,
            &cursorInfo
        );
    }

    // ========================================================
    // MOVE CURSOR
    // ========================================================

    void gotoXY(int x, int y)
    {
        COORD position;

        position.X = static_cast<SHORT>(x);
        position.Y = static_cast<SHORT>(y);

        SetConsoleCursorPosition(
            hConsole,
            position
        );
    }

    // ========================================================
    // SET COLOR
    // ========================================================

    void setColor(int color)
    {
        SetConsoleTextAttribute(
            hConsole,
            color
        );
    }

    // ========================================================
    // RESET COLOR
    // ========================================================

    void resetColor()
    {
        setColor(BRIGHT_WHITE);
    }

    // ========================================================
    // 7-BAG RANDOMIZER
    // ========================================================

    int getNextPiece()
    {
        if (bag.empty())
        {
            bag =
            {
                I,
                O,
                T,
                S,
                Z,
                J,
                L
            };

            shuffle(
                bag.begin(),
                bag.end(),
                rng
            );
        }

        int piece = bag.back();

        bag.pop_back();

        return piece;
    }

    // ========================================================
    // CREATE PIECE
    // ========================================================

    Piece createPiece(int type)
    {
        Piece piece;

        piece.type =
            static_cast<PieceType>(type);

        piece.x = BOARD_WIDTH / 2 - 2;
        piece.y = 0;

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                piece.shape[y][x] =
                    SHAPES[type][y][x];
            }
        }

        return piece;
    }

    // ========================================================
    // CHECK COLLISION
    // ========================================================

    bool canPlace(
        const Piece& piece,
        int newX,
        int newY,
        const char shape[4][4]
    )
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (shape[y][x] == ' ')
                    continue;

                int boardX = newX + x;
                int boardY = newY + y;

                // Left / right wall
                if (boardX < 0 ||
                    boardX >= BOARD_WIDTH)
                {
                    return false;
                }

                // Bottom
                if (boardY >= BOARD_HEIGHT)
                {
                    return false;
                }

                // Above board is allowed
                if (boardY < 0)
                    continue;

                // Existing block
                if (board[boardY][boardX] != ' ')
                {
                    return false;
                }
            }
        }

        return true;
    }

    // ========================================================
    // CHECK CURRENT PIECE
    // ========================================================

    bool canPlaceCurrent(
        int newX,
        int newY
    )
    {
        return canPlace(
            current,
            newX,
            newY,
            current.shape
        );
    }

    // ========================================================
    // MOVE LEFT
    // ========================================================

    void moveLeft()
    {
        if (canPlaceCurrent(
                current.x - 1,
                current.y))
        {
            current.x--;
        }
    }

    // ========================================================
    // MOVE RIGHT
    // ========================================================

    void moveRight()
    {
        if (canPlaceCurrent(
                current.x + 1,
                current.y))
        {
            current.x++;
        }
    }

    // ========================================================
    // SOFT DROP
    // ========================================================

    void softDrop()
    {
        if (canPlaceCurrent(
                current.x,
                current.y + 1))
        {
            current.y++;

            score += 1;
        }
        else
        {
            lockPiece();
        }
    }

    // ========================================================
    // HARD DROP
    // ========================================================

    void hardDrop()
    {
        int distance = 0;

        while (canPlaceCurrent(
                current.x,
                current.y + 1))
        {
            current.y++;

            distance++;
        }

        score += distance * 2;

        lockPiece();
    }

    // ========================================================
    // ROTATE PIECE
    // ========================================================

    void rotate()
    {
        // O does not need rotation
        if (current.type == O)
            return;

        char rotated[4][4];

        // Clockwise rotation
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                rotated[x][3 - y] =
                    current.shape[y][x];
            }
        }

        // ----------------------------------------------------
        // Simple Wall Kick
        // ----------------------------------------------------

        const int kicks[] =
        {
            0,
            -1,
            1,
            -2,
            2
        };

        for (int kick : kicks)
        {
            if (canPlace(
                    current,
                    current.x + kick,
                    current.y,
                    rotated))
            {
                current.x += kick;

                for (int y = 0; y < 4; y++)
                {
                    for (int x = 0; x < 4; x++)
                    {
                        current.shape[y][x] =
                            rotated[y][x];
                    }
                }

                return;
            }
        }
    }

    // ========================================================
    // GET GHOST Y
    // ========================================================

    int getGhostY()
    {
        int ghostY = current.y;

        while (canPlaceCurrent(
                current.x,
                ghostY + 1))
        {
            ghostY++;
        }

        return ghostY;
    }

    // ========================================================
    // LOCK CURRENT PIECE
    // ========================================================

    void lockPiece()
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (current.shape[y][x] == ' ')
                    continue;

                int boardX =
                    current.x + x;

                int boardY =
                    current.y + y;

                if (boardY >= 0 &&
                    boardY < BOARD_HEIGHT &&
                    boardX >= 0 &&
                    boardX < BOARD_WIDTH)
                {
                    board[boardY][boardX] =
                        current.shape[y][x];
                }
            }
        }

        // ----------------------------------------------------
        // Clear lines
        // ----------------------------------------------------

        int cleared = clearLines();

        updateScore(cleared);

        // ----------------------------------------------------
        // Spawn next
        // ----------------------------------------------------

        current = next;

        current.x =
            BOARD_WIDTH / 2 - 2;

        current.y = 0;

        next =
            createPiece(getNextPiece());

        canHold = true;

        // ----------------------------------------------------
        // Game Over
        // ----------------------------------------------------

        if (!canPlaceCurrent(
                current.x,
                current.y))
        {
            gameOver = true;
        }

        lastFall =
            chrono::steady_clock::now();
    }

    // ========================================================
    // CLEAR LINES
    // ========================================================

    int clearLines()
    {
        int cleared = 0;

        for (int y = BOARD_HEIGHT - 1;
             y >= 0;
             y--)
        {
            bool full = true;

            for (int x = 0;
                 x < BOARD_WIDTH;
                 x++)
            {
                if (board[y][x] == ' ')
                {
                    full = false;
                    break;
                }
            }

            if (full)
            {
                cleared++;

                // Move everything above down
                for (int row = y;
                     row > 0;
                     row--)
                {
                    for (int x = 0;
                         x < BOARD_WIDTH;
                         x++)
                    {
                        board[row][x] =
                            board[row - 1][x];
                    }
                }

                // Empty top row
                for (int x = 0;
                     x < BOARD_WIDTH;
                     x++)
                {
                    board[0][x] = ' ';
                }

                // Check same row again
                y++;
            }
        }

        return cleared;
    }

    // ========================================================
    // SCORE
    // ========================================================

    void updateScore(int cleared)
    {
        switch (cleared)
        {
            case 1:
                score += 100 * level;
                break;

            case 2:
                score += 300 * level;
                break;

            case 3:
                score += 500 * level;
                break;

            case 4:
                score += 800 * level;
                break;
        }

        lines += cleared;

        level =
            lines / 10 + 1;
    }

    // ========================================================
    // HOLD
    // ========================================================

    void hold()
    {
        if (!canHold)
            return;

        canHold = false;

        if (holdType == -1)
        {
            holdType =
                static_cast<int>(
                    current.type
                );

            current = next;

            current.x =
                BOARD_WIDTH / 2 - 2;

            current.y = 0;

            next =
                createPiece(getNextPiece());
        }
        else
        {
            int temp = holdType;

            holdType =
                static_cast<int>(
                    current.type
                );

            current =
                createPiece(temp);
        }

        current.x =
            BOARD_WIDTH / 2 - 2;

        current.y = 0;
    }

    // ========================================================
    // FALL SPEED
    // ========================================================

    int getFallDelay()
    {
        int delay =
            600 - (level - 1) * 50;

        if (delay < 50)
            delay = 50;

        return delay;
    }

    // ========================================================
    // AUTOMATIC FALL
    // ========================================================

    void update()
    {
        auto now =
            chrono::steady_clock::now();

        auto elapsed =
            chrono::duration_cast<
                chrono::milliseconds
            >(now - lastFall).count();

        if (elapsed >= getFallDelay())
        {
            if (canPlaceCurrent(
                    current.x,
                    current.y + 1))
            {
                current.y++;
            }
            else
            {
                lockPiece();
            }

            lastFall = now;
        }
    }

    // ========================================================
    // DRAW BLOCK
    // ========================================================

    void drawBlock(
        int screenX,
        int screenY,
        char block,
        int color
    )
    {
        gotoXY(
            screenX,
            screenY
        );

        setColor(color);

        cout << "[]";

        resetColor();
    }

    // ========================================================
    // DRAW GHOST BLOCK
    // ========================================================

    void drawGhostBlock(
        int screenX,
        int screenY
    )
    {
        gotoXY(
            screenX,
            screenY
        );

        setColor(GRAY);

        cout << "..";

        resetColor();
    }

    // ========================================================
    // DRAW BOARD
    // ========================================================

    void drawBoard()
    {
        // Top border
        gotoXY(0, 0);

        setColor(BRIGHT_WHITE);

        cout << "+";

        for (int x = 0;
             x < BOARD_WIDTH;
             x++)
        {
            cout << "--";
        }

        cout << "+";

        // Board
        for (int y = 0;
             y < BOARD_HEIGHT;
             y++)
        {
            gotoXY(
                0,
                y + 1
            );

            cout << "|";

            for (int x = 0;
                 x < BOARD_WIDTH;
                 x++)
            {
                if (board[y][x] == ' ')
                {
                    cout << "  ";
                }
                else
                {
                    setColor(
                        getPieceColor(
                            getPieceType(
                                board[y][x]
                            )
                        )
                    );

                    cout << "[]";

                    resetColor();
                }
            }

            cout << "|";
        }

        // Bottom border
        gotoXY(0, BOARD_HEIGHT + 1);

        cout << "+";

        for (int x = 0;
             x < BOARD_WIDTH;
             x++)
        {
            cout << "--";
        }

        cout << "+";
    }

    // ========================================================
    // GET PIECE TYPE FROM CHARACTER
    // ========================================================

    PieceType getPieceType(char c)
    {
        switch (c)
        {
            case 'I': return I;
            case 'O': return O;
            case 'T': return T;
            case 'S': return S;
            case 'Z': return Z;
            case 'J': return J;
            case 'L': return L;
        }

        return I;
    }

    // ========================================================
    // DRAW CURRENT PIECE
    // ========================================================

    void drawCurrentPiece()
    {
        // ----------------------------------------------------
        // Ghost
        // ----------------------------------------------------

        int ghostY =
            getGhostY();

        for (int y = 0;
             y < 4;
             y++)
        {
            for (int x = 0;
                 x < 4;
                 x++)
            {
                if (current.shape[y][x] == ' ')
                    continue;

                int gx =
                    current.x + x;

                int gy =
                    ghostY + y;

                if (gy < 0)
                    continue;

                if (gx < 0 ||
                    gx >= BOARD_WIDTH ||
                    gy >= BOARD_HEIGHT)
                    continue;

                // Don't overwrite current piece
                if (ghostY == current.y)
                    continue;

                gotoXY(
                    1 + gx * 2,
                    2 + gy
                );

                drawGhostBlock(
                    1 + gx * 2,
                    2 + gy
                );
            }
        }

        // ----------------------------------------------------
        // Current
        // ----------------------------------------------------

        for (int y = 0;
             y < 4;
             y++)
        {
            for (int x = 0;
                 x < 4;
                 x++)
            {
                if (current.shape[y][x] == ' ')
                    continue;

                int bx =
                    current.x + x;

                int by =
                    current.y + y;

                if (bx < 0 ||
                    bx >= BOARD_WIDTH ||
                    by < 0 ||
                    by >= BOARD_HEIGHT)
                    continue;

                drawBlock(
                    1 + bx * 2,
                    2 + by,
                    current.shape[y][x],
                    getPieceColor(
                        current.type
                    )
                );
            }
        }
    }

    // ========================================================
    // DRAW SMALL PIECE
    // ========================================================

    void drawPreview(
        const Piece& piece,
        int startX,
        int startY
    )
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                gotoXY(
                    startX + x * 2,
                    startY + y
                );

                if (piece.shape[y][x] != ' ')
                {
                    setColor(
                        getPieceColor(
                            piece.type
                        )
                    );

                    cout << "[]";

                    resetColor();
                }
                else
                {
                    cout << "  ";
                }
            }
        }
    }

    // ========================================================
    // DRAW SIDE PANEL
    // ========================================================

    void drawSidePanel()
    {
        int x = 25;

        setColor(BRIGHT_WHITE);

        gotoXY(x, 1);

        cout << "================";

        gotoXY(x, 2);

        cout << "     TETRIS";

        gotoXY(x, 3);

        cout << "================";

        // ----------------------------------------------------
        // NEXT
        // ----------------------------------------------------

        gotoXY(x, 5);

        cout << "NEXT";

        drawPreview(
            next,
            x,
            6
        );

        // ----------------------------------------------------
        // HOLD
        // ----------------------------------------------------

        gotoXY(x, 11);

        cout << "HOLD";

        if (holdType != -1)
        {
            Piece hold =
                createPiece(
                    holdType
                );

            drawPreview(
                hold,
                x,
                12
            );
        }

        // ----------------------------------------------------
        // SCORE
        // ----------------------------------------------------

        gotoXY(x, 18);

        cout << "SCORE: " << score;

        gotoXY(x, 19);

        cout << "LINES: " << lines;

        gotoXY(x, 20);

        cout << "LEVEL: " << level;

        // ----------------------------------------------------
        // CONTROLS
        // ----------------------------------------------------

        gotoXY(x, 22);

        cout << "A/D  Move";

        gotoXY(x, 23);

        cout << "W    Rotate";

        gotoXY(x, 24);

        cout << "SPACE Hard Drop";

        gotoXY(x, 25);

        cout << "C    Hold";

        gotoXY(x, 26);

        cout << "Q    Quit";
    }

    // ========================================================
    // DRAW GAME OVER
    // ========================================================

    void drawGameOver()
    {
        int x = 5;
        int y = 8;

        setColor(LIGHT_RED);

        gotoXY(x, y);

        cout << "+--------------------+";

        gotoXY(x, y + 1);

        cout << "|     GAME OVER      |";

        gotoXY(x, y + 2);

        cout << "+--------------------+";

        setColor(BRIGHT_WHITE);

        gotoXY(x + 3, y + 4);

        cout << "SCORE: " << score;

        gotoXY(x + 3, y + 5);

        cout << "LINES: " << lines;

        gotoXY(x + 3, y + 6);

        cout << "LEVEL: " << level;

        gotoXY(x + 2, y + 8);

        cout << "R = Restart";

        gotoXY(x + 2, y + 9);

        cout << "Q = Quit";

        resetColor();
    }

    // ========================================================
    // DRAW
    // ========================================================

    void draw()
    {
        drawBoard();

        if (!gameOver)
        {
            drawCurrentPiece();
        }

        drawSidePanel();

        if (gameOver)
        {
            drawGameOver();
        }

        cout.flush();
    }

    // ========================================================
    // HANDLE INPUT
    // ========================================================

    bool handleInput()
    {
        if (!_kbhit())
            return true;

        int key = _getch();

        // ----------------------------------------------------
        // Q
        // ----------------------------------------------------

        if (key == 'q' ||
            key == 'Q')
        {
            return false;
        }

        // ----------------------------------------------------
        // R
        // ----------------------------------------------------

        if (gameOver)
        {
            if (key == 'r' ||
                key == 'R')
            {
                initializeGame();
            }

            return true;
        }

        // ----------------------------------------------------
        // Extended keyboard
        // Arrow keys
        // ----------------------------------------------------

        if (key == 0 ||
            key == 224)
        {
            key = _getch();

            switch (key)
            {
                case 75: // Left
                    moveLeft();
                    break;

                case 77: // Right
                    moveRight();
                    break;

                case 80: // Down
                    softDrop();
                    break;

                case 72: // Up
                    rotate();
                    break;
            }

            return true;
        }

        // ----------------------------------------------------
        // A
        // ----------------------------------------------------

        if (key == 'a' ||
            key == 'A')
        {
            moveLeft();
        }

        // ----------------------------------------------------
        // D
        // ----------------------------------------------------

        else if (key == 'd' ||
                 key == 'D')
        {
            moveRight();
        }

        // ----------------------------------------------------
        // S
        // ----------------------------------------------------

        else if (key == 's' ||
                 key == 'S')
        {
            softDrop();
        }

        // ----------------------------------------------------
        // W
        // ----------------------------------------------------

        else if (key == 'w' ||
                 key == 'W')
        {
            rotate();
        }

        // ----------------------------------------------------
        // SPACE
        // ----------------------------------------------------

        else if (key == ' ')
        {
            hardDrop();
        }

        // ----------------------------------------------------
        // C
        // ----------------------------------------------------

        else if (key == 'c' ||
                 key == 'C')
        {
            hold();
        }

        return true;
    }

    // ========================================================
    // RUN GAME
    // ========================================================

    void run()
    {
        bool running = true;

        while (running)
        {
            running =
                handleInput();

            if (!gameOver)
            {
                update();
            }

            draw();

            // Small delay to prevent CPU 100%
            Sleep(10);
        }

        showCursor();

        gotoXY(
            0,
            BOARD_HEIGHT + 5
        );

        setColor(BRIGHT_WHITE);

        cout << "Thanks for playing Tetris!\n";

        resetColor();
    }
};

// ============================================================
// MAIN
// ============================================================

int main()
{
    // --------------------------------------------------------
    // Set console title
    // --------------------------------------------------------

    SetConsoleTitleA(
        "C++ Tetris"
    );

    // --------------------------------------------------------
    // Create game
    // --------------------------------------------------------

    Tetris game;

    // --------------------------------------------------------
    // Run
    // --------------------------------------------------------

    game.run();

    return 0;
}