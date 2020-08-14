/*进行逆波兰式变换时
 *重定义读入，将每一项读入结构体 
 *用重定义四则运算进行化简*/ 
#include <stdio.h>
#include <math.h> 
#include <stdlib.h>
#include <stddef.h>

#include "standard.h"
#include "functiondisplay.h"
static int pstr0;
static PN temp1,temp2,empty;


void AddTwoPN(PN* a,PN* b)
{
	int i=0,j=0;
	int flag[MAX]={0};
	//SortPN(a);
	//SortPN(b);
	for(i=0;i<a->cnt;i++)
	{
		for(j=0;j<b->cnt;j++)
		{
			if(Dequal(a->pow[i],b->pow[j]))
			{
				a->coef[i]+=b->coef[j];
				flag[j]=1;
			}
		}
	}
	for(j=0;j<b->cnt;j++)
	{
		if(!flag[j])
		{
			a->coef[a->cnt]=b->coef[j];
			a->pow[a->cnt]=b->pow[j];
			a->cnt++;
		}
	}
	SortPN(a);
}

void MinusTwoPN(PN* a,PN* b)
{
	int j=0;
	for(j=0;j<b->cnt;j++)
	{
		b->coef[j]*=(-1);
	}
	AddTwoPN(a,b);
}

void MultiTwoPN(PN* a,PN* b)
{
	int ptr=0; 
	PN* ptmp1=&temp1, *ptmp2=&temp2;
	temp2.cnt=0;
	temp2.coef[0]=0;
	temp2.pow[0]=0;
	int i=0,j=0;
	for(i=0;i<a->cnt;i++)
	{
		temp1.cnt=0;
		//if(!a->coef[i]) continue;
		for(j=0;j<b->cnt;j++)
		{
			//if(!b->coef[j]) continue;
			temp1.coef[j]=a->coef[i]*b->coef[j];
			temp1.pow[j]=a->pow[i]+b->pow[j];
			temp1.cnt++;
		}
		///puts("ptmp1:"); PrintPN(*ptmp1); puts("end");
		AddTwoPN(ptmp2,ptmp1);
		///puts("ptmp2:"); PrintPN(*ptmp2); puts("end");
		//temp2.cnt=ptr--;
	}
	*a=*ptmp2;
}

void PrintPN(PN a)
{
	FILE *fp=NULL;
	fp=fopen("test.txt", "a");
	int i=0,j=0;
	for(i=0;i<a.cnt;i++)
	{
		fprintf(fp,"(%.3f)X^(%.3f)",a.coef[i],a.pow[i]);
		if(i!=a.cnt-1) fputc('+',fp);
		//printf("(%.3f)X^(%.3f)\n",a.coef[i],a.pow[i]);
	}
	fputc('\n',fp);
	fclose(fp);
}

int Dequal(double a,double b)/*Double equal*/ 
{
	if(fabs(a-b)<EPS) return 1;
	return 0;
}
void SortPN(PN* a)
{
	int i=0,j=0,ptr=0;
	double min=-2147483647,tmp; 
	PN* b=&temp1;
	for(j=0;j<=a->cnt;j++)
	{
		tmp=2147483647;
		for(i=0;i<a->cnt;i++)
		{
			if(a->pow[i]>min && a->pow[i]<tmp)
			{
				tmp=a->pow[i];
			}	
		}
		if(Dequal(tmp,2147483647))
		{
			b->cnt=ptr;
			*a=*b;
			return;
		}
		min=tmp;
		b->pow[ptr]=tmp;
		b->coef[ptr]=0;
		for(i=0;i<a->cnt;i++)
		{
			if(Dequal(a->pow[i],tmp))
			{
				b->coef[ptr]+=a->coef[i];
			}	
		}
		if(b->coef[ptr]) ptr++;
	}
} 

char op[MAX];
int oTop,eTop;
PN elmt[MAX];

int OpLevel(char op)
{
	if('('==op) return 0;
	if('+'==op ||'-'==op)return 1;
	if('*'==op)return 2;
	else return -1;

} 

