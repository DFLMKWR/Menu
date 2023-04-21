#include <bits/stdc++.h>
#include "conio.h"

#define Enter         0x0d
#define Up            38
#define Left          37
#define Right         39
#define Down          40
#define Esc           27
#define SelectAttr    0x20
#define NoSelectAttr  0x70
#define SystemBorder  0x17
#define BorderColor   0x30
#define MainMenuLine  1
#define MainMenuCount 8

typedef signed short int ShortInt;

typedef struct MenuItemStru {
    ShortInt SubMenuItemCount;
    ShortInt SubMenuItemWidth;
    MenuItemStru *SubMenuItemAddr;
    ShortInt MenuItemStrWidth;
    char *MenuItemStrAddr;
} MenuItemStru;

typedef struct MenuSelectStru {
    ShortInt MenuIndex, MenuItemTotal;
    MenuItemStru *pMenu;
    PCHAR_INFO AddrOfSaveText;
    bool PullDownMenu;
    ShortInt left, top, right, bottom;
} MenuSelectStru;

typedef struct MenuAnalyseStru {
    ShortInt preBlankCount;
    int size;
    int top;
    int proiortop;
    MenuItemStru *MenuArray;
} MenuAnalyseStru;

FILE *OpenDataFile(char *filename);
void ShowAlineOfBox(int x, int y, int width, char *c, int c1, int c2);
void ShowBox(int x1, int y1, int x2, int y2, char *c, int c1, int c2);
void LeftMove();
void RightMove();
void UpMove();
void DownMove();
void EnterMove();
void EscMove();
void Operate();
void ShowItem(int c);
void ShowMenu();

char S_S_Box[10] = {-38, -60, -65, -77, 32, -77, -64, -60, -39};

int Layer = 0;
MenuAnalyseStru MenuLayer[5];
int menuKey[] = {Esc, Enter, Left, Right, Up, Down};
void (*menuFunc[])() = {EscMove, EnterMove, LeftMove, RightMove, UpMove, DownMove};
MenuItemStru MainMenu[MainMenuCount] = {};
int MainMenuItemCol[MainMenuCount] = {0x03, 0x0b, 0x13, 0x19, 0x25, 0x30, 0x3f, 0x48};
CHAR_INFO FourthSave[1000], ThirdSave[1000], SecondSave[1000], FirstSave[1000];
MenuSelectStru His[5] = {
    {0, 8, MainMenu, 0,          false, 0, 0, 0, 0},
    {0, 0, 0,        FirstSave,  false, 0, 0, 0, 0},
    {0, 0, 0,        SecondSave, false, 0, 0, 0, 0},
    {0, 0, 0,        ThirdSave,  false, 0, 0, 0, 0},
    {0, 0, 0,        FourthSave, false, 0, 0, 0, 0}
};

FILE *OpenDataFile(char *filename)
{
    char *p;
    FILE *myfile;
    char szFullPath[MAX_PATH];
    GetModuleFileName(NULL, szFullPath, MAX_PATH);
    int j = strlen(szFullPath) - 1;
    while (j > 0 && szFullPath[j] != '\\')
        j--;
    p = &szFullPath[j + 1];
    strcpy(p, filename);
    myfile = fopen(szFullPath, "r");
    return myfile;
}

