#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conio.h"

int __FOREGROUND = LIGHTGRAY;
int __BACKGROUND = BLACK;

void _setcursortype(int _type)
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = TRUE;
    if (_type == _NOCURSOR)
        Info.bVisible = FALSE;
    else if (_type == _SOLIDCURSOR)
        Info.dwSize = 100;
    else if (_type == _NORMALCURSOR)
        Info.dwSize = 1;
    SetConsoleCursorInfo (GetStdHandle (STD_OUTPUT_HANDLE), &Info);
}

void clrscr()
{
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;

    coord.X = 0;
    coord.Y = 0;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ',
                                info.dwSize.X * info.dwSize.Y, coord, &written);
    SetConsoleOutputCP(437);
    gotoxy (0, 0);
}

int cputs(const char *_str)
{
    printf ("%s\n", _str);
    return 0;
}

int getche()
{
    int ch;
    ch = getch ();
    printf ("%c\n", ch);
    return ch;
}

KeyValueStru ReadKeyPush()
{
    INPUT_RECORD _InRec;
    DWORD _NumRead;
    KeyValueStru KeyValue;
    KeyValue.VKey=0;
    KeyValue.AscChar=0;

    if(!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&_InRec,1,&_NumRead))
        return KeyValue;
    if(_InRec.EventType!=KEY_EVENT)
        return KeyValue;
    if(_InRec.Event.KeyEvent.bKeyDown == 0)
        return KeyValue;
    KeyValue.VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
    KeyValue.AscChar=_InRec.Event.KeyEvent.uChar.AsciiChar;
    return KeyValue;
}

KeyValueStru kbhit()
{
    KeyValueStru KeyValue;
    while (TRUE) {
        KeyValue=ReadKeyPush();
        if(KeyValue.VKey
                +KeyValue.AscChar>0x12) {
            return(KeyValue);
            break;
        }
    }
}

void gettextinfo(struct text_info *_r)
{
    CONSOLE_SCREEN_BUFFER_INFO Info;
    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &Info);
    _r->winleft = Info.srWindow.Left;
    _r->winright = Info.srWindow.Right;
    _r->wintop = Info.srWindow.Top;
    _r->winbottom = Info.srWindow.Bottom;
    _r->attribute = Info.wAttributes;
    _r->normattr = LIGHTGRAY | BLACK;
    _r->screenheight = Info.dwSize.Y;
    _r->screenwidth = Info.dwSize.X;
    _r->curx = wherex ();
    _r->cury = wherey ();
}

void puttext(int _left, int _top, int _right, int _bottom, PCHAR_INFO _source)
{
    COORD Corner,dwBufferSize;
    SMALL_RECT MyRect;
    Corner.X=_right-_left+1;
    Corner.Y=_bottom-_top+1;
    dwBufferSize.X=0;
    dwBufferSize.Y=0;
    MyRect.Left=_left;
    MyRect.Top=_top;
    MyRect.Right=_right;
    MyRect.Bottom=_bottom;
    WriteConsoleOutputA(GetStdHandle(STD_OUTPUT_HANDLE),_source,Corner,dwBufferSize,
                        &MyRect);
}

void gettext(int _left, int _top, int _right, int _bottom, PCHAR_INFO _target)
{
    COORD Corner,dwBufferSize;
    SMALL_RECT MyRect;
    Corner.X=_right-_left+1;
    Corner.Y=_bottom-_top+1;
    dwBufferSize.X=0;
    dwBufferSize.Y=0;
    MyRect.Left=_left;
    MyRect.Top=_top;
    MyRect.Right=_right;
    MyRect.Bottom=_bottom;
    ReadConsoleOutputA(GetStdHandle(STD_OUTPUT_HANDLE),_target,Corner,dwBufferSize,
                       &MyRect);
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x ;
    c.Y = y ;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void gppconio_init()
{
    /* Do nothing */
}

void highvideo()
{
    if (__FOREGROUND <= BROWN)
        textcolor (__FOREGROUND + 9);
    if (__BACKGROUND <= BROWN)
        textbackground (__BACKGROUND + 9);
}

void insline()
{
    printf ("\n");
}

int putch(int _c)
{
    printf ("%c", _c);
    return _c;
}

void textattr(int _attr)
{
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}

void textbackground(int _color)
{
    if (_color == BLINK)
        _color = WHITE;
    __BACKGROUND = _color;
    textattr (__FOREGROUND | (_color + 29));
}

void textcolor(int _color)
{
    if (_color == BLINK)
        _color = WHITE;
    __FOREGROUND = _color;
    textattr(_color | __BACKGROUND);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.Y - 2;
}

void window(int _left, int _top, int _right, int _bottom)
{
    SMALL_RECT R;
    R.Left = _left;
    R.Top = _top;
    R.Right = _right;
    R.Bottom = _bottom;
    SetConsoleWindowInfo (GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &R);
    gotoxy (_left, _top);
}

