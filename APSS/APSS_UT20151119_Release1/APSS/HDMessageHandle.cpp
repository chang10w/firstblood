/*
 * HDMessageHandle.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: root
 */

#include "HDMessageHandle.h"
using namespace std;
namespace HDServer {

HDMessageHandle::HDMessageHandle() {
	// TODO Auto-generated constructor stub

}

HDMessageHandle::~HDMessageHandle() {
	// TODO Auto-generated destructor stub
}

/************private function begin**************/

xmlNodeSetPtr HDMessageHandle::GetNodeSet(xmlDocPtr doc, xmlChar* xpath) {
	xmlXPathObjectPtr obj;
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	obj = xmlXPathEvalExpression(xpath, context);
	if (obj == NULL) {
		return NULL;
	}

	xmlXPathFreeContext(context);
	if (xmlXPathNodeSetIsEmpty(obj->nodesetval)) {
		xmlXPathFreeObject(obj);
		return NULL;
	} else {
		return obj->nodesetval;
	}
}

void HDMessageHandle::SetStrProperty(char* prop, xmlNodePtr node,
		const char* propNm) {
	xmlChar* _tempStr = NULL;
	_tempStr = xmlGetProp(node, (const xmlChar*) propNm);
	if (_tempStr != NULL) {
		strcpy(prop, (const char*) _tempStr);
		//printf("*******inset: %d,%s\n", strlen((const char*)_tempStr), _tempStr);
	} else {
		printf(
				"HDMessageHandle::SetStrProperty propNm:%s invalid or not exist!\n");
	}
}

void HDMessageHandle::SetUCharStrToUIntProperty(unsigned int* prop,
		xmlNodePtr node, const char* propNm) {
	string _tempStr = (char*) xmlGetProp(node, (const xmlChar*) propNm);
	if (!_tempStr.empty()) {
		unsigned int intTemp;
		UCharStrToUInt((unsigned char*) const_cast<char*> (_tempStr.c_str()),
				&intTemp);
		*prop = intTemp;
	} else {
		printf(
				"HDMessageHandle::SetUCharStrToUIntProperty propNm:%s invalid or not exist!\n");
	}
}

void HDMessageHandle::SetUintProperty(unsigned int* prop, xmlNodePtr node,
		const char* propNm) {
	string _tempStr = (char*) xmlGetProp(node, (const xmlChar*) propNm);
	if (!_tempStr.empty()) {
		*prop = boost::lexical_cast<unsigned int>(_tempStr);
	} else {
		printf(
				"HDMessageHandle::SetUintProperty propNm:%s invalid or not exist!\n");
	}
}

void HDMessageHandle::SetDoubleProperty(double* prop, xmlNodePtr node,
		const char* propNm) {
	string _tempStr = (char*) xmlGetProp(node, (const xmlChar*) propNm);
	if (!_tempStr.empty()) {
		*prop = boost::lexical_cast<double>(_tempStr);
	} else {
		printf(
				"HDMessageHandle::SetDoubleProperty propNm:%s invalid or not exist!\n");
	}
}

void HDMessageHandle::SetStrValue(char* prop, xmlNodePtr node) {
	xmlChar* _tempStr = xmlNodeGetContent(node);
	if (_tempStr != NULL) {
		strcpy(prop, (const char*) _tempStr);
	} else {
		printf("HDMessageHandle::SetStrValue invalid or not exist!\n");
	}
}

void HDMessageHandle::SetUintValue(unsigned int* prop, xmlNodePtr node) {
	string _tempStr = (char*) xmlNodeGetContent(node);
	if (!_tempStr.empty()) {
		*prop = boost::lexical_cast<unsigned int>(_tempStr);
	} else {
		printf("HDMessageHandle::SetUintValue invalid or not exist!\n");
	}
}

void HDMessageHandle::CreateUintProperty(unsigned int num, xmlNodePtr node,
		xmlChar* propNm) {
	char tempStr[32];
	sprintf(tempStr, "%d", num);
	xmlNewProp(node, propNm, BAD_CAST tempStr);
}

void HDMessageHandle::CreateDoubleProperty(double num, xmlNodePtr node,
		xmlChar* propNm) {
	char tempStr[32];
	sprintf(tempStr, "%lf", num);
	xmlNewProp(node, propNm, BAD_CAST tempStr);
}

void HDMessageHandle::CreateFloatProperty(float num, xmlNodePtr node,
		xmlChar* propNm) {
	char tempStr[32];
	sprintf(tempStr, "%f", num);
	xmlNewProp(node, propNm, BAD_CAST tempStr);
}

void HDMessageHandle::CreateLongLongProperty(unsigned long long num,
		xmlNodePtr node, xmlChar* propNm) {
	char tempStr[32];
	sprintf(tempStr, "%lld", num);
	xmlNewProp(node, propNm, BAD_CAST tempStr);
}

void HDMessageHandle::CreateGenericProperty(double num, xmlNodePtr node,
		xmlChar* propNm, char* fmt) {
	char tempStr[32];
	sprintf(tempStr, fmt, num);
	xmlNewProp(node, propNm, BAD_CAST tempStr);
}

/************private function end****************/

// Received Message from app
HD_MESSAGE_ENUM HDMessageHandle::GetMessageName(char* buffer, int bufferSize) {
	char* HDMessageNameArr[16] = { "applicationRegister",
			"applicationUnRegister", "getAvionicsData", "setAvionicsData",
			"subscribeAvionicsData", "subscribeAvionicsDataOnEvent",
			"unSubscribe", "heartbeat", "applicationRegisterResponse",
			"applicationUnRegisterResponse", "getAvionicsDataResponse",
			"setAvionicsDataResponse", "subscribeAvionicsDataResponse",
			"subscribeAvionicsDataOnEventResponse", "unSubscribeResponse",
			"heartbeatResponse" };
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	xmlChar* nameProp = xmlGetProp(root, (const xmlChar*) "name");
	HD_MESSAGE_ENUM msgEnum;
	if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[0])) {
		msgEnum = applicationRegister;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[1])) {
		msgEnum = applicationUnRegister;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[2])) {
		msgEnum = getAvionicsData;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[3])) {
		msgEnum = setAvionicsData;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[4])) {
		msgEnum = subscribeAvionicsData;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[5])) {
		msgEnum = subscribeAvionicsDataOnEvent;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[6])) {
		msgEnum = unSubscribe;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[7])) {
		msgEnum = heartbeat;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[8])) {
		msgEnum = applicationRegisterResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[9])) {
		msgEnum = applicationUnRegisterResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[10])) {
		msgEnum = getAvionicsDataResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[11])) {
		msgEnum = setAvionicsDataResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[12])) {
		msgEnum = subscribeAvionicsDataResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[13])) {
		msgEnum = subscribeAvionicsDataOnEventResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[14])) {
		msgEnum = unSubscribeResponse;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) HDMessageNameArr[15])) {
		msgEnum = heartbeatResponse;
	} else {
		msgEnum = invalidMsg;
	}

	xmlFreeDoc(doc);
	return msgEnum;
}

