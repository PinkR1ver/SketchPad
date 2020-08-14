#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include"functiondisplay.h"
#include"standard.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include<string.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"

#define TEXTWIDTH 0.089888
#define StringLength 50
#define PI 3.1415926

extern int function_num;
extern List *ploynomial[10];
extern Coordinate o;

double winwidth, winheight;
double WinWidth,WinHeight;

char MusicName[10][20];
int MusicNumber,CurrentMusic=0;

bool isContinue=FALSE;

static bool isUpdate=TRUE,isInput=FALSE,isLegal=TRUE,isException=FALSE,isClear=FALSE,isExit=FALSE,isFunction=FALSE,isFlag=FALSE;//���ڽ�������ж�
static bool isMusic=FALSE; 
static bool isDerivation=FALSE,isCalculus=FALSE,isDex=TRUE,isDey=TRUE;//���ڽ�������ж� 
static int isSave=0;
static char *sigh[]={"1","2","3","+","4","5","6","-","7","8","9","*",".","0","X","^","(",")","OVER","OK"};
static char *sigh2[]={"1","2","3","4","5","6","7","8","9",".","0","OK"};
static char *sigh3[]={"1","2","3","+","4","5","6","-","7","8","9","*",".","0","X","^","(",")","��","dX"};
static char UserInput[StringLength];//�û���������
static bool ShowDevelopInfor=FALSE;//�ж��Ƿ���ʾ��������Ϣ 
static bool ShowFunctionIntro=FALSE;//�ж��Ƿ���ʾ���������Ϣ 
static char *DevelopInfor[20]={"The Geometer's Sketchapad","����","��������","��","������","��","�������ݼ����","��","�������ݣ����ͼ��"};
static char *FunctionIntro="��Ӧ�ÿ����ڻ��ƶ���ʽ����ͼ���󵼡�������Լ��󽻵�";
static char *InvalidInformation="Invalid Input! F5 to back";
static char *Exception="�����ڴ����汾��"; 
static char *Save[2]={"Save Success!" ,"Save Fail!"};
static char *ContinueInformation[4]={"��⵽���ϴα���ĺ���ͼ��","��Y�ػ�","��N�˳�","δ��⵽�ϴ��к���ͼ��"};
static char *TipContinueInformation[3]={"����δ���溯��ͼ��ȷ��Ҫֱ���˳���","��Y�������˳�","��N����"};
static char *ClearInformation[2]={"Clear Success!","Clear Fail!"}; 
static char *ShowDerivation="������ֵ:"; 
static char *DerivationInput=NULL; //���ڴ����û�ѡ����󵼺����������� 
static char *Haha[10]={"I'M OK","MYGUI","NOTME","OHMYGOD","TESTTXST","WINDOWS","RAINBOW","OHMYGODDESS","NOYOUAREGOOD","GODFATHER"};//���ڲ���λ��(������ 
static char Dex[20],Dey[20];//���ڴ����û���������е�x�ĺ������������ 
static int indexdex=0,indexdey=0;//�û������󵼺����ꡢ�������ڴ��������е�ǰ����λ��
static char CalculusInput[100]={'\0'};
static int  indexcal=0;
static double angleX=0, cx, cy;
static int Inputk=0;

static bool inBox(double x, double y, double x1, double x2, double y1, double y2);// 

//װ�κ��� 
static void forward(double distance);
static void turn(double angle);
static void move(double distance);


//��������ĸ������� 
static void DevelopInformation(void);
static void FunctionIntroduction(void); 
static void InvalidInput(void);
static void toException(void);
static void toSave(void);
static void toContinue(void); 
static void toClear(void);
static void toExitInformation(void);

//ˢ�½��� 
static void InitScreen();

static void InitPloynomial();

static void DrawMusicDisplayButton();

static void InputDerivation(void);
static void InputCalculus(void); 

static void DrawBox(double x, double y, double width, double height);
static void DrawMusicTriangle(double length_of_side);
static void DrawMusicTriangle_opposite(double length_of_side);
static DrawMusicBox(double length_of_side);

static int MyButton(int id, double x, double y, int type,char *framColor,char *filledColor);
static void MyuiGetMouse(int x, int y, int button, int event);

typedef struct {
	double mousex;
	double mousey;
	int    mousedown;
	int    clickedItem;// item that was clicked
	int    actingMenu; // acting menu list 
	int    kbdItem;    // item that takes keyboard
	int    lastItem;   // item that had focus just before
	int    keyPress;   // input key
	int    charInput;  // input char
	int    keyModifiers;  //  key modifier (shift, ctrl)
} UIState;

