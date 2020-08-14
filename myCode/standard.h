#ifndef _Standard_h
#define _Standard_h
#define EPS 0.000001
#define MAX 5000 
typedef struct Polynomial{/*polynomial������ʽ*/ 
	int cnt;/*��cnt��*/ 
	double coef[MAX];/*coef��ϵ��*/ 
	double pow[MAX];/*pow���ݴ�*/ 
}PN; /*��PN�ṹ��洢����ʽ��������ϵ������ָ��*/ 
int Dequal(double a,double b);/*double ���*/ 
int isDight(char a);/*ĳ�ַ��ǲ������ֻ���С����*/ 

void PrintPN(PN a);/*��һ��PN�ṹ�������Ϊ�ı��ļ�*/
//void PrintList(struct List* ptr); /*��ӡһ�������������á���*/

int isStrLegal(char* a);/*�ж��ַ����Ϸ���*/ 

/*IO�������ض���*/ 
int Read(char* str,int pstr);/*��ȡһ�����ṹ��*/ /*һ��Ķ����ǣ������ֻ�X��ʼ����op����*/ 
void AddTwoPN(PN* a,PN* b);/*��������ʽ���,�����һ��*/ 
void MultiTwoPN(PN* a,PN* b);/*���*/ 
void MinusTwoPN(PN* a,PN* b);/*���*/ 
void SortPN(PN* a);/*��ĳ����ʽ����ʹ���ظ���ϲ���ϵ��Ϊ�����޳�����������*/ 

/*IO�������ض���*/ 

/*RPN���*/ 
PN ReadPN(int len,int p,int* q,char* s);/*����һ������ʽ ��len���ܳ��ȣ�p���ӵ�pλ��ʼ����q���ض�ȡֹͣ��λ����q��s������ȡ�ַ���)*/ 
double ReadDight(int len,int p,int* q,char* s);/*����һ�������� ��len���ܳ��ȣ�p���ӵ�pλ��ʼ����q���ض�ȡֹͣ��λ����q��s������ȡ�ַ���)*/ 
void Calc();/*���㣬������ջ*/ 
int OpLevel(char op);/*����������op�����������ȼ�*/ 
/*RPN���*/

PN Rpn(char* str);/*�沨��ʽ������*/ 
struct List* Change(PN a);/*�ṹ��ת����*/ 

struct List* FinalSolution(char* str);/*�������Ϻ���*/
#endif
