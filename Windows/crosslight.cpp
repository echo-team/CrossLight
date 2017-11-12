#include <iostream>
#include <windows.h>
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
    private:
        HANDLE hStdin, hStdout;
        CONSOLE_SCREEN_BUFFER_INFO csbInfo;
        ConsoleColor currConsoleText = White, currConsoleBackground = Black;
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
        bool SetCursorPosition(short x = 0, short y = 0)
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
            Next 2 functions set Color and Background of Console
        */
        bool SetTextColor(ConsoleColor text = White)
        {
            currConsoleText = text;
            return SetConsoleTextAttribute(hStdout, (WORD)((currConsoleBackground<<4)|text));
        }

        bool SetBackgroundColor(ConsoleColor background = Black)
        {
            currConsoleBackground = background;
            return SetConsoleTextAttribute(hStdout, (WORD)((background<<4)|currConsoleText));
        }

        ConsoleColor GetTextColor()
        {
            return currConsoleText;
        }

        ConsoleColor GetBackgroundColor()
        {
            return currConsoleBackground;
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
    ConsoleColor exTextColor = Blue;
    ConsoleColor exBackColor = Red;
    mainconsole.SetTextColor(exTextColor);
    mainconsole.SetBackgroundColor(exBackColor);
    COORDprint(mainconsole.GetConsoleSize());
    cout<<mainconsole.GetBackgroundColor();
    return 0;
}
