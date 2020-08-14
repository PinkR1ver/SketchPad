#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include<math.h>
#include<windows.h>
#include "graphics.h"
#include "genlib.h"
#include"functiondisplay.h"

#define DX 0.001

int function_num;
extern isFunction;

Coordinate o;	

List *ploynomial[10];

//static int DX=0.001;

static char *LondonColor[]={
	"Summer Fig",
	"Antique Moss",
	"Galaxy Blue",
	"Chicory Coffee",
	"Forest Biome",
	"Hazel",
	"Grapeade",
	"Butterscotch"
};//来自London时尚周的配色


//连线 
void PickLine(double x1,double y1,double x2,double y2)
{
	double dx=x2-x1,dy=y2-y1;
	MovePen(x1,y1);
	DrawLine(dx,dy);
}

#ifdef DEBUG
//未实现 
void DisPlay_Image_resolution(char *choose)
{
	if(strcmp(choose,"higher resolution")==0){
		DX=DX/10;
	}
	if(strcmp(choose,"lower resolution")==0){
		DX=DX*10;
	}
}
#endif


//Console读取数据，调试用 
List *ConsoleGetPloynomial(void)
{
	List *head,*tail,*p;
	head=NULL;
	tail=NULL;
	int n,i;
	printf("input your number of terms\n");
	scanf("%d",&n);
	printf("input your ax^(b), frist a, second b\n");
	for(i=0;i<n;i++){
	 	p=(List*)malloc(sizeof(List));
	 	scanf("%lf",&p->coefficient);
	 	scanf("%lf",&p->power);
	 	p->next=NULL;
	 	if(head==NULL)
	 		head=p;
	 	else 
	 		tail->next=p;
	 	tail=p;
	}
	printf("\n");
	system("pause");
	return head;
}

//计算函数值 
double price(List *ploynomial,double x)
{
	double sum=0;
	List *ptr;
	for(ptr=ploynomial->next;ptr!=NULL;ptr=ptr->next){
		//printf("coefficient = %f ,power = %f ,",ptr->coefficient,ptr->power);
		sum+=(ptr->coefficient)*pow(x,ptr->power);
		//printf("sum = %f\n",sum);
	}
	return sum;
}

//画坐标轴 
void CoordinateAxis(Coordinate OriginalPoint)
{
	int i;
	double j;
	char *label[]={"0","1","2","3","4","5","6","-1","-2","-3","-4","-5","-6"};
	double WindowHeight=GetWindowHeight();
	double WindowWidth=GetWindowWidth();
	//画格子
	/*
	SetPenColor("Gray");
	for(i=2;i<=4;i++){
		MovePen(0,WindowHeight/2+i);
		DrawLine(WindowWidth,0);
		MovePen(0,OriginalPoint.x-i);
		DrawLine(WindowWidth,0);
	}
	*/
	//画坐标轴 
	SetPenColor("Black");
	MovePen(0,OriginalPoint.y);
	DrawLine(WindowWidth,0);
	DrawLine(-0.05,0.05);
	MovePen(WindowWidth,OriginalPoint.y);
	DrawLine(-0.05,-0.05);
	MovePen(OriginalPoint.x,0);
	DrawLine(0,WindowHeight);
	DrawLine(-0.05,-0.05);
	MovePen(OriginalPoint.x,WindowHeight);
	DrawLine(0.05,-0.05);
	//画点
	for(j=0.5;j<=6;j+=0.5){
		MovePen(OriginalPoint.x+j,OriginalPoint.y);
		DrawLine(0,0.02);
		MovePen(OriginalPoint.x-j,OriginalPoint.y);
		DrawLine(0,0.02); 
	} 
	for(j=0.5;j<=3;j+=0.5){
		MovePen(OriginalPoint.x,OriginalPoint.y+j);
		DrawLine(0.03,0);
		MovePen(OriginalPoint.x,OriginalPoint.y-j);
		DrawLine(0.03,0);
	}
	//画数据
//	for(i=1;i<=6;i++){
//		SetPointSize(0.1);
//		drawLabel(OriginalPoint.x+i,OriginalPoint.y,label[i]);
//		drawLabel(OriginalPoint.x-i,OriginalPoint.y,label[i+6]);
//	} 
//	for(i=1;i<=6;i++){
//		SetPointSize(0.1);
//		drawLabel(OriginalPoint.x,OriginalPoint.y+i,label[i]);
//		drawLabel(OriginalPoint.x,OriginalPoint.y-i,label[i+6]);
//	}	 
	for(i=1;i<=6;i++){
		SetPointSize(5); 
		MovePen(OriginalPoint.x+i,OriginalPoint.y-0.15);
		DrawTextString(label[i]);
		MovePen(OriginalPoint.x-i,OriginalPoint.y-0.15);
		DrawTextString(label[i+6]);
	} 
	for(i=1;i<=6;i++){
		MovePen(OriginalPoint.x,OriginalPoint.y+i);
		DrawTextString(label[i]);
		MovePen(OriginalPoint.x,OriginalPoint.y-i);
		DrawTextString(label[i+6]);
	}
}

