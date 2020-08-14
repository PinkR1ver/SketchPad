#ifndef _funcitionDisplay
#define _functionDisplay

struct List{
	double coefficient;
	double power;
	struct List *next;
};//用链表储存多项式 

typedef struct List List;

struct Calculus_data{
	double up_limit;
	double down_limit;
	int function_serial;
};//计算积分的结构体 

typedef struct Calculus_data Calculus_data;

struct Derivation_data{
	double local;
	int function_serial;
};//计算导数的结构体 

typedef struct Derivation_data Derivation_data;

struct Coordinate{
	double x;
	double y;
};

typedef struct Coordinate Coordinate;

void PickLine(double x1,double y1,double x2,double y2);
//连线 

void CoordinateAxis(Coordinate OriginalPoint);
//画坐标轴 

void Calculate_Display(List *ploynomial,Coordinate OriginalPoint,char judge,int f_num);
//画图像 

/* 
void printploynomial(List *ploynomial);
在console中显示多项式 
Calculus_data Input_Tip_Calculus(void);
console中的提示 
*/

double Calculus(Calculus_data Data,List *ploynomial);
//计算积分 

double price(List *ploynomial,double x);

//Derivation_data Input_Tip_Derivation(void); 

double Derivation(Derivation_data Data,List *ploynomial);
//计算导数 

void MouseEventProcessFunctionMove(int x, int y, int button, int event); 

void KeyboardEventProcessFunctionMove(int key,int event);

List *ConsoleGetPloynomial(void);

void DisPlay_Image_resolution(char *choose);

void ConsolePrintploynomial(List *ploynomial);

#endif
