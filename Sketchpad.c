#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include<math.h>
#include<windows.h>
#include "graphics.h"
#include "genlib.h"
#include"functiondisplay.h"
#include"MYGUI.h"
#include"standard.h"
#include"cipher.h"
#include"break_continue.h"

extern double winwidth, winheight;
extern double WinWidth,WinHeight;

extern Coordinate o;
extern int function_num;
extern List *ploynomial[10];
extern bool isContinue;
extern char MusicName[10][20];
extern int MusicNumber;

void MouseEventProcess(int x,int y,int button,int event);
void KeyboardEventProcess(int key,int event);

static void InitProgram(void);

void Main()
{
	isBuy();
	SetWindowTitle("The Geometer's Sketchpad");
	
	//InitConsole();

	InitGraphics();
	
	InitProgram();
	
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	
}

static void InitProgram(void)
{
	function_num=0;
	
	o.x=GetWindowWidth()/2;
	o.y=GetWindowHeight()/2;
	
	winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
    
    WinWidth = GetWindowWidth();
    WinHeight = GetWindowHeight();
    
    InitUserInputNULL();
    
    if(CheckContinue()==1) isContinue=TRUE;
    if(FileGetMusicName()==-1) MessageBox(NULL,TEXT("“Ù¿÷∂¡»°¥ÌŒÛ"),TEXT("Sketchpad"),MB_OK);
}