//画图像 
void Calculate_Display(List *ploynomial,Coordinate OriginalPoint,char judge,int f_num)
{
	if(judge=='r') SetPenColor(LondonColor[f_num]);
	else{
		SetPenColor(LondonColor[function_num]);
		function_num++;
	}
	//double WindowHeight=GetWindowHeight();
	//double WindowWidth=GetWindowWidth();
	double i;
	double y1,y2;
	for(i=-8;i<8;i+=DX){
		y1=0,y2=0;
		y1=price(ploynomial,i);
		y2=price(ploynomial,i+DX);
		PickLine(OriginalPoint.x+i,OriginalPoint.y+y1,OriginalPoint.x+i+0.001,OriginalPoint.y+y2);
	}
}

//计算积分 
double Calculus(Calculus_data Data,List *ploynomial)
{
	//DX/=100;
	int flag=1;
	double i,sum=0;
	List *ptr;
	if(Data.up_limit<Data.down_limit){
		double tmp;
		tmp=Data.up_limit;
		Data.up_limit=Data.down_limit;
		Data.down_limit=tmp;
		flag=-1;
	}
	for(i=Data.down_limit;i<=Data.up_limit;i+=DX) sum+=price(ploynomial,i)*DX;
	//DX*=100;
	return sum*flag;
}

//计算导数 
double Derivation(Derivation_data Data,List *ploynomial)
{
	//DX/=100;
	double dy=price(ploynomial,Data.local+DX)-price(ploynomial,Data.local);
	//DX*=100;
	return dy/DX;
}

/*
void MouseEventProcessFunctionMove(int x, int y, int button, int event)
{
	static bool isDraw = FALSE;
	
	static double omx = 0.0, omy = 0.0;
    double mx, my;
	
	mx = ScaleXInches(x);
 	my = ScaleYInches(y);
 	
	switch(event)
	{
		case BUTTON_DOWN:
			if( button = LEFT_BUTTON){
				isDraw = TRUE;
			}
			omx = mx; omy = my;
			break;
		case BUTTON_DOUBLECLICK:
			break;
		case BUTTON_UP:
			if(button == LEFT_BUTTON) isDraw = FALSE;
			break;
		case MOUSEMOVE:
			if(isDraw){
				SetEraseMode(TRUE);
				CoordinateAxis(o);
				Calculate_Display(ploynomial[0],o);
				o.x+=mx-omx;
				o.y+=my-omy;
				omx = mx;
				omy = my; 
				SetEraseMode(FALSE);
				CoordinateAxis(o);
				Calculate_Display(ploynomial[0],o);
			}
			break;
	}
}
*/

/*
void KeyboardEventProcessFunctionMove(int key,int event)
{
	int i;
	if(isFunction){
	switch(event)
		{	
			case KEY_DOWN:
				switch(key)
				{
					case VK_UP:
						SetEraseMode(TRUE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						o.y+=1;
						SetEraseMode(FALSE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_DOWN:
						SetEraseMode(TRUE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						o.y-=1;
						SetEraseMode(FALSE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_LEFT:
						SetEraseMode(TRUE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						o.x-=1;
						SetEraseMode(FALSE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
					case VK_RIGHT:
						SetEraseMode(TRUE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						o.x+=1;
						SetEraseMode(FALSE);
						CoordinateAxis(o);
						for(i=0;i<function_num;i++){
							Calculate_Display(ploynomial[i],o,'r',i);
						}
						break;
				}
				break;
			case KEY_UP:
				break;
		}
	}
}
*/

//Console中输出数据检测 
void ConsolePrintploynomial(List *ploynomial)
{
	printf("y = ");
	List *ptr;
	ptr=ploynomial;
	printf("%.3fx^(%.0f)",ptr->coefficient,ptr->power);
	ptr=ptr->next;
	if(ptr!=NULL){
		for(;ptr!=NULL;ptr=ptr->next){
			printf(" + %.3fx^(%.0f)",ptr->coefficient,ptr->power);
		}
	}
	printf("\n");
}
