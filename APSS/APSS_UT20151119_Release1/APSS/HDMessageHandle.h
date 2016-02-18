/*
 * HDMessageHandle.h
 *
 *  Created on: Oct 28, 2015
 *      Author: root
 */

#ifndef HDMESSAGEHANDLE_H_
#define HDMESSAGEHANDLE_H_

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlstring.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <libxml/xmlwriter.h>
#include <list>
#include "CommonUtility.h"

/**
 * appMessage:applicationRegister/applicationUnRegister/unSubscribe/heartbeat
 * dataMessage:other send message
 */
typedef enum {
	appMessage, dataMessage
} HD_MESSAGE_TYPE;

typedef enum {
	applicationRegister,
	applicationUnRegister,
	getAvionicsData,
	setAvionicsData,
	subscribeAvionicsData,
	subscribeAvionicsDataOnEvent,
	unSubscribe,
	heartbeat,
	applicationRegisterResponse,
	applicationUnRegisterResponse,
	getAvionicsDataResponse,
	setAvionicsDataResponse,
	subscribeAvionicsDataResponse,
	subscribeAvionicsDataOnEventResponse,
	unSubscribeResponse,
	heartbeatResponse,
	invalidMsg
} HD_MESSAGE_ENUM;

/************** APS types only ************************/
typedef enum {
	getAvionicParameters,
	subscribeAvionicParameters,
	subscribeAvionicParametersOnEvent,
	apsunSubscribe,
	getAvionicParametersResponse,
	subscribeAvionicParametersResponse,
	publishAvionicParameters,
	subscribeAvionicParametersOnEventResponse,
	OnEventpublishAvionicParameters,
	apsunSubscribeResponse,
	apsInvalidMsg,
	apsInvalidXML
} APS_MESSAGE_ENUM;

typedef enum  {
	TYPE_F64B=0,
	TYPE_S64B,
	TYPE_F32B,
	TYPE_U32B,
	TYPE_S32B,
	TYPE_BOOL,
	TYPE_STRING,
	TYPE_DATE,
	TYPE_TIME,
	TYPE_OPAQUE,
	TYPE_ENUM,
	TYPE_UNKNOW,
} APS_DATA_TYPE_ENUM;

typedef enum {
	ND = 0, NO = 1, NCD = 2, FT = 3, NF = 4,
} APS_STATUS_ENUM;

typedef struct {
	char signame[50];
	int status;
	int type;
	unsigned char value[255];
	int length;
	double time;
} APS_DATA_APPLICATION;

typedef struct {
	char methodname[50];
	unsigned int publishPort[5];
	unsigned int publishPortNum;
} APS_APP_REQUST_DATA;

typedef struct {
	char methodname[50];
	unsigned int errcode;
} APS_APP_RESPONES_DATA;

typedef struct {
	char methodname[50];
	unsigned int publishPort[5];
	unsigned int publishPortNum;
	unsigned int refreshperiod;
	APS_DATA_APPLICATION signal[10];
	int signalnum;
	APS_MESSAGE_ENUM eventType;
} APS_DATA_REQUEST;

//Parameter data value
typedef struct {
	double value1;
	long long value2;
	float value3;
	unsigned int value4;
	int value5;
	bool value6;
	char value[80];
} CAPSParaVal;

typedef struct {
	char methogname[50];
	unsigned int errcode;
	double time;
	APS_MESSAGE_ENUM type;
	CAPSParaVal ParamVale[10];
	CAPSParaVal TempParamVale[10];
	APS_DATA_APPLICATION signal[10];
	int signalnum;
} APS_DATA_RESPONSE;

/*ST_RAWDATA_APPLICATION*/
typedef struct {
	char rawDataName[50];
	unsigned int validity;
	unsigned int type;
	char interfaceName[20];
	unsigned int length;
	char crc[50];
	char data[1600];
	char ICDid[50];
	double time;
} ST_RAWDATA_APPLICATION;

/*ST_APP_REQUEST_DATA*/
typedef struct {
	char methodName[30];
	char appName[30];
	char partNum[30];
	unsigned int processID;
	char passwd[20];
	char platform[20];
} ST_APP_REGISTER_DATA;

/*ST_APP_RESPONSE_DATA*/
typedef struct {
	char resName[30];
	unsigned int errorCode;
	char appName[30];
	char signature[50];
	unsigned int validity;
} ST_APP_REGISTER_RESPONSE_DATA;