static UIState 	mygs_UIState;

void display()
{
	int k;
	// ����
	if(isUpdate){
		DisplayClear();
	
		// ���ƺʹ���˵�
		drawMenu();
		drawpicture();
		DrawMusicDisplayButton();
	}
	
	if(isInput){
   		DrawInput();
   		//ShowInput=!ShowInput;
   }
    
	if(ShowDevelopInfor)
   {
   		DevelopInformation();
   }
   
   	if(ShowFunctionIntro)
   {
   		FunctionIntroduction();
   }
   
   if(!isLegal){
   		InvalidInput();
   }
   
   if(isSave!=0){
   		toSave();
   }
   
   if(isContinue){
   		toContinue();
   }
   
   if(isClear){
   		toClear();
   }
   
   if(isExit&&!isFlag){
   		toExitInformation();
   }
   
   if(isDerivation){
   	    InputDerivation();
   }
   
   if(isCalculus){
   	    InputCalculus();
   }
}

void drawMenu()
{ 
	static char * menuListDraw[] = {"Draw[D]",  
		"New Function | Ctrl-O", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"Clear All | Ctrl-E",
		"Save | Ctrl-S",
		"Continue | Ctrl-C",
		"Clear Old Data | Ctrl-D",
		"Exit | Ctrl-P"};
	static char * menuListTool[] = {"Tool[T]",
		"Derivative | Ctrl-F",
		"Calculus | Ctrl-L",
		"Crossover Points | Ctrl-H"};
	static char * menuListHelp[] = {"Help[H]",
		"Developer Information  | Ctrl-M",
		"Function Introduction | Ctrl-N"
		};

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListDraw[0])*2; // �ؼ����
	double wlist = TextStringWidth(menuListHelp[1])*1.2;
	double xindent = winheight/20; // ����
	int selection;
	
	
	// menu bar
	MovePen(0, y-h);
	DrawLine(winwidth,0);
	// Draw ��ͼ�˵� 
    selection=menuList(GenUIID(0), x, y-h, w, wlist, h, menuListDraw, sizeof(menuListDraw)/sizeof(menuListDraw[0]));
    switch (selection){
		case 0: break;
		case 1: 
			InitScreen();
			isInput=TRUE;
			break;
		case 2: 
			InitScreen();	
			InitPloynomial();
			MessageBox(NULL,TEXT("Clear All Success!"),TEXT("Sketchpad"),MB_OK);		 
			break;
		case 3:
			InitScreen();
			if(SaveInput()==0){
				isSave=-1;
			}
			else isSave=1;
			MessageBox(NULL,TEXT("Save Success!"),TEXT("Sketchpad"),MB_OK);
			break;
		case 4:
			InitScreen();
			isContinue=TRUE;
			break;
		case 5:
			InitScreen();
			isClear=TRUE;
			MessageBox(NULL,TEXT("Clear Old Data Success!"),TEXT("Sketchpad"),MB_OK);
			break;
		case 6: 
			InitScreen();
			if(isFlag) exit(0);
			isExit=TRUE;
			break;
		default: break;
	}
	// Tool ���߲˵�
	selection=menuList(GenUIID(0), x+w, y-h, w, wlist, h, menuListTool, sizeof(menuListTool)/sizeof(menuListTool[0]));
	 switch(selection)
	 {
		case 0:
			break;
	 	case 1:
	 		InitScreen();
	 		isDerivation=TRUE;
		 	break;
	 	case 2:
	 		InitScreen();
	 		isCalculus=TRUE;
		 	break;	
		case 3:
			InitScreen();
			isException=TRUE;
		 	break;		
	 	default:break;
	 }
	// Help �����˵�
	selection=menuList(GenUIID(0), x+w*2,y-h, w, wlist, h, menuListHelp, sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	switch(selection)
	{
		case 0:break;
		case 1:
			InitScreen();
			ShowDevelopInfor=TRUE;break; 
		case 2: 
			InitScreen();		
			ShowFunctionIntro=TRUE;break;
		default:break;
	}
}

