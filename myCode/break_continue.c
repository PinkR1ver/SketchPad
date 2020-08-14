#include<stdio.h>
#include<stdlib.h>
#include"standard.h"
#include"functiondisplay.h"
#include"MYGUI.H"

extern int function_num;
extern List *ploynomial[10];

static int toCoefficient(List *ploynomial);
static void FilePirintPloynomial(List *ploynomial);

static FILE *fp;

extern char MusicName[10][20];
extern int MusicNumber;

int SaveInput()
{
	int i;
	fp=fopen("function_save.txt","w");
	if(fp==NULL){
		return -1;
	}
	fprintf(fp,"%d\n",function_num);
	for(i=0;i<function_num;i++){
		fprintf(fp,"%d\n",toCoefficient(ploynomial[i]));
		FilePirintPloynomial(ploynomial[i]);
	}
	fclose(fp);
	return 1; 
}

int CheckContinue(void)
{
	int flag;
	fp=fopen("function_save.txt","r");
	if(fp==NULL){
		return -1;
	}
	fscanf(fp,"%d",&flag);
	if(flag==0) return -1; 
	fclose(fp);
	return 1;
}

void FileGetPloynomial(void)
{
	int n,i,k;
	fp=fopen("function_save.txt","r");
	fscanf(fp,"%d",&function_num);
	for(k=0;k<function_num;k++){
		List *head,*tail,*p;
		head=NULL;
		tail=NULL;
		fscanf(fp,"%d",&n);
		for(i=0;i<n;i++){
		 	p=(List*)malloc(sizeof(List));
		 	fscanf(fp,"%lf",&p->coefficient);
		 	fscanf(fp,"%lf",&p->power);
		 	p->next=NULL;
		 	if(head==NULL)
		 		head=p;
		 	else 
		 		tail->next=p;
		 	tail=p;
		}
		ploynomial[k]=head;
	}
	fclose(fp);
}

int ClearFileData(void)
{
	fp=fopen("function_save.txt","w");
	if(fp==NULL) return 0;
	fprintf(fp,"%d",0);
	fclose(fp);
	return 1;
}

static int toCoefficient(List *ploynomial)
{
	int sum=0;
	List *ptr;
	for(ptr=ploynomial;ptr!=NULL;ptr=ptr->next){
		sum++;
	}
	return sum;
}

static void FilePirintPloynomial(List *ploynomial)
{
	List *ptr;
	for(ptr=ploynomial;ptr!=NULL;ptr=ptr->next){
		fprintf(fp,"%f %f\n",ptr->coefficient,ptr->power);
	}
	fprintf(fp,"\n");
}

int FileGetMusicName()
{
	char tmp;
	int n,i,j;
	fp=fopen(".\\Music\\MusicSave.txt","r");
	if(fp==NULL){
		return -1;
	}
	fscanf(fp,"%d",&n);
	MusicNumber=n;
	tmp=fgetc(fp);
	for(i=0;i<n;i++){
		j=0;
		while((tmp=fgetc(fp))!='\n'){
			MusicName[i][j++]=tmp;
		}
		MusicName[i][j]='\0';
	}
	fclose(fp);
	return 1;
}