typedef struct {
	char methodName[30];
	char appName[30];
	char signature[50];
} ST_APP_UNREGISTER_DATA;

typedef struct {
	char resName[30];
	unsigned int errorCode;
} ST_APP_UNREGISTER_RESPONSE_DATA;

/*ST_DATA_REQUEST*/
typedef struct {
	char methodName[50];
	char appName[30];
	char partNum[30];
	unsigned int processID;
	char passwd[20];
	char platform[20];
	char signature[50];
	unsigned int publishPort;
	unsigned int unSubscribePublishPort[5];
	unsigned int unSubscribePublishPortNum;
	unsigned int refreshPeriod;
	ST_RAWDATA_APPLICATION rawDataApp[10];
	unsigned int rawDataNum;
} ST_DATA_REQUEST;

/*ST_DATA_RESPONSE*/
typedef struct {
	char resName[50];
	unsigned int errorCode;
	ST_RAWDATA_APPLICATION rawDataApp[10];
	unsigned int rawDataNum;
	char appName[30];
	char signature[50];
	unsigned int publishPort[5];
	unsigned int publishPortNum;
} ST_DATA_RESPONSE;

typedef struct {
	char methodName[50];
	char appName[30];
	char signature[50];
	unsigned int block;
} ST_HEARTBEAT_REQUEST;

typedef struct {
	char resName[50];
	unsigned int errorCode;
	unsigned int block;
	double time;
} ST_HEARTBEAT_RESPONSE;

/*******************************************aps types**************************************/
typedef enum {
	Sampling, Queuing, SAP
} ComPort;

//FeildProperty
typedef struct {
	char feildID[50];
	char feildName[50];
	char feildType[50];
	float scalarPattern;
	int feildSize;
	int startBit;
	int length;
	int offset;
} CFeildProerty;

//SignalProperty
typedef struct {
	char interfaceName[50];
	char ICDid[50];
	char signalName[50];
	int length;
	unsigned char data[1600];
	char direction[5];
	ComPort comport;
	int frequency;
	CFeildProerty field[4];
	int feildnum;
} CSignalProperty;

//ParameterStruct
typedef struct {
	char Name[50];
	int Id;
	int Status;
	APS_DATA_TYPE_ENUM Type;
	int Length;
	unsigned char Value[255];
	CAPSParaVal ParamVale;
	double time;
	CSignalProperty signel[2];
	int paramnum;
	int sigflag;
} CParamData;

typedef struct {
	char Aircraft_Type[50];
	char Version[50];
	char APS_ID[50];
	char APS_Name[50];
	char PartNumber[50];
	char FileTime[50];
	char Password[50];
	char Platform[50];
	char Signature[50];
	int ProcessId;
	char APS_Addr[50];
	char APSvr_Port_PARAM[50];
	char APSvr_Port_AIDCP[50];
	char AIDCP_Addr[50];
	char AIDCP_Port[50];
	char BIT_Addr[50];
	char BIT_Port[50];
	char Multicast_Addr[50];
	char Multicast_Port[50];
} APS_CONFIG;

/******************************************* aps types end **************************************/

namespace HDServer {

class HDMessageHandle {
public:
	HDMessageHandle();
	virtual ~HDMessageHandle();
private:
	static xmlNodeSetPtr GetNodeSet(xmlDocPtr doc, xmlChar* xpath);
	static void SetStrProperty(char* prop, xmlNodePtr node, const char* propNm);
	static void SetUCharStrToUIntProperty(unsigned int* prop, xmlNodePtr node,
			const char* propNm);
	static void SetUintProperty(unsigned int* prop, xmlNodePtr node,
			const char* propNm);
	static void SetDoubleProperty(double* prop, xmlNodePtr node,
			const char* proNm);
	static void SetStrValue(char* prop, xmlNodePtr node);
	static void SetUintValue(unsigned int* prop, xmlNodePtr node);
	static void CreateUintProperty(unsigned int num, xmlNodePtr node,
			xmlChar* propNm);
	static void CreateDoubleProperty(double num, xmlNodePtr node,
			xmlChar* propNm);
public:
	// Received Message from app
	static HD_MESSAGE_ENUM GetMessageName(char* buffer, int bufferSize);
	static void GetRegisterData(ST_APP_REGISTER_DATA* _data, char* buffer,
			int bufferSize);
	static void GetUnregisterData(ST_APP_UNREGISTER_DATA* _data, char* buffer,
			int bufferSize);
	static void GetAvionicsData(ST_DATA_REQUEST* _data, char* buffer,
			int bufferSize);
	static void GetSetAvionicsData(ST_DATA_REQUEST* _data, char* buffer,
			int bufferSize);
	static void GetSubscribeAvionicsData(ST_DATA_REQUEST* _data, char* buffer,
			int bufferSize);
	static void GetSubscribeAvionicsDataOnEvent(ST_DATA_REQUEST* _data,
			char* buffer, int bufferSize);
	static void GetUnSubscribe(ST_DATA_REQUEST* _data, char* buffer,
			int bufferSize);
	static void GetHeartBeat(ST_HEARTBEAT_REQUEST* _data, char* buffer,
			int bufferSize);