void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event); //GUI��ȡ���
	MyuiGetMouse(x,y,button,event);
	double X,Y;
	double fh=GetFontHeight();//����߶� 
	double butw=10*TEXTWIDTH,buth=2*fh;
	int i,j,m=0;
	int k;
	
	X=ScaleXInches(x);
	Y=ScaleYInches(y);
	if(isInput){
		switch (event)
		{ 
		  case BUTTON_DOWN:
		  if(inBox(X,Y,winwidth/2-2*butw,winwidth/2+2*butw,winheight-7*buth,winheight-5*buth))
			{	
			} 
		  else if(inBox(X,Y,winwidth/2+22*TEXTWIDTH,winwidth/2+32*TEXTWIDTH,winheight-12*buth,winheight-11*buth))
		  {
		       UserInput[--Inputk]='\0';
		  }
		  else if(inBox(X,Y,winwidth/2-2*butw,winwidth/2+2*butw,winheight-12*buth,winheight-7*buth))
		  {	
		  	
			 for(i=0;i<=4;i++){
				for(j=0;j<=3;j++)
			   		if((i==4)&&(j==2||j==3)){
			     		if(j==2){
				       		if(inBox(X,Y,winwidth/2,winwidth/2+butw,winheight-12*buth,winheight-11*buth)){//�û�ѡ��OVER��ť 
				       			InitScreen();
			             	}
			           		else m++;
			    	}
				    	else if(inBox(X,Y,winwidth/2+butw,winwidth/2+2*butw,winheight-12*buth,winheight-11*buth)){//�û�ѡ��OK��ť 
				    		if(strlen(UserInput)==0){
				    			InitScreen();
							}
							else if(isStrLegal(UserInput)){
					    		ploynomial[function_num]=FinalSolution(UserInput);
					    		InitScreen();
					    		isFunction=TRUE;
				    			isUpdate=FALSE;
					    		DisplayClear();
					    		CoordinateAxis(o);
					    		for(k=0;k<function_num;k++){
					    			Calculate_Display(ploynomial[k],o,'r',k);
								}
								Calculate_Display(ploynomial[function_num],o,'n',0);
							}
							else{
								InitScreen();
								isLegal=FALSE;
							}
				    	}
			   	}	
			    else if(inBox(X,Y,winwidth/2+(j-2)*butw,winwidth/2+(j-1)*butw,winheight-(8+i)*buth,winheight-(7+i)*buth)){
					UserInput[Inputk++]=*sigh[m++];
	            }
	            	else m++;
	           }
			} 
			break;
		  case BUTTON_UP: break;
		}
	}	
	if(isDerivation)
	{
		    switch(event)
		    {
		    	case BUTTON_DOWN:
				        m=0;
			           if(inBox(X,Y,winwidth/2-40*TEXTWIDTH,winwidth/2-20*TEXTWIDTH,winheight-13.5*fh,winheight-5.5*fh)){
			                 for(i=0;i<10;i++){  //�˴�10Ӧ�滻Ϊfunction_num,10��������ʹ�� 
			                      if(inBox(X,Y,winwidth/2-40*TEXTWIDTH,winwidth/2-20*TEXTWIDTH,winheight-(7+1.5*i)*fh,winheight-(5.5+1.5*i)*fh))
			                         DerivationInput=Haha[i];
			                   }
			                 DerivationInput=NULL;
		               }
		               if(inBox(X,Y,winwidth/2+22*TEXTWIDTH,winwidth/2+35*TEXTWIDTH,winheight-20.5*fh,winheight-18.5*fh)){
		               	    if(isDex)
		               	    Dex[--indexdex]='\0';
		               	    else Dey[--indexdey]='\0';
					   }
		               if(inBox(X,Y,winwidth/2-19*TEXTWIDTH,winwidth/2+20*TEXTWIDTH,winheight-20.5*fh,winheight-12.5*fh)){
				            for(i=0;i<=3;i++){
		     		            for(j=0;j<=2;j++){
		     			           // if(i==3&&j==2){
		     				           if(inBox(X,Y,winwidth/2+7*TEXTWIDTH,winwidth/2+20*TEXTWIDTH,winheight-20.5*fh,winheight-18.5*fh)){
		     					            if(isDex)
		     					               isDex=FALSE;
		     					            else isDey=FALSE;
							            }
		     				 
					            //	}
						            else if(inBox(X,Y,winwidth/2+(13*j-19)*TEXTWIDTH,winwidth/2+(13*j-6)*TEXTWIDTH,winheight-(14.5+2*i)*fh,winheight-(12.5+2*i)*fh)){
							                if(isDex)
							                    Dex[indexdex++]=*sigh2[m++];
							                else Dey[indexdey++]=*sigh2[m++];
					             	}
						            else m++;
						 
					            }
				            }
			        } 
			        break;
			    case BUTTON_UP:break;
			    default:break;
		    }
		     
	}
	if(isCalculus){
		m=0;
		switch (event)
		{ 
		  case BUTTON_DOWN:
		   if(inBox(X,Y,winwidth/2+22*TEXTWIDTH,winwidth/2+32*TEXTWIDTH,winheight-11*buth,winheight-10*buth))
		  {
		       CalculusInput[--indexcal]='\0';
		  }
		  else if(inBox(X,Y,winwidth/2+22*TEXTWIDTH,winwidth/2+32*TEXTWIDTH,winheight-12*buth,winheight-11*buth))//�û�ѡ��OK��ť,���ջ�ͼ�ĵĿ��ܲ����ⲿ����Ҫ�޸� 
		  {
		  		        	if(strlen(CalculusInput)==0){
				    			InitScreen();
							}
							else if(isStrLegal(CalculusInput)){
					    		ploynomial[function_num]=FinalSolution(CalculusInput);
					    		InitScreen();
					    		isFunction=TRUE;
				    			isUpdate=FALSE;
					    		DisplayClear();
					    		CoordinateAxis(o);
					    		for(k=0;k<function_num;k++){
					    			Calculate_Display(ploynomial[k],o,'r',k);
								}
								Calculate_Display(ploynomial[function_num],o,'n',0);
							}
							else{
								InitScreen();
								isLegal=FALSE;
							}
		  }
		  else if(inBox(X,Y,winwidth/2-2*butw,winwidth/2+2*butw,winheight-12*buth,winheight-7*buth))
		  {	
		  	if(inBox(X,Y,winwidth/2,winwidth/2+butw,winheight-12*buth,winheight-11*buth))
			   {
				   	CalculusInput[indexcal++]='I';
				   	CalculusInput[indexcal++]=' ';
				}
			else if(inBox(X,Y,winwidth/2+butw,winwidth/2+2*butw,winheight-12*buth,winheight-11*buth))
				{
					CalculusInput[indexcal++]='d';
					CalculusInput[indexcal++]='X';
					
				}
			else {
			 for(i=0;i<=4;i++){
				for(j=0;j<=3;j++)
                   if(inBox(X,Y,winwidth/2+(j-2)*butw,winwidth/2+(j-1)*butw,winheight-(8+i)*buth,winheight-(7+i)*buth)){
					CalculusInput[indexcal++]=*sigh3[m++];
	            }
	            	else m++;
	           }				
			}

			} 
	
			break;
		  case BUTTON_UP: break;
		  default:break;
		}
	}
	display(); // ˢ����ʾ
}

