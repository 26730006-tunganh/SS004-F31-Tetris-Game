#ifndef WINDOWS_H_COMPAT
#define WINDOWS_H_COMPAT

#ifdef _WIN32

#include <windows.h>

#else

#include <unistd.h>
#include <iostream>

// ============================
// Sleep
// ============================

inline void Sleep(unsigned int ms)
{
    usleep(ms * 1000);
}

// ============================
// Console code page
// ============================

inline int SetConsoleOutputCP(unsigned int wCodePageID)
{
    (void)wCodePageID;
    return 1;
}

inline int SetConsoleCP(unsigned int wCodePageID)
{
    (void)wCodePageID;
    return 1;
}

// ============================
// Console Color
// ============================

// Giả lập HANDLE
using HANDLE = int;

#define STD_OUTPUT_HANDLE (-1)

// Màu Windows
#define FOREGROUND_BLUE 0x01
#define FOREGROUND_GREEN 0x02
#define FOREGROUND_RED 0x04
#define FOREGROUND_INTENSITY 0x08

inline HANDLE GetStdHandle(int)
{
    return 1;
}

inline int SetConsoleTextAttribute(HANDLE, int color)
{
    switch (color)
    {
    case FOREGROUND_GREEN | FOREGROUND_BLUE |
        FOREGROUND_INTENSITY:

        std::cout << "\033[96m"; // Cyan
        break;

    case FOREGROUND_RED | FOREGROUND_GREEN |
        FOREGROUND_INTENSITY:

        std::cout << "\033[93m"; // Yellow
        break;

    case FOREGROUND_RED | FOREGROUND_BLUE |
        FOREGROUND_INTENSITY:

        std::cout << "\033[95m"; // Purple
        break;

    case FOREGROUND_GREEN |
        FOREGROUND_INTENSITY:

        std::cout << "\033[92m"; // Green
        break;

    case FOREGROUND_RED |
        FOREGROUND_INTENSITY:

        std::cout << "\033[91m"; // Red
        break;

    case FOREGROUND_BLUE |
        FOREGROUND_INTENSITY:

        std::cout << "\033[94m"; // Blue
        break;

    case FOREGROUND_RED |
        FOREGROUND_GREEN:

        std::cout << "\033[38;5;208m"; // Orange
        break;

    default:

        std::cout << "\033[0m"; // Reset
        break;
    }

    return 1;
}

#endif

#endif