	// Response Message send to app
	static void SetRegisterDataPesponse(ST_APP_REGISTER_RESPONSE_DATA response,
			char* buffer);
	static void SetUnregisterDataPesponse(
			ST_APP_UNREGISTER_RESPONSE_DATA response, char* buffer);
	static void SetGetAvionicsDataPesponse(ST_DATA_RESPONSE response,
			char* buffer);
	static void SetAvionicsDataPesponse(ST_DATA_RESPONSE response,
			char* buffer);
	static void SetSubscribeAvionicsDataPesponse(ST_DATA_RESPONSE response,
			char* buffer);
	static void SetSubscribeAvionicsDataOnEventPesponse(
			ST_DATA_RESPONSE response, char* buffer);
	static void SetUnSubscribePesponse(ST_DATA_RESPONSE response, char* buffer);
	static void SetHeartBeatPesponse(ST_HEARTBEAT_RESPONSE response,
			char* buffer);

	// Sync message send to app
	static void SetPeriodicMessage(ST_DATA_RESPONSE response, char* buffer);
	static void SetEventMessage(ST_DATA_RESPONSE response, char* buffer);

	/******************************for APSS*********************************/
	static void GetPublishAvionicsDataPesponse(ST_DATA_RESPONSE* _data,
			char* buffer, int bufferSize);
	static void GetSubscribeAvionicsDataPesponse(ST_DATA_RESPONSE* _data,
			char* buffer, int bufferSize);
	static void GetUnSubscribePesponse(ST_DATA_RESPONSE* _data, char* buffer,
			int bufferSize);
	static void GetHeartBeatResponse(ST_HEARTBEAT_RESPONSE* _data, char* buffer,
			int bufferSize);
	static void GetPeriodicMessage(ST_DATA_RESPONSE* _data, char* buffer,
			int bufferSize);
	static void GetEventMessage(ST_DATA_RESPONSE* _data, char* buffer,
			int bufferSize);

	static void SetSubscribeAvionicsData(ST_DATA_REQUEST data, char* buffer);
	static void SetUnSubscribe(ST_DATA_REQUEST data, char* buffer);
	static void SetHeartBeat(ST_HEARTBEAT_REQUEST data, char* buffer);

	static void SetRegisterData(ST_APP_REGISTER_DATA _data, char* buffer);
	static void GetRegisterResponseData(ST_APP_REGISTER_RESPONSE_DATA* _data,
			char* buffer, int bufferSize);

	static void SetUnregisterData(ST_APP_UNREGISTER_DATA data, char* buffer);
	static void GetUnRegisterDataPesponse(
				ST_APP_UNREGISTER_RESPONSE_DATA* _data, char* buffer,int bufferSize);
	/******************************for APSS client *********************************/
	static APS_MESSAGE_ENUM GetApsMessageName(char* buffer, int bufferSize);
	static void GetApsRequestMsg(APS_DATA_REQUEST* _data, char* buffer,
			int bufferSize);
	static void SetApsResponseMsg(APS_DATA_RESPONSE data, char* buffer, bool isMethod);
	static void CreateGenericProperty(double num, xmlNodePtr node,
			xmlChar* propNm, char* fmt);
	static void CreateLongLongProperty(unsigned long long num, xmlNodePtr node,xmlChar* propNm);
	static void CreateFloatProperty(float num, xmlNodePtr node,	xmlChar* propNm);


};
} /* namespace ThreadTest */

#endif /* HDMESSAGEHANDLE_H_ */