void DrawInput(void)
{
	double lx,ly;//���뺯����������Ͻǵ�����
	double fh=GetFontHeight();//����߶� 
	int i,j,k=0;//ѭ������ 
	double butw=10*TEXTWIDTH,buth=2*fh;
	lx = winwidth/2-20*TEXTWIDTH;
	ly = winheight - fh*14;
	drawRectangle(lx,ly,40*TEXTWIDTH,fh*4,0);//��ʾ���봰��
	for(i=0;i<=4;i++)
	{
	   lx = winwidth/2-20*TEXTWIDTH;
	   ly = winheight - fh*(16+2*i);
	   for(j=0;j<=3;j++)//���ư��� 
	    {
			button(GenUIID(k),lx,ly,butw,buth,sigh[k]); 
	    	k++;
	    	lx += 10*TEXTWIDTH;
		}
	}
	drawLabel(winwidth/2-2*butw+TEXTWIDTH,winheight-6*buth,UserInput);	
	button(GenUIID(0),winwidth/2+22*TEXTWIDTH,winheight-24*fh,butw,buth,"Delete");
}

void KeyboardEventProcess(int key,int event)
{
	uiGetKeyboard(key,event);
	int i;
	//ͼ���ƶ�	
	if(isFunction){
	switch(event)
		{	
			case KEY_DOWN:
				switch(key)
				{
					case VK_UP:
						DisplayClear();
						o.y+=1;
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_DOWN:
						DisplayClear();
						o.y-=1;
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_LEFT:
						DisplayClear();
						o.x-=1;
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_RIGHT:
						DisplayClear();
						o.x+=1;
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_F5:
						isUpdate=TRUE;
						isFunction=FALSE;			
						o.x=GetWindowWidth()/2;
						o.y=GetWindowHeight()/2;
						break;
				}
				break;
			case KEY_UP:
				break;
		}
	}
	
	//�������󷵻� 
	if(!isLegal){
		switch(event)
		{	
			case KEY_DOWN:
				switch(key)
				{
					case VK_F5:
						isLegal=TRUE;
						break;
				}
				break;
		}
	}
	
	//����Ƿ��ȡ��ǰ�ĺ������� 
	if(isContinue){
		switch(event)
		{
			case BUTTON_DOWN:
				switch(key)
				{
					case 0x59:
						FileGetPloynomial();
						InitScreen();
					    isFunction=TRUE;
				    	isUpdate=FALSE;
				    	DisplayClear();
					    CoordinateAxis(o);
					    for(i=0;i<function_num;i++){
					    	Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case 0x4E:
						InitScreen();
						break;
				}
				break;
			case BUTTON_UP:
				break;
		}
	}
	
	//�˳�ʱ��ʾ��� 
	if(isExit){
		switch(event)
		{
			case BUTTON_DOWN:
				switch(key)
				{
					case 0x59:
						ClearFileData();
						exit(-1);
						break;
					case 0x4E:
						InitScreen();
						break;
				}
				break;
		}
	}
	display();
}

//��ʼ���������� 
void InitUserInputNULL(void)
{
	int i;
	for(i=0;i<StringLength;i++){
		UserInput[i]=NULL;
	}
}

static bool inBox(double x, double y, double x1, double x2, double y1, double y2)
{
	return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

static void DevelopInformation(void)
{
	double fh=GetFontHeight();//Ӣ������߶� 
	double fw1=2*TEXTWIDTH, fw2=TEXTWIDTH;//���ֿ�Ⱥ�Ӣ����ĸ��� 
	double center=winwidth/2,left=center-13*fw2;//�м����λ�úͿ�ͷ����λ�� 
	double inheight=winheight-7*fh;//��ʼ�ı���ʾ�߶� 
	double interval=fh,fh1=2*fh;//�м���ͺ��ָ߶� 
	double x,y;
	int i,j,k=1;
	
	drawLabel(left,inheight,DevelopInfor[0]);
	
	for(i=0;i<=3;i++)
	{
		y=inheight-2*fh-i*fh;
		for(j=0;j<=1;j++)
		{
			x=left+j*13*fw2;
			drawLabel(x,y,DevelopInfor[k++]);
		}
		
	}
	
}

static void FunctionIntroduction(void)
{
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-7*fh;
	drawLabel(lx,ly,FunctionIntro); 
}

static void InvalidInput(void)
{
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-12*fh;
	drawLabel(lx,ly,InvalidInformation); 
}

static void toException(void)
{
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-12*fh;
	drawLabel(lx,ly,Exception); 
}

static void toSave(void)
{
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-12*fh;
	if(isSave==1){
		drawLabel(lx,ly,Save[0]);
		isFlag=TRUE;
	}
	if(isSave==-1){
		drawLabel(lx,ly,Save[1]);
	}
}

static void toContinue(void)
{
	int i;
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-12*fh;
	if(CheckContinue()==1){
		for(i=0;i<3;i++){
			drawLabel(lx,ly,ContinueInformation[i]);
			ly-=fh; 
		}
	}
	if(CheckContinue()==-1){
		drawLabel(lx,ly,ContinueInformation[3]);
	}
}

static void toClear(void)
{
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-12*fh;
	if(ClearFileData()){
		drawLabel(lx,ly,ClearInformation[0]);
	}
	if(!ClearFileData()){
		drawLabel(lx,ly,ClearInformation[1]);
	}
}

static void toExitInformation(void)
{
	int i;
	double fh=GetFontHeight();//��������߶�
	double fw= 2*TEXTWIDTH;//���ֿ��
	double lx=winwidth/2-23*TEXTWIDTH;
	double ly=winheight-12*fh;
	for(i=0;i<3;i++){
		drawLabel(lx,ly,TipContinueInformation[i]);
		ly-=fh; 		
	}
}

void drawpicture(void)
{
	double fh=GetFontHeight();
	double butw=10*TEXTWIDTH,buth=2*fh;
	double starlength[3]={2*TEXTWIDTH,3*TEXTWIDTH,2*TEXTWIDTH};
	char *StarColor[]={"Galaxy Blue","Forest Biome","Niagara"};
	int i,j;
	mySetPenColor(StarColor[0]);
	cx=winwidth/2+3*butw;
	cy=winheight-5*buth;
	for(j=1;j<=3;j++)
	{
		if(j==2)
		{
			cx=winwidth/2+2*butw;
			cy=winheight-7*buth;
			mySetPenColor(StarColor[1]);
		}
		else if(j==3)
		{
			cx=winwidth/2+4*butw;
			cy=winheight-9*buth;
			mySetPenColor(StarColor[2]);
		}
		MovePen(cx,cy);
		angleX=-36;
		//StartFilledRegion(1);
	    for(i=1;i<=5;i++)
	   {
	     forward(starlength[0]);
         move(starlength[0]);
	     turn(144);
	   }
	  // EndFilledRegion();
	}
	//������
	MovePen(winwidth/2-3.5*butw,winheight-13*buth);
	StartFilledRegion(1);
	DrawArc(buth,0,360);
	EndFilledRegion();
	SetPenColor("black");
	MovePen(winwidth/2-3.5*butw,winheight-12.75*buth);
	DrawEllipticalArc(1.5*buth,0.5*buth,47,-274); 
    
    SetPenColor("Hazel");
	MovePen(winwidth/2-2*butw,winheight-14*buth);
	StartFilledRegion(1);
	DrawArc(buth,0,360);
	EndFilledRegion();
	SetPenColor("black");
	MovePen(winwidth/2-2*butw,winheight-13.75*buth);
	DrawEllipticalArc(1.5*buth,0.5*buth,47,-274); 
		
	SetPenColor("Pale Dogwood");
	MovePen(winwidth/2-4*butw,winheight-16*buth);
	StartFilledRegion(1);
	DrawArc(buth,0,360);
	EndFilledRegion();
	SetPenColor("black");
	MovePen(winwidth/2-4*butw,winheight-15.75*buth);
	DrawEllipticalArc(1.5*buth,0.5*buth,47,-274); 
	 
}


static void forward(double distance)
{
	DrawLine(distance*cos(angleX*PI/180),distance*sin(angleX*PI/180));
}

static void turn(double angle)
{
   angleX=angleX-angle;
}

static void move(double distance)
{
	cx=cx+cos(angleX*PI/180)*distance;
	cy=cy+sin(angleX*PI/180)*distance;
	MovePen(cx,cy);
}

static void InitScreen()
{
	InitUserInputNULL();
	Inputk=0;
	isInput=FALSE;
	isLegal=TRUE;
	isException=FALSE;
	ShowDevelopInfor=FALSE;
	ShowFunctionIntro=FALSE;
	isFunction=FALSE;
	isSave=0;
	isContinue=FALSE;
	isClear=FALSE;
	isExit=FALSE;
	isDerivation=FALSE;
	isCalculus=FALSE;
}

static void InitPloynomial()
{
	int i;
	for(i=0;i<=function_num;i++){
		ploynomial[i]=NULL;
	}
	function_num=0;
}

static void InputDerivation(void)
{
	double lx,ly;//���뺯����������Ͻǵ�����
	double fh=GetFontHeight();//����߶� 
	double butw=13*TEXTWIDTH,buth=2*fh;
	
	int i,j,k=0;//ѭ������ 
	
	char *dex="x=";
	char *dey="y=";
	lx = winwidth/2-19*TEXTWIDTH;
	ly = winheight - fh*14.5;

	button(GenUIID(0),winwidth/2+22*TEXTWIDTH,winheight-20.5*fh,butw,buth,"Delete");
	char *Text1="�˴���ʾ������ֵ";
	for(i=0;i<10;i++)//�˴�10Ӧ��Ϊfunction_num,�����ڲ���
	{
	button(GenUIID(i),winwidth/2-40*TEXTWIDTH,winheight-(7+1.5*i)*fh,20*TEXTWIDTH,1.5*fh,Haha[i]);//������к�������ѡ���󵼣�������ʾ��õ�����չʾ���� 
	}
	drawLabel(winwidth/2-19*TEXTWIDTH,winheight-7*fh,ShowDerivation);
	drawRectangle(winwidth/2-19*TEXTWIDTH,winheight-10*fh,40*TEXTWIDTH,2*fh,0); 
	drawLabel(winwidth/2-18*TEXTWIDTH,winheight-9.5*fh,Text1);//Text1��Ӧ�滻Ϊ����õĵ���ֵ, Text1��������ʹ�� 
	drawRectangle(lx,ly,39*TEXTWIDTH,fh*2,0);//��ʾ���봰��
	drawLabel(winwidth/2-18*TEXTWIDTH,winheight-11.5*fh,dex);
	drawLabel(winwidth/2-15.5*TEXTWIDTH,winheight-11.5*fh,Dex); //�˴���ȱ���������Ϊx���� 
	drawLabel(winwidth/2+TEXTWIDTH,winheight-11.5*fh,dey);
	drawLabel(winwidth/2+3.5*TEXTWIDTH,winheight-11.5*fh,Dey);  //�˴���ȱ�������Ϊy����   
	for(i=0;i<=3;i++)
	{
	   lx = winwidth/2-19*TEXTWIDTH;
	   ly = winheight - fh*(14.5+2*i);
	   for(j=0;j<=2;j++)//���ư��� 
	    {
			button(GenUIID(k),lx,ly,butw,buth,sigh2[k]); 
	    	k++;
	    	lx += 13*TEXTWIDTH;
		}
	}
}

static void InputCalculus(void)
{
	double lx,ly;//���뺯����������Ͻǵ�����
	double fh=GetFontHeight();//����߶� 
	int i,j,k=0;//ѭ������ 
	double butw=10*TEXTWIDTH,buth=2*fh;
	lx = winwidth/2-20*TEXTWIDTH;
	ly = winheight - fh*14;
	drawRectangle(lx,ly,40*TEXTWIDTH,fh*4,0);//��ʾ���봰��
	for(i=0;i<=4;i++)
	{
	   lx = winwidth/2-20*TEXTWIDTH;
	   ly = winheight - fh*(16+2*i);
	   for(j=0;j<=3;j++)//���ư��� 
	    {
			button(GenUIID(k),lx,ly,butw,buth,sigh3[k]); 
	    	k++;
	    	lx += 10*TEXTWIDTH;
		}
	}
	drawLabel(winwidth/2-2*butw+TEXTWIDTH,winheight-6*buth,CalculusInput);	
	button(GenUIID(0),winwidth/2+22*TEXTWIDTH,winheight-22*fh,butw,buth,"Delete");
	button(GenUIID(0),winwidth/2+22*TEXTWIDTH,winheight-24*fh,butw,buth,"OK");
}

static void DrawBox(double x, double y, double width, double height)
{
    MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
}

static void DrawMusicTriangle(double length_of_side)
{
	DrawLine((sqrt(3)/3)*length_of_side,0);
	DrawLine((-sqrt(3)/2)*length_of_side,0.5*length_of_side);
	DrawLine(0,-length_of_side);
	DrawLine((sqrt(3)/2)*length_of_side,0.5*length_of_side);
}

static void DrawMusicTriangle_opposite(double length_of_side)
{
	DrawLine((-sqrt(3)/3)*length_of_side,0);
	DrawLine((sqrt(3)/2)*length_of_side,0.5*length_of_side);
	DrawLine(0,-length_of_side);
	DrawLine((-sqrt(3)/2)*length_of_side,0.5*length_of_side);
}

static DrawMusicBox(double length_of_side)
{
	DrawLine(0.5*length_of_side,0.5*length_of_side);
	DrawLine(-length_of_side,0);
	DrawLine(0,-length_of_side);
	DrawLine(length_of_side,0);
	DrawLine(0,length_of_side);
}

static int MyButton(int id, double x, double y, int type,char *frameColor,char *filledColor)
{
	double movement = 0.02;
	double sinkx = 0, sinky = 0;
	
	//�������Χ�� 
	if(inBox(mygs_UIState.mousex, mygs_UIState.mousey, x-0.14, x+2*0.14, y-0.14, y+2*0.14)) 
	{
		mygs_UIState.actingMenu = 0;
		if(mygs_UIState.mousedown) //������ƶ� 
		{
			mygs_UIState.clickedItem = id;
			sinkx = movement;
			sinky = -movement;
		}
	}
	else 
	{
		if(mygs_UIState.clickedItem==id)
	    mygs_UIState.clickedItem = 0;
	}
	
	
	switch(type) //ѡ��ť���� 
	{
	    case 1: //���� 
	    	if(!isMusic){
		   	    SetPenColor(filledColor); //Ȧ 
		        MovePen(x+2.4*0.08+sinkx,y+0.08+sinky); 
		        StartFilledRegion(1);
		        DrawArc(2*0.08,0,360);
		        EndFilledRegion();
		        SetPenColor(frameColor);//���� 
		        MovePen(x+0.4*0.08+sinkx,y+0.08+sinky);
		        StartFilledRegion(1);
		        DrawMusicTriangle(0.22);
		        EndFilledRegion();
	    	}
	    	else{
	    		SetPenColor(filledColor); //Ȧ 
		        MovePen(x+2.4*0.08+sinkx,y+0.08+sinky); 
		        StartFilledRegion(1);
		        DrawArc(2*0.08,0,360);
		        EndFilledRegion();
		        SetPenColor(frameColor);//���� 
		        MovePen(x+0.4*0.08+sinkx,y+0.08+sinky);
		        StartFilledRegion(1);
		        DrawMusicBox(0.15);
		        EndFilledRegion();
			}
		break;
		case 2://���ֿ��
			SetPenColor(filledColor); //Ȧ 
		    MovePen(x+2.4*0.08+sinkx,y+0.08+sinky); 
		    StartFilledRegion(1);
		    DrawArc(2*0.08,0,360);
			EndFilledRegion();
		    SetPenColor(frameColor);//���� 
		    MovePen(x+sinkx,y+0.08+sinky);
		    StartFilledRegion(1);
		    DrawMusicTriangle(0.15);
		    EndFilledRegion();
		    MovePen(x+1*0.08+sinkx,y+0.08+sinky);
		    StartFilledRegion(1);
		    DrawMusicTriangle(0.15);
		    EndFilledRegion();
		break;
		case 3:
			SetPenColor(filledColor); //Ȧ 
		    MovePen(x+2.4*0.08+sinkx,y+0.08+sinky); 
		    StartFilledRegion(1);
		    DrawArc(2*0.08,0,360);
			EndFilledRegion();
		    SetPenColor(frameColor);//���� 
		    MovePen(x+sinkx,y+0.08+sinky);
		    StartFilledRegion(1);
		    DrawMusicTriangle_opposite(0.15);
		    EndFilledRegion();
		    MovePen(x+1*0.08+sinkx,y+0.08+sinky);
		    StartFilledRegion(1);
		    DrawMusicTriangle_opposite(0.15);
		    EndFilledRegion();
		break;
	}

	if(mygs_UIState.clickedItem==id && !mygs_UIState.mousedown)  
	{
		mygs_UIState.clickedItem = 0;
		mygs_UIState.kbdItem = id;
		return 1; 
	}

	return 0;
}

static void MyuiGetMouse(int x, int y, int button, int event)
{
	mygs_UIState.mousex = ScaleXInches(x); //pixels --> inches
	mygs_UIState.mousey = ScaleYInches(y); //pixels --> inches

	switch (event) 
	{
	    case BUTTON_DOWN:
		    mygs_UIState.mousedown = 1;
		break;
	    case BUTTON_UP:
		    mygs_UIState.mousedown = 0;
		break;
	}
}

static void DrawMusicDisplayButton()
{
	int i;
	char MusicComment[40];
	if(MyButton(GenUIID(0), WinWidth*0.80, (WinHeight-GetFontHeight())*0.1, 1,"Jason Wu","Chicory Coffee")){
		if(!isMusic){
			isMusic=TRUE;
			strcpy(MusicComment,"play ");
			strcat(MusicComment,".\\Music\\");
			strcat(MusicComment,MusicName[CurrentMusic]);
			strcat(MusicComment," repeat");
			mciSendString(MusicComment,0,NULL,0);
		}
		else{
			isMusic=FALSE;
			strcpy(MusicComment,"pause ");
			strcat(MusicComment,".\\Music\\");
			strcat(MusicComment,MusicName[CurrentMusic]);
			mciSendString(MusicComment,0,NULL,0);
		}
	}
	if(MyButton(GenUIID(0), WinWidth*0.85, (WinHeight-GetFontHeight())*0.1, 2,"Jason Wu","Chicory Coffee")){
		if(isMusic){
			strcpy(MusicComment,"pause ");
			strcat(MusicComment,".\\Music\\");
			strcat(MusicComment,MusicName[CurrentMusic]);
			mciSendString(MusicComment,0,NULL,0);
		}
		CurrentMusic++;
		if(CurrentMusic>=MusicNumber) CurrentMusic=0;
		if(isMusic){
			isMusic=TRUE;
			strcpy(MusicComment,"play ");
			strcat(MusicComment,".\\Music\\");
			strcat(MusicComment,MusicName[CurrentMusic]);
			strcat(MusicComment," repeat");
			mciSendString(MusicComment,0,NULL,0);
		}
	}
	if(MyButton(GenUIID(0), WinWidth*0.75, (WinHeight-GetFontHeight())*0.1, 3,"Jason Wu","Chicory Coffee")){
		if(isMusic){
			strcpy(MusicComment,"pause ");
			strcat(MusicComment,".\\Music\\");
			strcat(MusicComment,MusicName[CurrentMusic]);
			mciSendString(MusicComment,0,NULL,0);
		}
		CurrentMusic--;
		if(CurrentMusic<0) CurrentMusic=MusicNumber-1;
		if(isMusic){
			isMusic=TRUE;
			strcpy(MusicComment,"play ");
			strcat(MusicComment,".\\Music\\");
			strcat(MusicComment,MusicName[CurrentMusic]);
			strcat(MusicComment," repeat");
			mciSendString(MusicComment,0,NULL,0);
		}		
	}
}
