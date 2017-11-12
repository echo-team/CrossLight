#include <iostream>
#include <windows.h>


#define BACKGROUND_WHITE BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED
#define FOREGROUND_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED

using namespace std;


enum ConsoleColor
{
    Black         = 0,
    Blue          = 1,
    Green         = 2,
    Cyan          = 3,
    Red           = 4,
    Magenta       = 5,
    Brown         = 6,
    LightGray     = 7,
    DarkGray      = 8,
    LightBlue     = 9,
    LightGreen    = 10,
    LightCyan     = 11,
    LightRed      = 12,
    LightMagenta  = 13,
    Yellow        = 14,
    White         = 15
};

class Console
{
        HANDLE hStdin, hStdout;
        CONSOLE_SCREEN_BUFFER_INFO csbInfo;

    public:
        Console()
        {
            /*
                initialization of input and output HANDLEs
            */
            hStdin = GetStdHandle(STD_INPUT_HANDLE);
            hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        };

        /*
            function that sets cursor position in console;
            if successfully accomplished than return a non-zero int, else returns zero
                parameters:
            1 - some piece of shit (screen buffer handle)
            2 - coordinates using a COORD structure
        */
        bool SetCursorPosition(short x, short y)
        {
            COORD coordDest;
            coordDest.X = x;
            coordDest.Y = y;
            return SetConsoleCursorPosition(hStdout, coordDest);
        }

        /*
            function that returns current coordinates of Cursor in format: row, column; using COORD structure;
                parameters:
            1 - some piece of shit (screen buffer handle)
        */
        COORD GetCursorPosition()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return csbInfo.dwCursorPosition;
        }

        /*
            function that returns current size of console window in format: number of rows, number of columns; using COORD structure;
                parameters:
            1 - some piece of shit (screen buffer handle)
        */
        COORD GetConsoleSize()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return csbInfo.dwSize;
        }

        /*
            function that changes text and background colors

            this function overwrites the previous value
            TO DO:
                store previous value
        */
        bool SetColor(ConsoleColor text, ConsoleColor background)
        {
            return SetConsoleTextAttribute(hStdout, (WORD)((background<<4)|text));
        }

};

//function for printing COORD structure
void COORDprint(COORD coords)
{
    cout<<coords.X<<' '<<coords.Y<<endl;
}


int main()
{
    COORD coords;
    Console mainconsole;
    ConsoleColor exTextColor = White;
    ConsoleColor exBackColor = Red;
    mainconsole.SetColor(exTextColor, exBackColor);
    COORDprint(mainconsole.GetConsoleSize());
    return 0;
}
