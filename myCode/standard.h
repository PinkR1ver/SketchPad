#ifndef _Standard_h
#define _Standard_h
#define EPS 0.000001
#define MAX 5000 
typedef struct Polynomial{/*polynomial，多项式*/ 
	int cnt;/*共cnt项*/ 
	double coef[MAX];/*coef，系数*/ 
	double pow[MAX];/*pow，幂次*/ 
}PN; /*用PN结构体存储多项式的项数、系数、幂指数*/ 
int Dequal(double a,double b);/*double 相等*/ 
int isDight(char a);/*某字符是不是数字或者小数点*/ 

void PrintPN(PN a);/*将一个PN结构体输出成为文本文件*/
//void PrintList(struct List* ptr); /*打印一个链表（—调试用—）*/

int isStrLegal(char* a);/*判断字符串合法性*/ 

/*IO及运算重定义*/ 
int Read(char* str,int pstr);/*读取一项，存入结构体*/ /*一项的定义是：从数字或X开始，到op结束*/ 
void AddTwoPN(PN* a,PN* b);/*将两多项式相加,存入第一项*/ 
void MultiTwoPN(PN* a,PN* b);/*相乘*/ 
void MinusTwoPN(PN* a,PN* b);/*相减*/ 
void SortPN(PN* a);/*将某多项式整理，使得重复项合并，系数为零项剔除，升幂排列*/ 

/*IO及运算重定义*/ 

/*RPN相关*/ 
PN ReadPN(int len,int p,int* q,char* s);/*读入一个多项式 （len，总长度；p，从第p位开始读；q返回读取停止的位置是q；s，待读取字符串)*/ 
double ReadDight(int len,int p,int* q,char* s);/*读入一整个数字 （len，总长度；p，从第p位开始读；q返回读取停止的位置是q；s，待读取字符串)*/ 
void Calc();/*计算，批量出栈*/ 
int OpLevel(char op);/*给定操作符op，返回其优先级*/ 
/*RPN相关*/

PN Rpn(char* str);/*逆波兰式主函数*/ 
struct List* Change(PN a);/*结构体转链表*/ 

struct List* FinalSolution(char* str);/*最终整合函数*/
#endif