void Storage()
{
    FILE *fp = OpenDataFile("TurboC.txt");
    fseek(fp, 0L, SEEK_END);
    int flen = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    int lay = 0;
    int len, preSpaceCnt = -1, spaceCnt;
    MenuItemStru *parentmenu, *submenu;

    for (int i = 0; i < 5; i++) {
        MenuLayer[i].size = 10000;
        MenuLayer[i].MenuArray = (MenuItemStru *) malloc(sizeof(
                                     MenuItemStru) * MenuLayer[i].size);
    }

    char *ch = (char *) malloc(sizeof(char) * (flen + 1));
    fread(ch, flen, 1, fp);
    char *pch = ch;

    flen = strlen(pch);
    for (int i = 0; i < flen; i++) {
        if (*(pch + i) == '\n') *(pch + i) = '\0';
    }

    int cnt = 0;
    while (cnt < flen) {
        spaceCnt = 0;
        while (*pch == ' ' && cnt < flen) {
            pch++;
            cnt++;
            spaceCnt++;
        }

        if (spaceCnt > preSpaceCnt) {
            lay++;
            MenuLayer[lay].preBlankCount = spaceCnt;
            MenuLayer[lay].proiortop = MenuLayer[lay].top;
            MenuLayer[lay].top++;

            submenu = MenuLayer[lay].MenuArray + MenuLayer[lay].top;
            parentmenu = MenuLayer[lay - 1].MenuArray + MenuLayer[lay - 1].top;
            parentmenu->SubMenuItemAddr = submenu;
            submenu->MenuItemStrAddr = pch;
            submenu->SubMenuItemAddr = NULL;

            submenu->MenuItemStrWidth = strlen(pch);
            parentmenu->SubMenuItemWidth = std::max(parentmenu->SubMenuItemWidth,
                                                    submenu->MenuItemStrWidth);
        } else if (spaceCnt == preSpaceCnt) {
            MenuLayer[lay].top++;
            submenu = MenuLayer[lay].MenuArray + MenuLayer[lay].top;
            submenu->MenuItemStrAddr = pch;
            submenu->SubMenuItemAddr = NULL;

            submenu->MenuItemStrWidth = strlen(pch);
            parentmenu = MenuLayer[lay - 1].MenuArray + MenuLayer[lay - 1].top;
            parentmenu->SubMenuItemWidth = std::max(parentmenu->SubMenuItemWidth,
                                                    submenu->MenuItemStrWidth);
        } else {
            MenuLayer[lay - 1].MenuArray[MenuLayer[lay - 1].top].SubMenuItemCount =
                MenuLayer[lay].top - MenuLayer[lay].proiortop;

            while (lay > 0 && MenuLayer[lay].preBlankCount > spaceCnt) {
                lay--;
                MenuLayer[lay].MenuArray[MenuLayer[lay].top].SubMenuItemCount =
                    MenuLayer[lay + 1].top - MenuLayer[lay + 1].proiortop;
            }

            MenuLayer[lay].top++;
            submenu = MenuLayer[lay].MenuArray + MenuLayer[lay].top;
            submenu->MenuItemStrAddr = pch;
            submenu->SubMenuItemAddr = NULL;

            submenu->MenuItemStrWidth = strlen(pch);
            parentmenu = MenuLayer[lay - 1].MenuArray + MenuLayer[lay - 1].top;
            parentmenu->SubMenuItemWidth = std::max(parentmenu->SubMenuItemWidth,
                                                    submenu->MenuItemStrWidth);
        }

        len = strlen(pch) + 1;
        pch += len;
        cnt += len;
        preSpaceCnt = spaceCnt;
    }

    while (lay > 0) {
        lay--;
        MenuLayer[lay].MenuArray[MenuLayer[lay].top].SubMenuItemCount =
            MenuLayer[lay + 1].top - MenuLayer[lay + 1].proiortop;
    }

    for (int i = 1; i <= MenuLayer[0].MenuArray[0].SubMenuItemCount; i++) {
        MainMenu[i - 1] = MenuLayer[1].MenuArray[i];
    }
}

void ShowAlineOfBox(int x, int y, int width, char *c, int c1, int c2)
{
    gotoxy(x, y);
    textattr(c1);
    printf("%c", c[0]);
    textattr(c2);
    for (int i = 1; i < width; i++) {
        printf("%c", c[1]);
    }
    textattr(c1);
    printf("%c", c[2]);
}

void ShowBox(int x1, int y1, int x2, int y2, char *c, int c1, int c2)
{
    int width = x2 - x1 + 1;
    ShowAlineOfBox(x1, y1, width, c, c1, c1);
    for (int i = y1 + 1; i < y2; i++) {
        ShowAlineOfBox(x1, i, width, c + 3, c1, c2);
    }
    ShowAlineOfBox(x1, y2, width, c + 6, c1, c1);
}

void LeftMove()
{
    if (!Layer) {
        ShowItem(NoSelectAttr);
        His[Layer].MenuIndex = (His[Layer].MenuIndex + His[Layer].MenuItemTotal - 1) %
                               His[Layer].MenuItemTotal;
        ShowItem(SelectAttr);
    }
}

