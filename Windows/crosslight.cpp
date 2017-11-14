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
        DWORD numOfAttrsWritten;

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
            1 - x-coordinate;
            2 - y-coordinate;
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
        */
        COORD GetCursorPosition()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return csbInfo.dwCursorPosition;
        }

        /*
            function that returns current size of console window in format: number of rows, number of columns; using COORD structure;
        */
        COORD GetConsoleSize()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return csbInfo.dwSize;
        }

        /*
            Next 2 functions set Text and Background Colors of Console, which is used right after function call;
            if successfully accomplished than return a non-zero int, else returns zero;
        */
        bool SetTextColor(ConsoleColor text = White)
        {
            return SetConsoleTextAttribute(hStdout, (WORD)((GetBackgroundColor()<<4)|text));
        }

        bool SetBackgroundColor(ConsoleColor background = Black)
        {
            return SetConsoleTextAttribute(hStdout, (WORD)((background<<4)|GetTextColor()));
        }

        /*
            Next 2 functions return current Text and Background Colors of Console;
        */
        int GetTextColor()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return(csbInfo.wAttributes % 16);
        }

        int GetBackgroundColor()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return(csbInfo.wAttributes >> 4);
        }

        /*
            Next 2 procedures set Text and Background Colors for the whole previous text and for the future use;
        */
        void SetGlobalBackgroundColor(ConsoleColor background = Black)
        {
            char tmpBack = GetBackgroundColor(), tmpText = GetTextColor(), color[16];
            if (tmpBack > 10)
                tmpBack += 55;
            else
                tmpBack += 48;
            if (tmpText > 10)
                tmpText += 55;
            else
                tmpText += 48;
            sprintf(color, "color %c%c", tmpBack, tmpText);
            const int NotUsed = system(color);
        }

        void SetGlobalTextColor(ConsoleColor text = White)
        {
            char tmpBack = GetBackgroundColor(), tmpText = GetTextColor(), color[16];
            if (tmpBack > 10)
                tmpBack += 55;
            else
                tmpBack += 48;
            if (tmpText > 10)
                tmpText += 55;
            else
                tmpText += 48;
            sprintf(color, "color %c%c", tmpBack, tmpText);
            const int NotUsed = system(color);
        }

        /*
            Next 2 functions change Text and Background Colors in 'length' cells starting from 'x','y';
            If the function succeeds, the return value is nonzero. Else return value is zero.
            Obviously, that function tries to change every cell, so if there is no text in the cell it still counts;
            And also obviously that if you call for example 2-nd function after 1-st one; it will overwrite the previous attr with the one it gets from GetTextColor method;
        */
        bool ChangeTextColor(short x, short y, DWORD length, ConsoleColor text = White)
        {
            WORD attr = (WORD)((GetBackgroundColor()<<4)|text);
            COORD a;
            a.X = x;
            a.Y = y;
            return FillConsoleOutputAttribute(hStdout, attr, length, a, &numOfAttrsWritten);
        }

        bool ChangeBackColor(short x, short y, DWORD length, ConsoleColor background = Black)
        {
            WORD attr = (WORD)(background<<4|GetTextColor());
            cout<<endl<<"WORD "<<attr<<endl;
            COORD a;
            a.X = x;
            a.Y = y;
            return FillConsoleOutputAttribute(hStdout, attr, length, a, &numOfAttrsWritten);
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
    COORDprint(mainconsole.GetConsoleSize());
    for (int i = 0; i < 15; i++)
        cout<<i<<' ';
    mainconsole.ChangeTextColor(6, 1, 8, Cyan);
    mainconsole.ChangeBackColor(7, 1, 5, Red);
    cout<<endl<<endl<<"END OF YOUR LIFE";
    return 0;
}
