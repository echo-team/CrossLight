#include <iostream>
#include <windows.h>


using namespace std;


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
            function that sets text attributes
            all attributes described in main

            this function overwrites the previous value
            TO DO:
                store previous value
        */
        bool SetTextAttribute(WORD atributes)
        {
            return SetConsoleTextAttribute(hStdout, atributes);
        }

};

//function for printing COORD structure
void COORDprint(COORD coords)
{
    cout<<coords.X<<' '<<coords.Y<<endl;
}


int main()
{
    WORD attributesExample;
    COORD coords;
    attributesExample = FOREGROUND_RED | BACKGROUND_BLUE;
    /*
                ALL POSSIBLES ATTRIBUTES
                FOR SetTextAttribute:
        FOREGROUND_BLUE 0x0001
            Text color contains blue.
        FOREGROUND_GREEN 0x0002
            Text color contains green.
        FOREGROUND_RED 0x0004
            Text color contains red.
        FOREGROUND_INTENSITY 0x0008
            Text color is intensified.
        BACKGROUND_BLUE 0x0010
            Background color contains blue.
        BACKGROUND_GREEN 0x0020
            Background color contains green.
        BACKGROUND_RED 0x0040
            Background color contains red.
        BACKGROUND_INTENSITY 0x0080
            Background color is intensified.
        COMMON_LVB_LEADING_BYTE 0x0100
            Leading byte.
        COMMON_LVB_TRAILING_BYTE 0x0200
            Trailing byte.
        COMMON_LVB_GRID_HORIZONTAL 0x0400
            Top horizontal
        COMMON_LVB_GRID_LVERTICAL 0x0800
                Left vertical.
        COMMON_LVB_GRID_RVERTICAL 0x1000
            Right vertical.
        COMMON_LVB_REVERSE_VIDEO 0x4000
            Reverse foreground and background attribute.
        COMMON_LVB_UNDERSCORE 0x8000
            Underscore.
    */
    Console mainconsole;
    mainconsole.SetTextAttribute(attributesExample);
    attributesExample = FOREGROUND_RED;
    mainconsole.SetTextAttribute(attributesExample);
    COORDprint(mainconsole.GetConsoleSize());
    return 0;
}
