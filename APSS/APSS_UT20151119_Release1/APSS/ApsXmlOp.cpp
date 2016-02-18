/*
 * ApsXmlOp.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: root
 */

#include "ApsXmlOp.h"
using namespace std;
using namespace HDServer;

char *signalType[11] = { "TYPE_F64B", "TYPE_S64B", "TYPE_F32B", "TYPE_U32B",
		"TYPE_S32B", "TYPE_BOOL", "TYPE_STRING", "TYPE_DATE", "TYPE_TIME",
		"TYPE_OPAQUE", "TYPE_ENUM" };

ApsXmlOp::ApsXmlOp() {
	// TODO Auto-generated constructor stub

}

ApsXmlOp::~ApsXmlOp() {
	// TODO Auto-generated destructor stub
}

/* return the content of specified node */
bool ApsXmlOp::getNodeValue(xmlDocPtr doc, char *xpath, char *value) {
	xmlXPathContextPtr ctxt;
	xmlXPathObjectPtr xptr;
	xmlNodeSetPtr nset;
	xmlChar *result;
	ctxt = xmlXPathNewContext(doc);
	xptr = xmlXPathEval(BAD_CAST xpath, ctxt);
	nset = xptr->nodesetval;
	if (xmlXPathNodeSetIsEmpty(nset)) {
		return false;
	}
	result = xmlNodeGetContent(nset->nodeTab[0]);
	if (result == NULL) {
		return false;
	}
	memcpy(value, result, strlen((char*) result));
	xmlFree(result);
	return true;
}

	/* get the nodes from specified path.
	 * the path example: "/APS/Parameters/Parameter[name='xx']" */
xmlNodeSetPtr ApsXmlOp::getNodeSet(xmlDocPtr doc, char *xpath) {
	xmlXPathContextPtr ctxt;
	xmlXPathObjectPtr xptr;
	xmlNodeSetPtr nset;
	ctxt = xmlXPathNewContext(doc);
	//xmlXPathEvalExpression
	xptr = xmlXPathEval(BAD_CAST xpath, ctxt);
	nset = xptr->nodesetval;
	if (xmlXPathNodeSetIsEmpty(nset)) {
		return NULL;
	}
	return nset;
}

	// int change to APS_DATA_TYPE_ENUM in caller
int ApsXmlOp::sigType2Int(char* type) {
	for (int n = 0; n < 11; n++) {
		if (!strcmp(type, signalType[n])) {
			return n;
		}
	}
	return 11;
}

bool ApsXmlOp::getParaFieldProp(xmlDocPtr doc, CSignalProperty *signal,
		CFeildProerty *field, int* sigFlag) {
	xmlNodeSetPtr nset;
	xmlNodePtr curNode;
	xmlChar *data;
	char xpath[255] = { 0 };
	/* search the match ICD section */
	sprintf(xpath, "/APS/ARINC429S/ARINC429[ICD='%s']", signal->ICDid);
	*sigFlag = 1;
	nset = getNodeSet(doc, xpath);
	if (nset == NULL) {
		sprintf(xpath, "/APS/AFDXS/AFDX[ICD='%s']", signal->ICDid);
		nset = getNodeSet(doc, xpath);
		*sigFlag = 2;
		if (nset == NULL) {
			cout << "No ICD found " << xpath << endl;
			*sigFlag = 0;
			return false;
		}
	}

	curNode = xmlFirstElementChild(nset->nodeTab[0]);
	while (curNode != NULL) {
		if (!xmlStrcmp(curNode->name, BAD_CAST"InterfaceName")) {
			data = xmlNodeGetContent(curNode);
			if (data != NULL) {
				memcpy(signal->interfaceName, data, strlen((char*) data));
				xmlFree(data);
			}
		} else if (!xmlStrcmp(curNode->name, BAD_CAST "Direction")) {
			data = xmlNodeGetContent(curNode);
			if (data != NULL) {
				memcpy(signal->direction, data, strlen((char*) data));
				xmlFree(data);
			}
		} else if (!xmlStrcmp(curNode->name, BAD_CAST "FIELD")) {
			data = xmlGetProp(curNode, BAD_CAST "FieldID");
			if (!xmlStrcmp(data, BAD_CAST field->feildID)) {
				xmlFree(data);
				data = xmlGetProp(curNode, BAD_CAST "Name");
				if (data != NULL) {
					memcpy(field->feildName, data, strlen((char*) data));
					xmlFree(data);
				}
				data = xmlGetProp(curNode, BAD_CAST "Type");
				if (data != NULL) {
					memcpy(field->feildType, data, strlen((char*) data));
					xmlFree(data);
				}
				if (*sigFlag == 1) {
					data = xmlGetProp(curNode, BAD_CAST "ScalePattern");
					if (data != NULL) {
						field->scalarPattern = atof((char*) data);
						xmlFree(data);
					}
					data = xmlGetProp(curNode, BAD_CAST "Length");
					if (data != NULL) {
						field->feildSize = atoi((char*) data); // why not use length? diff with afdx
				xmlFree(data);
			}
			data = xmlGetProp(curNode, BAD_CAST "StartBit");
			if (data != NULL) {
				field->startBit = atoi((char*) data);
				xmlFree(data);
			}
		} else {
			data = xmlGetProp(curNode, BAD_CAST "Length");
			if (data != NULL) {
				field->length = atoi((char*) data);
				xmlFree(data);
			}
			data = xmlGetProp(curNode, BAD_CAST "Offset");
			if (data != NULL) {
				field->offset = atoi((char*) data);
				xmlFree(data);
			}
		}
		break; // only one field supported
			}
		}
		curNode = xmlNextElementSibling(curNode);

	}

	return true;
}

		/* get all parameters from xml doc
		 * so far, only A429 and AFDX supported */