void HDMessageHandle::GetRegisterData(ST_APP_REGISTER_DATA* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr nodeSet = GetNodeSet(doc, BAD_CAST"/method/application");
	if (nodeSet == NULL) {
		return;
	} else {
		node = nodeSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->partNum, node, (const char*) "partNumber");
		SetUintProperty(&(_data->processID), node, (const char*) "processID");
		SetStrProperty(_data->passwd, node, (const char*) "password");
		SetStrProperty(_data->platform, node, (const char*) "platform");
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetUnregisterData(ST_APP_UNREGISTER_DATA* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr nodeSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (nodeSet == NULL) {
		return;
	} else {
		node = nodeSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetAvionicsData(ST_DATA_REQUEST* _data, char* buffer,
		int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}
	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
			BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;
			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetSetAvionicsData(ST_DATA_REQUEST* _data, char* buffer,
		int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}
	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
			BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;
			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			SetUintProperty(&(rawData.length), node, (const char*) "length");
			SetStrProperty(rawData.crc, node, (const char*) "CRC");
			SetStrValue(rawData.data, node);
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetSubscribeAvionicsData(ST_DATA_REQUEST* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}

	xmlNodeSetPtr publishPortSet = GetNodeSet(doc,
			BAD_CAST "/method/publishport");
	if (publishPortSet == NULL) {
		return;
	} else {
		node = publishPortSet->nodeTab[0];
		SetUintValue(&(_data->publishPort), node);
	}

	xmlNodeSetPtr refreshPeriodSet = GetNodeSet(doc,
			BAD_CAST "/method/refreshperiod");
	if (publishPortSet == NULL) {
		return;
	} else {
		node = refreshPeriodSet->nodeTab[0];
		SetUintValue(&(_data->refreshPeriod), node);
	}

	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
			BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;
			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetSubscribeAvionicsDataOnEvent(ST_DATA_REQUEST* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}

	xmlNodeSetPtr publishPortSet = GetNodeSet(doc,
			BAD_CAST "/method/publishport");
	if (publishPortSet == NULL) {
		return;
	} else {
		node = publishPortSet->nodeTab[0];
		SetUintValue(&(_data->publishPort), node);
	}

	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
			BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;
			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetUnSubscribe(ST_DATA_REQUEST* _data, char* buffer,
		int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}

	xmlNodeSetPtr publishPortSet = GetNodeSet(doc,
			BAD_CAST "/method/publishport");
	if (publishPortSet == NULL) {
		return;
	} else {
		_data->unSubscribePublishPortNum = publishPortSet->nodeNr;
		for (int i = 0; i < publishPortSet->nodeNr; i++) {
			node = publishPortSet->nodeTab[i];
			SetUintValue(&(_data->unSubscribePublishPort[i]), node);
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetHeartBeat(ST_HEARTBEAT_REQUEST* _data, char* buffer,
		int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}

	xmlNodeSetPtr heartBeatSet = GetNodeSet(doc,
			BAD_CAST "/method/heartbeat");
	if (heartBeatSet == NULL) {
		return;
	} else {
		node = heartBeatSet->nodeTab[0];
		SetUintProperty(&(_data->block), node, (const char*) "block");
	}

	xmlFreeDoc(doc);
}

// Response Message send to app
			void HDMessageHandle::SetRegisterDataPesponse(
					ST_APP_REGISTER_RESPONSE_DATA response, char* buffer) {
				xmlChar* xml;
				int strSize;
				xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
				xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
				xmlDocSetRootElement(doc, root_node);
				xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");
	xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(node, BAD_CAST "name", BAD_CAST response.appName);
	CreateUintProperty(response.validity, node, BAD_CAST "validity");
	xmlNewProp(node, BAD_CAST "signature", BAD_CAST response.signature);
	xmlAddChild(root_node, node);

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetUnregisterDataPesponse(
		ST_APP_UNREGISTER_RESPONSE_DATA response, char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");
	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetGetAvionicsDataPesponse(ST_DATA_RESPONSE response,
		char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");

	xmlNodePtr rawdatasNode = xmlNewNode(NULL, BAD_CAST "rawdatas");
	xmlAddChild(root_node, rawdatasNode);

	for (int i = 0; i < response.rawDataNum; i++) {
		ST_RAWDATA_APPLICATION rawdata = response.rawDataApp[i];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "rawdata");
		xmlNewProp(node, BAD_CAST "name", BAD_CAST rawdata.rawDataName);
		CreateUintProperty(rawdata.validity, node, BAD_CAST "validity");
		CreateUintProperty(rawdata.type, node, BAD_CAST "type");
		xmlNewProp(node, BAD_CAST "interface", BAD_CAST rawdata.interfaceName);
		xmlNewProp(node, BAD_CAST "ICDid", BAD_CAST rawdata.ICDid);
		CreateUintProperty(rawdata.length, node, BAD_CAST "length");
		CreateDoubleProperty(rawdata.time, node, BAD_CAST "time");
		xmlNewProp(node, BAD_CAST "CRC", BAD_CAST rawdata.crc);
		xmlAddChild(node, xmlNewText(BAD_CAST rawdata.data));
		xmlAddChild(rawdatasNode, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetAvionicsDataPesponse(ST_DATA_RESPONSE response,
		char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");
	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetSubscribeAvionicsDataPesponse(
		ST_DATA_RESPONSE response, char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");

	xmlNodePtr rawdatasNode = xmlNewNode(NULL, BAD_CAST "rawdatas");
	xmlAddChild(root_node, rawdatasNode);

	for (int i = 0; i < response.rawDataNum; i++) {
		ST_RAWDATA_APPLICATION rawdata = response.rawDataApp[i];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "rawdata");
		xmlNewProp(node, BAD_CAST "name", BAD_CAST rawdata.rawDataName);
		CreateUintProperty(rawdata.validity, node, BAD_CAST "validity");
		CreateUintProperty(rawdata.type, node, BAD_CAST "type");
		xmlNewProp(node, BAD_CAST "interface", BAD_CAST rawdata.interfaceName);
		xmlNewProp(node, BAD_CAST "ICDid", BAD_CAST rawdata.ICDid);
		CreateUintProperty(rawdata.length, node, BAD_CAST "length");
		CreateDoubleProperty(rawdata.time, node, BAD_CAST "time");
		xmlNewProp(node, BAD_CAST "CRC", BAD_CAST rawdata.crc);
		xmlAddChild(node, xmlNewText(BAD_CAST rawdata.data));
		xmlAddChild(rawdatasNode, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetSubscribeAvionicsDataOnEventPesponse(
		ST_DATA_RESPONSE response, char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");

	xmlNodePtr rawdatasNode = xmlNewNode(NULL, BAD_CAST "rawdatas");
	xmlAddChild(root_node, rawdatasNode);

	for (int i = 0; i < response.rawDataNum; i++) {
		ST_RAWDATA_APPLICATION rawdata = response.rawDataApp[i];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "rawdata");
		xmlNewProp(node, BAD_CAST "name", BAD_CAST rawdata.rawDataName);
		CreateUintProperty(rawdata.validity, node, BAD_CAST "validity");
		CreateUintProperty(rawdata.type, node, BAD_CAST "type");
		xmlNewProp(node, BAD_CAST "interface", BAD_CAST rawdata.interfaceName);
		xmlNewProp(node, BAD_CAST "ICDid", BAD_CAST rawdata.ICDid);
		CreateUintProperty(rawdata.length, node, BAD_CAST "length");
		CreateDoubleProperty(rawdata.time, node, BAD_CAST "time");
		xmlNewProp(node, BAD_CAST "CRC", BAD_CAST rawdata.crc);
		xmlAddChild(node, xmlNewText(BAD_CAST rawdata.data));
		xmlAddChild(rawdatasNode, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetUnSubscribePesponse(ST_DATA_RESPONSE response,
		char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");
	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetHeartBeatPesponse(ST_HEARTBEAT_RESPONSE response,
		char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "response");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");

	xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "heartbeat");
	CreateUintProperty(response.block, node, BAD_CAST "block");
	CreateDoubleProperty(response.time, node, BAD_CAST "time");
	xmlAddChild(root_node, node);

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

// Sync message send to app
void HDMessageHandle::SetPeriodicMessage(ST_DATA_RESPONSE response,
		char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");

	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST response.appName);
	xmlNewProp(appNode, BAD_CAST "signature", BAD_CAST response.signature);
	xmlAddChild(root_node, appNode);

	xmlNodePtr rawdatasNode = xmlNewNode(NULL, BAD_CAST "rawdatas");
	xmlAddChild(root_node, rawdatasNode);

	for (int i = 0; i < response.rawDataNum; i++) {
		ST_RAWDATA_APPLICATION rawdata = response.rawDataApp[i];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "rawdata");
		xmlNewProp(node, BAD_CAST "name", BAD_CAST rawdata.rawDataName);
		CreateUintProperty(rawdata.validity, node, BAD_CAST "validity");
		CreateUintProperty(rawdata.type, node, BAD_CAST "type");
		xmlNewProp(node, BAD_CAST "interface", BAD_CAST rawdata.interfaceName);
		xmlNewProp(node, BAD_CAST "ICDid", BAD_CAST rawdata.ICDid);
		CreateUintProperty(rawdata.length, node, BAD_CAST "length");
		CreateDoubleProperty(rawdata.time, node, BAD_CAST "time");
		xmlNewProp(node, BAD_CAST "CRC", BAD_CAST rawdata.crc);
		xmlAddChild(node, xmlNewText(BAD_CAST rawdata.data));
		xmlAddChild(rawdatasNode, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}
void HDMessageHandle::SetEventMessage(ST_DATA_RESPONSE response, char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.resName);
	CreateUintProperty(response.errorCode, root_node, BAD_CAST "errorcode");

	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST response.appName);
	xmlNewProp(appNode, BAD_CAST "signature", BAD_CAST response.signature);
	xmlAddChild(root_node, appNode);

	xmlNodePtr rawdatasNode = xmlNewNode(NULL, BAD_CAST "rawdatas");
	xmlAddChild(root_node, rawdatasNode);

	for (int i = 0; i < response.rawDataNum; i++) {
		ST_RAWDATA_APPLICATION rawdata = response.rawDataApp[i];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "rawdata");
		xmlNewProp(node, BAD_CAST "name", BAD_CAST rawdata.rawDataName);
		CreateUintProperty(rawdata.validity, node, BAD_CAST "validity");
		CreateUintProperty(rawdata.type, node, BAD_CAST "type");
		xmlNewProp(node, BAD_CAST "interface", BAD_CAST rawdata.interfaceName);
		xmlNewProp(node, BAD_CAST "ICDid", BAD_CAST rawdata.ICDid);
		CreateUintProperty(rawdata.length, node, BAD_CAST "length");
		CreateDoubleProperty(rawdata.time, node, BAD_CAST "time");
		xmlNewProp(node, BAD_CAST "CRC", BAD_CAST rawdata.crc);
		xmlAddChild(node, xmlNewText(BAD_CAST rawdata.data));
		xmlAddChild(rawdatasNode, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

/******************************for APSS*********************************/
void HDMessageHandle::GetPublishAvionicsDataPesponse(ST_DATA_RESPONSE* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->resName, root, (const char*) "name");
	//SetUintProperty(&(_data->errorCode), root, (const char*) "errorcode");
	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
	BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;

			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			SetUintProperty(&(rawData.validity), node,
					(const char*) "validity");
			SetUintProperty(&(rawData.type), node, (const char*) "type");
			SetStrProperty(rawData.interfaceName, node,
					(const char*) "interface");
			SetStrProperty(rawData.ICDid, node, (const char*) "ICDid");
			SetUintProperty(&(rawData.length), node, (const char*) "length");
			SetDoubleProperty(&(rawData.time), node, (const char*) "time");
			SetStrProperty(rawData.crc, node, (const char*) "CRC");
			SetStrValue(rawData.data, node);
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetSubscribeAvionicsDataPesponse(ST_DATA_RESPONSE* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->resName, root, (const char*) "name");
	SetUintProperty(&(_data->errorCode), root, (const char*) "errorcode");
	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
	BAD_CAST "/response/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;

			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			SetUintProperty(&(rawData.validity), node,
					(const char*) "validity");
			SetUintProperty(&(rawData.type), node, (const char*) "type");
			SetStrProperty(rawData.interfaceName, node,
					(const char*) "interface");
			SetStrProperty(rawData.ICDid, node, (const char*) "ICDid");
			SetUintProperty(&(rawData.length), node, (const char*) "length");
			SetDoubleProperty(&(rawData.time), node, (const char*) "time");
			SetStrProperty(rawData.crc, node, (const char*) "CRC");
			SetStrValue(rawData.data, node);
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetUnSubscribePesponse(ST_DATA_RESPONSE* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->resName, root, (const char*) "name");
	SetUintProperty(&(_data->errorCode), root, (const char*) "errorcode");
	xmlFreeDoc(doc);
}

void HDMessageHandle::GetHeartBeatResponse(ST_HEARTBEAT_RESPONSE* _data,
		char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->resName, root, (const char*) "name");
	SetUintProperty(&(_data->errorCode), root, (const char*) "errorcode");
	xmlNodeSetPtr heartBeatSet = GetNodeSet(doc,
	BAD_CAST "/response/heartbeat");
	if (heartBeatSet == NULL) {
		return;
	} else {
		node = heartBeatSet->nodeTab[0];
		SetUintProperty(&(_data->block), node, (const char*) "block");
		SetDoubleProperty(&(_data->time), node, (const char*) "time");
	}
	xmlFreeDoc(doc);
}

void HDMessageHandle::GetPeriodicMessage(ST_DATA_RESPONSE* _data, char* buffer,
		int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->resName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}
	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
	BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;
			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			SetUintProperty(&(rawData.validity), node,
					(const char*) "validity");
			SetUintProperty(&(rawData.type), node, (const char*) "type");
			SetStrProperty(rawData.interfaceName, node,
					(const char*) "interface");
			SetStrProperty(rawData.ICDid, node, (const char*) "ICDid");
			SetUintProperty(&(rawData.length), node, (const char*) "length");
			SetDoubleProperty(&(rawData.time), node, (const char*) "time");
			SetStrProperty(rawData.crc, node, (const char*) "CRC");
			SetStrValue(rawData.data, node);
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::GetEventMessage(ST_DATA_RESPONSE* _data, char* buffer,
		int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->resName, root, (const char*) "name");
	xmlNodeSetPtr appSet = GetNodeSet(doc, BAD_CAST "/method/application");
	if (appSet == NULL) {
		return;
	} else {
		node = appSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}
	xmlNodeSetPtr rawdataSet = GetNodeSet(doc,
	BAD_CAST "/method/rawdatas/rawdata");
	if (rawdataSet != NULL) {
		_data->rawDataNum = rawdataSet->nodeNr;
		for (int i = 0; i < rawdataSet->nodeNr; i++) {
			node = rawdataSet->nodeTab[i];
			ST_RAWDATA_APPLICATION rawData;
			SetStrProperty(rawData.rawDataName, node, (const char*) "name");
			SetUintProperty(&(rawData.validity), node,
					(const char*) "validity");
			SetUintProperty(&(rawData.type), node, (const char*) "type");
			SetStrProperty(rawData.interfaceName, node,
					(const char*) "interface");
			SetStrProperty(rawData.ICDid, node, (const char*) "ICDid");
			SetUintProperty(&(rawData.length), node, (const char*) "length");
			SetDoubleProperty(&(rawData.time), node, (const char*) "time");
			SetStrProperty(rawData.crc, node, (const char*) "CRC");
			SetStrValue(rawData.data, node);
			_data->rawDataApp[i] = rawData;
		}
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::SetSubscribeAvionicsData(ST_DATA_REQUEST data,
		char* buffer) {
	xmlChar* xml;
	char buf[20];
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST data.methodName);
	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST data.appName);
	xmlNewProp(appNode, BAD_CAST "signature", BAD_CAST data.signature);
	xmlAddChild(root_node, appNode);

	xmlNodePtr portNode = xmlNewNode(NULL, BAD_CAST "publishport");
	sprintf(buf, "%d", data.publishPort);
	xmlAddChild(portNode, xmlNewText(BAD_CAST buf));
	xmlAddChild(root_node, portNode);

	xmlNodePtr periodNode = xmlNewNode(NULL, BAD_CAST "refreshperiod");
	sprintf(buf, "%d", data.refreshPeriod);
	xmlAddChild(periodNode, xmlNewText(BAD_CAST buf));
	xmlAddChild(root_node, periodNode);

	xmlNodePtr rawdatasNode = xmlNewNode(NULL, BAD_CAST "rawdatas");
	xmlAddChild(root_node, rawdatasNode);

	for (int i = 0; i < data.rawDataNum; i++) {
		ST_RAWDATA_APPLICATION rawdata = data.rawDataApp[i];
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "rawdata");
		xmlNewProp(node, BAD_CAST "name", BAD_CAST rawdata.rawDataName);
		//CreateUintProperty(rawdata.length, node, BAD_CAST "length");
		//xmlNewProp(node, BAD_CAST "CRC", BAD_CAST rawdata.crc);
		xmlAddChild(rawdatasNode, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetUnSubscribe(ST_DATA_REQUEST data, char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST data.methodName);
	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST data.appName);
	xmlNewProp(appNode, BAD_CAST "signature", BAD_CAST data.signature);
	xmlAddChild(root_node, appNode);
	for (unsigned int i = 0; i < data.unSubscribePublishPortNum; i++) {
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "publishport");
		char str[32];
		sprintf(str, "%u", data.unSubscribePublishPort[i]);
		xmlAddChild(node, xmlNewText(BAD_CAST str));
		xmlAddChild(root_node, node);
	}

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetHeartBeat(ST_HEARTBEAT_REQUEST data, char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST data.methodName);
	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST data.appName);
	xmlNewProp(appNode, BAD_CAST "signature", BAD_CAST data.signature);
	xmlAddChild(root_node, appNode);

	xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "heartbeat");
	CreateUintProperty(data.block, node, BAD_CAST "block");
	xmlAddChild(root_node, node);

	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::SetRegisterData(ST_APP_REGISTER_DATA response,
		char* buffer) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.methodName);

	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST response.appName);
	xmlNewProp(appNode, BAD_CAST "parNumber", BAD_CAST response.partNum);
	CreateUintProperty(getpid(), appNode, BAD_CAST "processID");
	xmlNewProp(appNode, BAD_CAST "password", BAD_CAST response.passwd);
	xmlNewProp(appNode, BAD_CAST "platform", BAD_CAST response.platform);

	xmlAddChild(root_node, appNode);
	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::GetRegisterResponseData(
		ST_APP_REGISTER_RESPONSE_DATA* _data, char* buffer, int bufferSize) {
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	memset(_data, 0, sizeof(ST_APP_REGISTER_RESPONSE_DATA));

	SetStrProperty(_data->resName, root, (const char*) "name");
	SetUintProperty(&_data->errorCode, root, (const char*) "errorcode");
	xmlNodeSetPtr nodeSet = GetNodeSet(doc, BAD_CAST "/response/application");
	if (nodeSet == NULL) {
		return;
	} else {
		node = nodeSet->nodeTab[0];
		SetStrProperty(_data->appName, node, (const char*) "name");
		SetUintProperty(&_data->validity, node, (const char*) "validity");
		SetStrProperty(_data->signature, node, (const char*) "signature");
	}

	xmlFreeDoc(doc);
}

void HDMessageHandle::SetUnregisterData(ST_APP_UNREGISTER_DATA response, char* buffer){
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "method");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.methodName);
	xmlNodePtr appNode = xmlNewNode(NULL, BAD_CAST "application");
	xmlNewProp(appNode, BAD_CAST "name", BAD_CAST response.appName);
	xmlNewProp(appNode, BAD_CAST "signature", BAD_CAST response.signature);
	xmlAddChild(root_node, appNode);
	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	strcpy(buffer, (char*) xml);
}

void HDMessageHandle::GetUnRegisterDataPesponse(
			ST_APP_UNREGISTER_RESPONSE_DATA* _data, char* buffer,int bufferSize){
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	memset(_data, 0, sizeof(ST_APP_UNREGISTER_RESPONSE_DATA));
	SetStrProperty(_data->resName, root, (const char*) "name");
	SetUintProperty(&_data->errorCode, root, (const char*) "errorcode");
	xmlFreeDoc(doc);
}



/***************** for aps client ************************************/
// Received Message from app
APS_MESSAGE_ENUM HDMessageHandle::GetApsMessageName(char* buffer,
		int bufferSize) {
	static char *APSRequstSty[4] = { "getAvionicParameters",
			"subscribeAvionicParameters", "subscribeAvionicParametersOnEvent",
			"unSubscribe" };
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	if (doc == NULL) {
		return apsInvalidXML;
	}
	xmlNodePtr root = xmlDocGetRootElement(doc);
	xmlChar* nameProp = xmlGetProp(root, (const xmlChar*) "name");
	APS_MESSAGE_ENUM msgEnum;
	if (!xmlStrcmp(nameProp, (xmlChar*) APSRequstSty[0])) {
		msgEnum = getAvionicParameters;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) APSRequstSty[1])) {
		msgEnum = subscribeAvionicParameters;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) APSRequstSty[2])) {
		msgEnum = subscribeAvionicParametersOnEvent;
	} else if (!xmlStrcmp(nameProp, (xmlChar*) APSRequstSty[3])) {
		msgEnum = apsunSubscribe;
	} else {
		msgEnum = apsInvalidMsg;
	}

	xmlFreeDoc(doc);
	return msgEnum;
}

