/*
 * ApsXmlOp.h
 *
 *  Created on: Oct 28, 2015
 *      Author: root
 */

#ifndef APSXMLOP_H_
#define APSXMLOP_H_

#include <iostream>
#include <libxml/xmlstring.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <list>

#include "HDMessageHandle.h"

class ApsXmlOp {

private:
	static bool getApsConfig(xmlDocPtr doc, APS_CONFIG *apsConfig);
	static bool getParaConfig(xmlDocPtr doc, CParamData **pItem);
	static bool getParaFieldProp(xmlDocPtr doc, CSignalProperty *signal,
			CFeildProerty *field, int* sigFlag);
	static int sigType2Int(char* type);
	static xmlNodeSetPtr getNodeSet(xmlDocPtr doc, char *xpath);
	static bool getNodeValue(xmlDocPtr doc, char *xpath, char *value);
	static bool getFlightModes(xmlDocPtr doc, std::list<int> *flight);

public:
	ApsXmlOp();
	virtual ~ApsXmlOp();
	static bool getDataFromXMLFile(char* url) ;
    static int paramnum;
    static APS_CONFIG *g_apsconfig;
    static CParamData *g_hparameter;
    static std::list<int> flightModes;
};

#endif /* APSXMLOP_H_ */
