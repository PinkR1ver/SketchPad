#ifndef _funcitionDisplay
#define _functionDisplay

struct List{
	double coefficient;
	double power;
	struct List *next;
};//�����������ʽ 

typedef struct List List;

struct Calculus_data{
	double up_limit;
	double down_limit;
	int function_serial;
};//������ֵĽṹ�� 

typedef struct Calculus_data Calculus_data;

struct Derivation_data{
	double local;
	int function_serial;
};//���㵼���Ľṹ�� 

typedef struct Derivation_data Derivation_data;

struct Coordinate{
	double x;
	double y;
};

typedef struct Coordinate Coordinate;

void PickLine(double x1,double y1,double x2,double y2);
//���� 

void CoordinateAxis(Coordinate OriginalPoint);
//�������� 

void Calculate_Display(List *ploynomial,Coordinate OriginalPoint,char judge,int f_num);
//��ͼ�� 

/* 
void printploynomial(List *ploynomial);
��console����ʾ����ʽ 
Calculus_data Input_Tip_Calculus(void);
console�е���ʾ 
*/

double Calculus(Calculus_data Data,List *ploynomial);
//������� 

double price(List *ploynomial,double x);

//Derivation_data Input_Tip_Derivation(void); 

double Derivation(Derivation_data Data,List *ploynomial);
//���㵼�� 

void MouseEventProcessFunctionMove(int x, int y, int button, int event); 

void KeyboardEventProcessFunctionMove(int key,int event);

List *ConsoleGetPloynomial(void);

void DisPlay_Image_resolution(char *choose);

void ConsolePrintploynomial(List *ploynomial);

#endif