void HDMessageHandle::GetApsRequestMsg(APS_DATA_REQUEST* _data, char* buffer,
		int bufferSize) {
	APS_MESSAGE_ENUM msgEnum;
	msgEnum = GetApsMessageName(buffer, bufferSize);
	_data->eventType = msgEnum;
	xmlNodePtr node = NULL;
	xmlDocPtr doc = xmlParseMemory(buffer, bufferSize);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	SetStrProperty(_data->methodname, root, (const char*) "name");
	if (msgEnum == subscribeAvionicParameters
			|| msgEnum == subscribeAvionicParametersOnEvent
			|| msgEnum == apsunSubscribe) {
		xmlNodeSetPtr publishPortSet = GetNodeSet(doc,
		BAD_CAST "/method/publishport");
		if (publishPortSet == NULL) {
			return;
		} else {
			_data->publishPortNum = publishPortSet->nodeNr;
			if (_data->publishPortNum > 5) {
				_data->publishPortNum = 5;
				printf("[APS] Warning: exceed 5 ports\n");
			}
			for (int i = 0; i < publishPortSet->nodeNr; i++) {
				node = publishPortSet->nodeTab[i];
				SetUintValue(&(_data->publishPort[i]), node);
			}
		}
		xmlFree(publishPortSet);
	}

	if (msgEnum == getAvionicParameters || msgEnum == subscribeAvionicParameters
			|| msgEnum == subscribeAvionicParametersOnEvent) {

		xmlNodeSetPtr paramSet = GetNodeSet(doc,
		BAD_CAST "/method/parameters/parameter");
		if (paramSet == NULL) {
			return;
		} else {
			_data->signalnum = paramSet->nodeNr;
			if (_data->signalnum > 10) {
				_data->signalnum = 10;
				printf("[APS] Warning: exceed 10 parameters\n");
			}
			for (int i = 0; i < _data->signalnum; i++) {
				node = paramSet->nodeTab[i];
				SetStrProperty(_data->signal[i].signame, node,
						(const char*) "name");
			}
		}
		xmlFree(paramSet);
	}

	if (msgEnum == subscribeAvionicParameters) {
		xmlNodeSetPtr refreshPeriodSet = GetNodeSet(doc,
		BAD_CAST "/method/refreshperiod");
		if (refreshPeriodSet == NULL) {
			return;
		} else {
			node = refreshPeriodSet->nodeTab[0];
			SetUintValue(&(_data->refreshperiod), node);
		}
		xmlFree(refreshPeriodSet);
	}
	xmlFreeDoc(doc);
}

