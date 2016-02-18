/*
 * CommonUtility.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: root
 */
#include "CommonUtility.h"

void GetDateTimeFileName(char *pFileName)
{
	int hours, minutes, seconds;
	int month, day, year;
//	GetSystemTime(&hours, &minutes, &seconds);
//	GetSystemDate(&month, &day, &year);
	sprintf(pFileName, "%4d%02d%02d%02d%02d%02d",  year, month, day, hours, minutes, seconds);
}

void GetFullFileNameInAppFolder(char *pFielName, char *pPathName)
{
//	char projectDir[MAX_PATHNAME_LEN];
//	if (GetProjectDir(projectDir) < 0)
// 		return;
// 	else
//		MakePathname(projectDir, pFielName, pPathName);
}

void StrTimeToIntTime(char *time, int *Hours, int *Minutes, int *Seconds )
{
//    char strHours[3], strMinutes[3],strSeconds[3];
//
//    strHours[0]=time[0];
//    strHours[1]=time[1];
//    strHours[2]='\0';
//    Fmt(Hours,"%d<%s",strHours);
//
//    strMinutes[0]=time[3];
//    strMinutes[1]=time[4];
//    strMinutes[2]='\0';
//    Fmt(Minutes,"%d<%s",strMinutes);
//
//    strSeconds[0]=time[6];
//    strSeconds[1]=time[7];
//    strSeconds[2]='\0';
//    Fmt(Seconds,"%d<%s",strSeconds);
}

void StrDateToIntDate(char *date,int *Month, int *Day, int *Year)
{
//    char strYear[5],strMonth[3],strDay[3];
//
//    strYear[0]=date[0];
//    strYear[1]=date[1];
//    strYear[2]=date[2];
//    strYear[3]=date[3];
//    strYear[4]='\0';
//    Fmt(Year,"%d<%s",strYear);
//
//    strMonth[0]=date[5];
//    strMonth[1]=date[6];
//    strMonth[2]='\0';
//    Fmt(Month,"%d<%s",strMonth);
//
//    strDay[0]=date[8];
//    strDay[1]=date[9];
//    strDay[2]='\0';
//    Fmt(Day,"%d<%s",strDay);
}

void EnumSerialPort(int hPanel, int nComCtrlID)
{
//	char string[512];
//	unsigned int size1,size2,values,i, k;
//	int type;
//	char valueName[256];
//
//	RegQueryInfoOnKey(REGKEY_HKLM, "Hardware\\DeviceMap\\SerialComm",
//	                   NULL, &values, NULL, NULL, NULL);
//	for(i=0; i < values; i++)
//	{
//		size1 = 256;
//		size2 = 512;
//		RegEnumerateValue(REGKEY_HKLM, "Hardware\\DeviceMap\\SerialComm",
//		                   i, valueName, &size1, string, &size2,
//		                   &type);
//		k = atoi(&string[3]);				   //COMk---ÌáÈ¡k
//		InsertListItem(hPanel, nComCtrlID, -1, string, k);   //ÁÐ±í¿òÏîÖµ
//	}
}

int Sec2Str(int Second, char *Str, int Format24, int FormatHMS)
{

    int Sec, Min, Hour;
    int CpySecond=Second;

    Hour = Second/3600;
    Second = Second - (3600*Hour);
    if(Format24==AM_PM)
        Hour%=12;
    else
        Hour%=24;

    Min = Second/60;
    Second = Second - (60*Min);
    Sec = Second;
    switch(FormatHMS)
	{
        case HHMM:
            sprintf(Str, "%02d:%02d", Hour, Min);
        break;
        case HHMMSS:
            sprintf(Str, "%02d:%02d:%02d", Hour, Min, Sec);
        break;
    }
    Second=CpySecond;
    if (Format24==AM_PM){
        if (Second>=43200)          /* 43200=12*3600 */
            strcat(Str, " PM");
        else
            strcat(Str, " AM");
    }
    return(0);
}

int Str2Sec(char *Str, int *Second)
{
    char *bob;
    int i=3600;

    *Second=0;
    bob = strtok (Str, ":");
    while(bob)
	{
        *Second = *Second+ i*atoi(bob);
        i=i/60;
        bob = strtok (NULL, ":");
    }
    return(0);
}
void GetCurrentTimeChar(char *timetemp)
{
//	double currDateTime;
//	int bufferLen;
//	char *dateTimeBuffer = NULL;
//	char reciveBuf[100];
//	GetCurrentDateTime (&currDateTime);
//	bufferLen = FormatDateTimeString (currDateTime, "%I:%M:%S", NULL, 0);
//	dateTimeBuffer = malloc (bufferLen + 1);
//	FormatDateTimeString (currDateTime, "%I:%M:%S", dateTimeBuffer, bufferLen + 1 );
//	strcpy(timetemp,dateTimeBuffer);
}
void HexStrToUChar(char *Str,unsigned char *chartemp)
{
	if(strlen(Str)>0)
	{
		unsigned char *strtemp;
		int len=0;
		len=strlen(Str);
		if(len%2!=0)
		{
			len++;
			sprintf(Str,"0%s",Str);
		}
		memset(chartemp,0,len/2);
		strtemp=(unsigned char*)malloc(len/2);
		memset(strtemp,0,sizeof(strtemp));
		for(int i=0;i<len/2;i++)
		{
			int num;
			char temp[2];
			strncpy(temp,Str,2);
			sscanf(temp,"%x",&num);
			strtemp[i]=num;
			Str=Str+2;
		}
		memcpy(chartemp,strtemp,len/2);
	}
}
void UCharStrToUInt(unsigned char *chartemp,unsigned int *num)
{
    *num=chartemp[0]*pow(256,0)+chartemp[1]*pow(256,1)+chartemp[2]*pow(256,2)+chartemp[3]*pow(256,3);
}
void UCharArrayToHexStr(unsigned char *UChar,int count,char *str)
{
	//memset(str,0,2*count+1);
	char a[3];
	for(int i=0;i<count;i++)
	{
		if(UChar[i]>15)
		{
			sprintf(a,"%x",UChar[i]);
		}
		else if(UChar[i]>=0&&UChar[i]<=15)
		{
			sprintf(a,"0%x",UChar[i]);
		}
		str[i*2]=toupper(a[0]);
		str[i*2+1]=toupper(a[1]);
	}
}


unsigned long long bswap_64(unsigned long long x) {
    x= ((x<< 8)&0xFF00FF00FF00FF00ULL) | ((x>> 8)&0x00FF00FF00FF00FFULL);
    x= ((x<<16)&0xFFFF0000FFFF0000ULL) | ((x>>16)&0x0000FFFF0000FFFFULL);
    return (x>>32) | (x<<32);
}



