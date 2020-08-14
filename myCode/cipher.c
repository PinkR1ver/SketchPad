#include<stdio.h>
#include<windows.h>
#include<string.h>
#include"graphics.h"


void isBuy()
{
	int i=0;
	FILE *fp;
	int sBufSize = MAX_COMPUTERNAME_LENGTH +1;
	char computerName[sBufSize];
	char fileContent[sBufSize];
	DWORD bufSizeP = sBufSize;
	GetComputerName(computerName,&bufSizeP);
	fp=fopen(".\\ciphertext\\ciphertext.txt","r");
	if(fp==NULL){
		InitConsole();
		printf("you don't have the cipher text,sorry\n");
		system("pause");
		exit(-1);
	}
	fgets(fileContent,sBufSize,fp);
	if(strcmp(fileContent,computerName)!=0){
		InitConsole();
		printf("you don't have the cipher text,sorry\n");
		system("pause");
		exit(-1);		
	}
	fclose(fp);
}