void HDMessageHandle::SetApsResponseMsg(APS_DATA_RESPONSE response,
		char* buffer, bool isMethod) {
	xmlChar* xml;
	int strSize;
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root_node;
	if(isMethod){
		root_node = xmlNewNode(NULL, BAD_CAST "method");
	}else{
		root_node = xmlNewNode(NULL, BAD_CAST "response");
	}
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "name", BAD_CAST response.methogname);
	CreateUintProperty(response.errcode, root_node, BAD_CAST "errorcode");
	//printf("xml:%d\n", response.errcode);
	if (response.errcode == 0 || response.errcode == 8) {
		xmlNodePtr paramsNode = xmlNewNode(NULL, BAD_CAST "parameters");

		for (int i = 0; i < response.signalnum; i++) {
			xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "parameter");
			xmlNewProp(node, BAD_CAST "name",
			BAD_CAST response.signal[i].signame);
			CreateUintProperty(response.signal[i].status, node,
			BAD_CAST "status");
			CreateUintProperty(response.signal[i].type, node, BAD_CAST "type");
			switch (response.signal[i].type) {
			case TYPE_F64B:
				CreateDoubleProperty(response.ParamVale[i].value1, node,
				BAD_CAST "value");
				break;
			case TYPE_S64B:
				CreateLongLongProperty(response.ParamVale[i].value2, node,
				BAD_CAST "value");
				break;
			case TYPE_F32B:
				CreateFloatProperty(response.ParamVale[i].value3, node,	BAD_CAST "value");
				break;
			case TYPE_U32B:
				// discard
				break;
			case TYPE_S32B:
			case TYPE_OPAQUE:
			case TYPE_ENUM:
				CreateUintProperty(response.ParamVale[i].value5, node, BAD_CAST "value");
				//printf("welsss---->%d\n", response.ParamVale[i].value5);

				break;
			case TYPE_BOOL:
				CreateUintProperty(response.ParamVale[i].value6, node, BAD_CAST "value");
				break;
			case TYPE_STRING:
			case TYPE_DATE:
			case TYPE_TIME:
				xmlNewProp(node, BAD_CAST "value",
						BAD_CAST response.ParamVale[i].value);
				break;
			default:
				break;
			}
			CreateDoubleProperty(response.signal[i].time, node,
			BAD_CAST "time");
			xmlAddChild(paramsNode, node);
			xmlAddChild(root_node, paramsNode);
		}
	}
	string format = "utf-8";
	xmlDocDumpMemoryEnc(doc, &xml, &strSize, format.c_str());
	xmlFreeDoc(doc);
	//printf("xml:%s\n", xml);
	strcpy(buffer, (char*) xml);

}



} /* namespace ThreadTest */
