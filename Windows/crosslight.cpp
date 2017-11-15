#include <iostream>
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <stdio.h>
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
        DWORD numOfAttrsWritten, numOfAttrsRead, numOfEventsRead;
        WORD Attribute;
        INPUT_RECORD inputBuffer;
        COORD dimensions;

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
        BOOL setCursorPosition(short x = 0, short y = 0)
        {
            COORD coordDest;
            coordDest.X = x;
            coordDest.Y = y;
            return SetConsoleCursorPosition(hStdout, coordDest);
        }

        /*
            function that returns current coordinates of Cursor in format: row, column; using COORD structure;
        */
        COORD getCursorPosition()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return csbInfo.dwCursorPosition;
        }

        /*
            function that returns current size of console window in format: number of rows, number of columns; using COORD structure;
        */
        COORD getSize()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return csbInfo.dwSize;
        }

        /*
            Next 2 functions set Text and Background Colors of Console, which is used right after function call;
            if successfully accomplished than return a non-zero int, else returns zero;
        */
        BOOL setTextColor(ConsoleColor text = White)
        {
            return SetConsoleTextAttribute(hStdout, (WORD)((getBackgroundColor()<<4)|text));
        }

        BOOL setBackgroundColor(ConsoleColor background = Black)
        {
            return SetConsoleTextAttribute(hStdout, (WORD)((background<<4)|getTextColor()));
        }

        /*
            Next 2 functions return current Text and Background Colors of Console;
        */
        int getTextColor()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return(csbInfo.wAttributes % 16);
        }

        int getBackgroundColor()
        {
            GetConsoleScreenBufferInfo(hStdout, &csbInfo);
            return(csbInfo.wAttributes >> 4);
        }

        /*
            Next 2 procedures set Text and Background Colors for the whole previous text and for the future use;
        */
        void setGlobalBackgroundColor(int background = Black)
        {
            char tmpText = getTextColor(), color[16];
            if (background > 10)
                background += 55;
            else
                background += 48;
            if (tmpText > 10)
                tmpText += 55;
            else
                tmpText += 48;
            sprintf(color, "color %c%c", background, tmpText);
            const int NotUsed = system(color);
        }

        void setGlobalTextColor(int text = White)
        {
            char tmpBack = getBackgroundColor(), color[16];
            if (tmpBack > 10)
                tmpBack += 55;
            else
                tmpBack += 48;
            if (text > 10)
                text += 55;
            else
                text += 48;
            sprintf(color, "color %c%c", tmpBack, text);
            const int NotUsed = system(color);
        }

        /*
            Next 2 functions change Text and Background Colors in 'length' cells starting from 'x','y';
            If the function succeeds, the return value is nonzero. Else return value is zero.
            Obviously, that function tries to change every cell, so if there is no text in the cell it still counts;
            And also obviously that if you call for example 2-nd function after 1-st one; it will overwrite the previous attr with the one it gets from getTextColor method;
        */
        BOOL changeTextColor(short x, short y, DWORD length, ConsoleColor text = White)
        {
            WORD attr = (WORD)((getBackgroundColor()<<4)|text);
            COORD a;
            a.X = x;
            a.Y = y;
            return FillConsoleOutputAttribute(hStdout, attr, length, a, &numOfAttrsWritten);
        }

        BOOL changeBackgroundColor(short x, short y, DWORD length, ConsoleColor background = Black)
        {
            WORD attr = (WORD)(background<<4|getTextColor());
            COORD a;
            a.X = x;
            a.Y = y;
            return FillConsoleOutputAttribute(hStdout, attr, length, a, &numOfAttrsWritten);
        }

        /*
            Next function prints 'length' long string of chars 'ch' from position 'x', 'y';
            overwrites previous information, but takes it's TextAttributes;
        */
        BOOL characterPrint(DWORD length, short x, short y, TCHAR ch = ' ')
        {
            COORD a;
            a.X = x;
            a.Y = y;
            return FillConsoleOutputCharacter(hStdout, ch, length, a, &numOfAttrsWritten);
        }

        /*
            Flushes the console input buffer. All input records currently in the input buffer are discarded.
        */
        BOOL flushInput()
        {
            return FlushConsoleInputBuffer(hStdin);
        }

        /*
            Read console cells attributes starting from 'x', 'y';
            'length' many cells being read and written to Attribute;
        */
        int readTextAttribute(DWORD length, short x, short y)
        {
            COORD a;
            a.X = x;
            a.Y = y;
            ReadConsoleOutputAttribute(hStdout, &Attribute, length, a, &numOfAttrsRead);
            return(Attribute%16);
        }

        int readBackgroundAttribute(DWORD length, short x, short y)
        {
            COORD a;
            a.X = x;
            a.Y = y;
            ReadConsoleOutputAttribute(hStdout, &Attribute, length, a, &numOfAttrsRead);
            return(Attribute>>4);
        }

        /*
            Reads data from input buffer without removing it from the buffer;
            'length' number of elements and writes it to 'inputBuffer'
        */
        BOOL peekInput(DWORD length)
        {
            return PeekConsoleInput(hStdin, &inputBuffer, length, &numOfEventsRead);
        }

        /*
            Sets the display mode of the specified console screen buffer.
        */
        BOOL setFullscreenMode()
        {
            return SetConsoleDisplayMode(hStdout, 1, &dimensions);
        }

        BOOL setWindowedMode()
        {
            return SetConsoleDisplayMode(hStdout, 2, &dimensions);
        }

        /*
            Takes between 2 and 4 parameters in order:
                right border of window
                bottom border of window
                left border of window(optional)
                top border of window(optional)
            And sets Window Size of the current Screen Buffer
        */
        BOOL setWindowSize(short right, short bottom, short left = 0, short top = 0)
        {
            SMALL_RECT rect;
            if (left == 0 && top == 0)
            {
                GetConsoleScreenBufferInfo(hStdout, &csbInfo);
                rect.Left = csbInfo.srWindow.Left;
                rect.Top = csbInfo.srWindow.Top;
                rect.Right = csbInfo.srWindow.Left + right;
                rect.Bottom = csbInfo.srWindow.Top + bottom;
            }
            else
            {
                rect.Left = left;
                rect.Right = right;
                rect.Bottom = bottom;
                rect.Top = top;
            }
            SetConsoleWindowInfo(hStdout, 1, &rect);
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
    COORDprint(mainconsole.getSize());
    for (int i = 0; i < 15; i++)
        cout<<i<<' ';
    mainconsole.changeTextColor(6, 1, 8, Cyan);
    mainconsole.changeBackgroundColor(7, 1, 1, Cyan);
    mainconsole.characterPrint(7, 1, 1, 'e');
    mainconsole.setFullscreenMode();
    cin.get();
    mainconsole.setWindowedMode();
    cout<<endl<<endl<<"END OF YOUR LIFE"<<endl;
    return 0;
}
