#include<stdio.h>
#include<windows.h>
#include<stdlib.h>

int main()
{
	FILE *fp;
	fp=fopen("ciphertext.txt","w");
	int sBufSize = MAX_COMPUTERNAME_LENGTH +1;
	char computerName[sBufSize];
	DWORD bufSizeP = sBufSize;
	GetComputerName(computerName,&bufSizeP);
	if(fp == NULL){
    	printf("fail to open the file! \n");
    	system("pause");
    	exit(-1);
    }
    fputs(computerName,fp);
    fclose(fp);
    return 0;
}