int isDight(char a)
{
	if((a>='0' && a<='9') ||a=='.')return 1;
	return 0;
}
void Calc()
{
	PN val;
	//printf("calc before:eTop:%d\n",eTop);
	//PrintPN(elmt[eTop-1]);puts("end"); 
	//printf("%c\n",op[oTop]);puts("end");
	//PrintPN(elmt[eTop]);puts("end");
	
	if(op[oTop]=='*') MultiTwoPN(&elmt[eTop-1],&elmt[eTop]);
	else if(op[oTop]=='+') AddTwoPN(&elmt[eTop-1],&elmt[eTop]);
	else if(op[oTop]=='-') MinusTwoPN(&elmt[eTop-1],&elmt[eTop]);
	oTop--;
	eTop--;
	//printf("calc after\n");
	//PrintPN(elmt[eTop]);puts("end");
	//elmt[eTop++]=val;
}

double ReadDight(int len,int p,int* q,char* s)
{
	double v=0;
	int t=0,flag=0;
	static char tmp[MAX];
	if('-'==s[p]) flag=1,p++;
	while(p<len && isDight(s[p]))
	{
		tmp[t++]=s[p++];
	}
	tmp[t]=0;
	sscanf(tmp,"%lf",&v);
	if(flag)v=-v;
	*q=p;
	return v;	
}
PN ReadPN(int len,int p,int* q,char* s)
{
	PN* a;
	a=&temp1;
	double coef=0,pow=0;
	//puts("!");
	if(!isDight(s[p]))//s[p]=='X'
	{
		coef=1;
		p++;
		if(s[p]!='^') 
		{
			pow=1;
			*q=p;
		}
		else if(s[p+1]=='(')
		{
			p++;
			p++;
			//pow=114514;
			pow=ReadDight(len,p,q,s);
			(*q)++;
		}
		else pow=ReadDight(len,p+1,q,s);
	}
	else
	{
		coef=ReadDight(len,p,q,s);
		p=*q;
		if(s[p]!='X')
		{
			pow=0;
		}
		else ///s[p]=='X'
		{
			p++;
			if(s[p]!='^')pow=1,*q=p;
			else if(s[p+1]=='(')
			{
				p++;
				p++; 
				pow=ReadDight(len,p,q,s);
				(*q)++;
			}
			else pow=ReadDight(len,p+1,q,s);
		}
	}
	a->cnt=1;
	a->coef[0]=coef;
	a->pow[0]=pow;
	//printf("%f %f\n",coef,pow);
	return *a;
}

PN Rpn(char* str)
{
	PN tmp;
	int len=strlen(str);
	int p=0; 
	int q;
	//printf("str:%s\nlen:%d\n",str,len);
	while(p<len)
	{
		//puts("!");
		if(isDight(str[p]) || 'X'==str[p])
		{
			elmt[++eTop]=ReadPN(len,p,&q,str);
		}
		else/*是op*/ 
		{
			
			if(')'==str[p]) {
			
				while(op[oTop]!='(') 
				{
					Calc();
				}
				oTop--;
			}
			else if('('==str[p]) op[++oTop]=str[p];
			else {
				while(oTop && OpLevel(str[p])<OpLevel(op[oTop])) Calc();
				op[++oTop]=str[p];
			}
			q=p+1;
		}
		p=q;
		//printf("p:%d\n",p); 
	}
	while(oTop>=0) Calc();/////这句话有bug' 
	//puts("QWQ\n");
	//SortPN(&elmt[1]);
	return elmt[1];
}

struct List *head=NULL,*pre,*newNode;
List* Change(PN a)
{
	int i=0;
	head=(List*)malloc(sizeof(List));
	pre = head;
	for(i=0;i<a.cnt;i++)
	{
		newNode=(struct List*)malloc(sizeof(struct List));
		newNode->coefficient = a.coef[i];
		newNode->power = a.pow[i];
		pre->next = newNode;
		pre = pre->next;
	}
	pre->next = NULL;
	//*cnt=a.cnt;
	return head;

}
//void PrintList(struct List* ptr)
//{
//	while(ptr)
//	{
//		ptr=ptr->next;
//		printf("(%f)X^(%f)\n",ptr->coefficient,ptr->power);
//	}
//}
 int isOp(char a)
 {
 	if(a=='+'||a=='-'||a=='*')return 1;
 	return 0;
 }
