#include<stdio.h>
//#include<Mmsystem.h>
#include<windows.h>
#include"MusicPlay.h"

void MusicPlay()
{
	　　char buf[50];
　　　　MCIERROR mciError;
　　　　mciError=mciSendString("opencdaudio",buf,strlen(buf),NULL);
　　　　if(mciError)
　　　　{
　　　　　　mciGetErrorString(mciError,buf,strlen(buf));
　　　　　　AfxMessageBox(buf);
　　　　　　return;
　　　　}
}