bool ApsXmlOp::getParaConfig(xmlDocPtr doc, CParamData **pItem) {
	CParamData *paraItem;
	xmlNodeSetPtr nset;
	xmlNodePtr curNode;
	xmlChar *data;
	int sigNum, fieldNum;

	nset = getNodeSet(doc, "/APS/Parameters/Parameter");
	if (nset == NULL)
		return false;
	//paraItem = (CParamData*) malloc(sizeof(CParamData) * nset->nodeNr);
	//memset(paraItem, 0, sizeof(CParamData) * nset->nodeNr);
	//pItem = &paraItem;
	paraItem = *pItem;
	//paramNum = nset->nodeNr;
	//paramNum = 1;
	for (int i = 0; i < nset->nodeNr; i++) {
		curNode = xmlFirstElementChild(nset->nodeTab[i]);
		while (curNode != NULL) {
			if (!xmlStrcmp(curNode->name, BAD_CAST"Name")) {
				data = xmlNodeGetContent(curNode);
				if (data != NULL) {
					memcpy(paraItem[i].Name, data, strlen((char*) data));
					xmlFree(data);
				}
			} else if (!xmlStrcmp(curNode->name, BAD_CAST "ID")) {
				data = xmlNodeGetContent(curNode);
				if (data != NULL) {
					paraItem[i].Id = atoi((char*)data);
					xmlFree(data);
				}
			}else if (!xmlStrcmp(curNode->name, BAD_CAST "Type")) {
				data = xmlNodeGetContent(curNode);
				if (data != NULL) {
					paraItem[i].Type = (APS_DATA_TYPE_ENUM) sigType2Int((char*)data);
					xmlFree(data);
				}
			} else if (!xmlStrcmp(curNode->name, BAD_CAST "Block")) {
				sigNum = paraItem[i].paramnum;
				fieldNum = paraItem[i].signel[paraItem[i].paramnum].feildnum;
				data = xmlGetProp(curNode, BAD_CAST "SigName");
				/*if (!strncmp((char*) data, "A429", 4)) {
					paraItem[i].sigflag = 1; // Only afdx and a429 are taken into account.
				} else {
					paraItem[i].sigflag = 2;
				}*/
				// Assume all parameters have only one block signal, otherwise re-do it.
				memcpy(paraItem[i].signel[sigNum].signalName, data,
						strlen((char*) data));
				xmlFree(data);
				data = xmlGetProp(curNode, BAD_CAST "ICD");
				memcpy(paraItem[i].signel[sigNum].ICDid, data,
						strlen((char*) data));
				xmlFree(data);
				data = xmlGetProp(curNode, BAD_CAST "FieldID");
				//memcpy(paraItem[i].signel[0].field[0].feildID, data, strlen((char*)data));
				memcpy(paraItem[i].signel[sigNum].field[fieldNum].feildID, data,
						strlen((char*) data));
				xmlFree(data);
				//Update field Properties
				getParaFieldProp(doc, &paraItem[i].signel[sigNum],
						&paraItem[i].signel[sigNum].field[fieldNum],
						&paraItem[i].sigflag);
				paraItem[i].signel[paraItem[i].paramnum].feildnum++;
				paraItem[i].paramnum++;
			}
			curNode = xmlNextElementSibling(curNode);
		}
	}

	return true;
}

