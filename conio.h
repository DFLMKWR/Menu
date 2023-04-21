#ifndef CONIO_H
#define CONIO_H

#include <windows.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct KeyValueStru {
    WORD  VKey,AscChar;
} KeyValueStru;

struct text_info {
    unsigned char winleft;
    unsigned char wintop;
    unsigned char winright;
    unsigned char winbottom;
    unsigned char attribute;
    unsigned char normattr;
    unsigned char currmode;
    unsigned char screenheight;
    unsigned char screenwidth;
    unsigned char curx;
    unsigned char cury;
};

enum COLORS {
    /*  dark colors     */
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    /*  light colors    */
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

#define BLINK          0x80
#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

KeyValueStru kbhit();
void _set_screen_lines(int nlines);
void _setcursortype(int _type);
void blinkvideo();
char *cgets(char *_str);
void clreol();
void clrscr();
#define cprintf printf
int cputs(const char *_str);
#define cscanf scanf
void delline();
#define getch getchar
int getche();
void gettext(int _left, int _top, int _right, int _bottom, PCHAR_INFO _target);
void gettextinfo(struct text_info *_r);
void gotoxy(int x, int y);
void gppconio_init();
void highvideo();
void insline();
void intensevideo();
void lowvideo();
int movetext(int _left, int _top, int _right, int _bottom, int _destleft,
             int _desttop);
void normvideo();
int putch(int _c);
void puttext(int _left, int _top, int _right, int _bottom, PCHAR_INFO _source);
void textattr(int _attr);
void textbackground(int _color);
void textcolor(int _color);
void textmode(int _mode);
int ungetch(int);
int wherex();
int wherey();
void window(int _left, int _top, int _right, int _bottom);

#ifdef __cplusplus
}
#endif

#endif