int isStrLegal(char* str)
{
	int i=0,bracket=0/*括号对应性*/,legal=1/*合法布尔值*/,dot=0/*单点性*/;
	int len=strlen(str);
	for(i=0;i<len-1;i++)
	{
		if(str[i]=='(')
		{
			dot=0;
			bracket++;
			if(str[i+1]=='('||(str[i+1]>='0' && str[i+1]<='9')||str[i+1]=='X'||str[i+1]=='-');
			else
			{
				legal=0;
				break;
			}
		}
		if(str[i]==')')
		{
			dot=0;
			bracket--;
			if(str[i+1]==')'||isOp(str[i+1]));
			else
			{
				legal=0;
				break;
			}
		}
		if(str[i]=='X')
		{
			dot=0;
			if(str[i+1]=='^'||str[i+1]==')'||isOp(str[i+1]));
			else
			{
				legal=0;
				break;
			}
		}
		if(str[i]=='^')
		{
			dot=0;
			if((str[i+1]>='0' && str[i+1]<='9')||'('==(str[i+1]));
			else
			{
				legal=0;
				break;
			}
		}
		if(str[i]=='.')
		{
			if(dot)
			{
				legal=0;
				break;
			}
			dot=1;
			if(str[i+1]>='0' && str[i+1]<='9');
			else
			{
				legal=0;
				break;
			}
		}
		if(str[i]>='0' && str[i]<='9')
		{
			if((str[i+1]>='0' && str[i+1]<='9')||str[i+1]=='.'||
				str[i+1]==')'||str[i+1]=='X'||isOp(str[i+1]));
			else
			{
				legal=0;
				break;
			}
		}
		if(isOp(str[i]))
		{
			dot=0;
			if((str[i+1]>='0' && str[i+1]<='9')||str[i+1]=='X'||str[i+1]=='(');
			else
			{
				legal=0;
				break;
			}
		}
	}
	/*末位特判*/
	if(str[len-1]==')') bracket--;
	if(str[len-1]=='('||str[len-1]=='^'||str[len-1]=='.'||
		isOp(str[len-1])) legal=0;
	if(bracket!=0) legal=0;/*括号对应*/
	return legal;
} 
struct List* FinalSolution(char* str)
{
	InitStandard();
	struct List* head;
	//char* str;
	//scanf("%s",&str);
	//str="((2.5X)*(5X)+(10X-1X^2))";
	if(isStrLegal(str))
	{
		PN res=Rpn (str);
		PrintPN(res);
		//printf("\n");
		head=Change(res);
	}
	else
	{
		printf("Error: Invaild Input\n"); 
		head= NULL;
	} 
	//PrintList(head);
	//puts("!");
	return head;
}
void InitStandard(void)
{
	head=NULL,pre=NULL,newNode=NULL;
	temp1=empty; 
	temp2=empty;
	op[MAX]=0;
	oTop=0,eTop=0;
	memset(elmt,0,sizeof(elmt));
}
//int main()
//{
//	char* str;
//	str="X^2+2X+1";
//	FinalSolution(str);
//	//str="X";
//	///FinalSolution(str);
//	//str="2";
//	//FinalSolution(str);
//	return 0; 
// } 
/*合法的字符
 * 0123456789+-*.()X^
 * op：+-*
 * elmt: DOUBLE X^ DOUBLE
 *		 DOUBLE
 *		 X^DOUBLE
 *		 X
 * DOUBLE: 0123456789. 
 *		   ONE OR ZREO .
 *		   NO FIRST . NO LAST . 
 *数字 、op、（、） 、X 、^、点 
 */ 

/*合法的字符串
 * （：前是op或（；后是（或数字或X    d
 * ）：前是数字或）或X；后是op或）    d
 * （）的对应性                       d
 * op：前是数字或X或）；后是数字或X或（   d 
 *数字：后是数字或点或X或）或op；前是数字或op或^或（ 或点    d
 *点：前后都数字                       d
 *单点性                               d
 *X：前是数字或（或op；后是^或op或）   d
 *^：前X后数字                         d
 */


/*补漏
 *（后可-
 *^后可（ 
 */ 





/*  ((2.5X)*(5X)+(10X-1X^2)) 
 *
 */
