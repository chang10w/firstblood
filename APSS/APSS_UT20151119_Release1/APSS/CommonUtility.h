/*
 * CommonUtility.h
 *
 *  Created on: Oct 29, 2015
 *      Author: root
 */

#ifndef COMMONUTILITY_H_
#define COMMONUTILITY_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#define HHMM  0
#define HHMMSS  1
#define AM_PM 2

void GetDateTimeFileName(char *pFileName);

void StrTimeToIntTime(char *time,int *Hours, int *Minutes, int *Seconds );

void StrDateToIntDate(char *date,int *Month, int *Day, int *Year);

void EnumSerialPort(int hPanel, int nComCtrlID);

void GetFullFileNameInAppFolder(char *pFielName, char *pPathName);

int Sec2Str(int Second, char *Str, int Format24, int FormatHMS);

int Str2Sec(char *Str, int *Second);
void GetCurrentTimeChar(char *timetemp);
void HexStrToUChar(char *Str,unsigned char *chartemp);
void UCharStrToUInt(unsigned char *chartemp,unsigned int *num);
void UCharArrayToHexStr(unsigned char *UChar,int count,char *str);

unsigned long long bswap_64(unsigned long long x);

#endif /* COMMONUTILITY_H_ */