bool ApsXmlOp::getApsConfig(xmlDocPtr doc, APS_CONFIG *apsConfig) {
	bool isSuccess = true;
	//apsConfig = (APS_CONFIG *) malloc(sizeof(APS_CONFIG));
	//memset(apsConfig, 0, sizeof(APS_CONFIG));
	getNodeValue(doc, "/APS/Aircraft_Type", apsConfig->Aircraft_Type);
	getNodeValue(doc, "/APS/Version", apsConfig->Version);
	getNodeValue(doc, "/APS/APS_ID", apsConfig->APS_ID);
	getNodeValue(doc, "/APS/PartNumber", apsConfig->PartNumber);
	getNodeValue(doc, "/APS/FileTime", apsConfig->FileTime);
	getNodeValue(doc, "/APS/Password", apsConfig->Password);
	getNodeValue(doc, "/APS/Platform", apsConfig->Platform);
	getNodeValue(doc, "/APS/APS_Addr", apsConfig->APS_Addr);
	getNodeValue(doc, "/APS/APSvr_Port_PARAM", apsConfig->APSvr_Port_PARAM);
	getNodeValue(doc, "/APS/APSvr_Port_AIDCP", apsConfig->APSvr_Port_AIDCP);
	getNodeValue(doc, "/APS/AIDCP_Addr", apsConfig->AIDCP_Addr);
	getNodeValue(doc, "/APS/AIDCP_Port", apsConfig->AIDCP_Port);
	getNodeValue(doc, "/APS/BIT_Addr", apsConfig->BIT_Addr);
	getNodeValue(doc, "/APS/BIT_Port", apsConfig->BIT_Port);
	getNodeValue(doc, "/APS/Multicast_Addr", apsConfig->Multicast_Addr);
	getNodeValue(doc, "/APS/Multicast_Port", apsConfig->Multicast_Port);
	return isSuccess;
}

bool ApsXmlOp::getFlightModes(xmlDocPtr doc, std::list<int> *flight) {
	xmlNodeSetPtr nset;
	int i;
	nset = getNodeSet(doc, "/APS/FlightModes/FlightMode");
	if (nset == NULL) {
		cout << "[APS] no FlightMode defined\n";
		return false;
	}
	for(i=0; i<nset->nodeNr; i++){
		char *tmp = (char*) xmlGetProp(nset->nodeTab[i], (const xmlChar*) "ID");
		flight->push_back(atoi(tmp));
	}

	return true;
}

/* Load config values from xml,
 * output: g_apsconfig and g_hparameter */
bool ApsXmlOp::getDataFromXMLFile(char* url) {
	xmlDocPtr doc;
	xmlNodeSetPtr nset;
	int i;

	doc = xmlReadFile(url, "utf-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		cout << "[APS] parse xml error\n";
		return false;
	}
	nset = getNodeSet(doc, "/APS/Parameters/Parameter");
	if (nset == NULL) {
		cout << "[APS] no parameter defined\n";
		return false;
	}
	paramnum = nset->nodeNr;

	g_apsconfig = (APS_CONFIG *) malloc(sizeof(APS_CONFIG));
	memset(g_apsconfig, 0, sizeof(APS_CONFIG));
	g_hparameter = (CParamData*) malloc(sizeof(CParamData) * paramnum);
	memset(g_hparameter, 0, sizeof(CParamData) * paramnum);

	if (!getApsConfig(doc, g_apsconfig)) {
		xmlFreeDoc(doc);
		return false;
	}
	if (!getParaConfig(doc, &g_hparameter)) {
		xmlFreeDoc(doc);
		return false;
	}
	if (!getFlightModes(doc, &flightModes)){
		xmlFreeDoc(doc);
		return false;
	}

	xmlFreeDoc(doc);

	return true;
}



int ApsXmlOp::paramnum;
APS_CONFIG* ApsXmlOp::g_apsconfig;
CParamData* ApsXmlOp::g_hparameter;
list<int> ApsXmlOp::flightModes;