void RightMove()
{
    if (!Layer) {
        ShowItem(NoSelectAttr);
        His[Layer].MenuIndex = (His[Layer].MenuIndex + 1) % His[Layer].MenuItemTotal;
        ShowItem(SelectAttr);
    }
}

void UpMove()
{
    ShowItem(NoSelectAttr);
    His[Layer].MenuIndex = (His[Layer].MenuIndex + His[Layer].MenuItemTotal - 1) %
                           His[Layer].MenuItemTotal;
    ShowItem(SelectAttr);
}

void DownMove()
{
    ShowItem(NoSelectAttr);
    His[Layer].MenuIndex = (His[Layer].MenuIndex + 1) % His[Layer].MenuItemTotal;
    ShowItem(SelectAttr);
}

void EnterMove()
{
    if (!His[Layer].pMenu[His[Layer].MenuIndex].SubMenuItemCount) return;

    Layer++;
    if (Layer != 1) {
        His[Layer].left = His[Layer - 1].left;
        His[Layer].top = His[Layer - 1].top + His[Layer - 1].MenuIndex + 3;
    } else {
        His[Layer].left = MainMenuItemCol[His[Layer - 1].MenuIndex];
        His[Layer].top = His[Layer - 1].top + 2;
    }

    His[Layer].MenuIndex = 0;
    His[Layer].MenuItemTotal =
        His[Layer - 1].pMenu[His[Layer - 1].MenuIndex].SubMenuItemCount;
    His[Layer].pMenu =
        His[Layer - 1].pMenu[His[Layer - 1].MenuIndex].SubMenuItemAddr;
    His[Layer].right = His[Layer].left +
                       His[Layer - 1].pMenu[His[Layer - 1].MenuIndex].SubMenuItemWidth;
    His[Layer].bottom = His[Layer].top + His[Layer].MenuItemTotal;
    His[Layer].PullDownMenu = true;

    gettext(His[Layer].left - 1, His[Layer].top - 1, His[Layer].right + 1,
            His[Layer].bottom + 1, His[Layer].AddrOfSaveText);
    ShowMenu();
}

void EscMove()
{
    if (!Layer) {
        exit(0);
    }

    if (His[Layer].PullDownMenu) {
        puttext(His[Layer].left - 1, His[Layer].top - 1, His[Layer].right + 1,
                His[Layer].bottom + 1, His[Layer].AddrOfSaveText);
        Layer--;
    }
}

void Operate()
{
    do {
        KeyValueStru CurrentKey = kbhit();
        for (int i = 0; i < 6; i++) {
            if (menuKey[i] == CurrentKey.VKey) {
                (*menuFunc[i])();
                break;
            }
        }
    } while (1);
}

void ShowItem(int c)
{
    textattr(c);
    if (Layer) {
        gotoxy(His[Layer].left, His[Layer].top + His[Layer].MenuIndex);
    } else gotoxy(MainMenuItemCol[His[Layer].MenuIndex], MainMenuLine - 1);
    printf("%s", His[Layer].pMenu[His[Layer].MenuIndex].MenuItemStrAddr);
}

void ShowMenu()
{
    int CurColor = SelectAttr;
    int f = 1;

    for (int i = 0; i < His[Layer].MenuItemTotal; i++) {
        if (Layer == 0) {
            gotoxy(MainMenuItemCol[i], MainMenuLine - 1);
        } else if (His[Layer].MenuItemTotal) {
            if (f) {
                ShowBox(His[Layer].left - 1, His[Layer].top - 1, His[Layer].right,
                        His[Layer].bottom, S_S_Box, (RED << 4), (WHITE << 4));
                f = 0;
            }
            gotoxy(His[Layer].left, His[Layer].top + i);
        }

        textattr(CurColor);
        printf("%s", His[Layer].pMenu[i].MenuItemStrAddr);
        CurColor = NoSelectAttr;
    }
    gotoxy(His[Layer].left, His[Layer].top);
}

void init()
{
    clrscr();
    textattr(0x75);
    printf("%87s", " ");
    ShowBox(0, 1, 85, 25, S_S_Box, (RED << 4), (WHITE << 4));
}

int main()
{
    init();
    Storage();
    ShowMenu();
    Operate();

    return 0;
}


