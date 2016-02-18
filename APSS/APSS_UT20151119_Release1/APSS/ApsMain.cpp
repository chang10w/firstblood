#include <iostream>
#include <boost/thread/thread.hpp>
#include <time.h>
#include <sys/select.h>
#include <boost/lexical_cast.hpp>
#include <libxml/xmlstring.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "ApsXmlOp.h"
#include "ApsHDSvr.h"
#include "ApsParaSvr.h"
#include "APSS.h"
//#include "aps_const.h"

using namespace std;

#define MAX_PARAM 100

APS_CONFIG *g_apsconfig;
CParamData *g_hparameter;
int paramnum;

void initParam() {
	char filename[] = "/tmp/aps_config.xml";

	g_apsconfig = (APS_CONFIG *) malloc(sizeof(APS_CONFIG));
	memset(g_apsconfig, 0, sizeof(APS_CONFIG));
	g_hparameter = (CParamData*) malloc(sizeof(CParamData) * MAX_PARAM);
	memset(g_hparameter, 0, sizeof(CParamData) * MAX_PARAM);

	ApsXmlOp::getDataFromXMLFile(filename, &g_hparameter, g_apsconfig);
	for (int i = 0; i < MAX_PARAM; i++) {
		if (g_hparameter[i].paramnum == 0) {
			break;
		}
		//cout << g_hparameter[i].signel[0].ICDid << endl;
		paramnum = i;
	}
	paramnum++;
	strcpy(g_apsconfig->APS_Name, "APS");
	g_apsconfig->ProcessId = getpid();

	ApsHDSvr::g_apsconfig = g_apsconfig;
	ApsHDSvr::g_hparameter = g_hparameter;
	ApsHDSvr::paramnum = paramnum;
	ApsParaSvr::g_hparameter = g_hparameter;
	ApsParaSvr::g_apsconfig = g_apsconfig;
	ApsParaSvr::paramnum = paramnum;

}

int main(int argc, char** argv) {
	/*	ApsHDSvr* hdSvr = new ApsHDSvr();
	 ApsParaSvr* apsSvr = new ApsParaSvr();*/
	initParam();
	/*	pthread_t  apsProcHandle, apsMsgHandle, apsPubHandle, apsEventHandle, heartHandle;
	 pthread_t hdHandle;*/
	/*	int rtn;
	 struct timeval tv;
	 tv.tv_sec= 3;
	 tv.tv_usec = 0;
	 rtn = select(0, NULL, NULL, NULL, &tv);
	 if(rtn<0){
	 printf("[APS] stimer error\n");
	 }
	 printf("[APS] %d\n", rtn);*/

	//printf("gp: %x\n", &g_hparameter);
	/*//ApsParaSvr::apsSvr
	 if(! ApsParaSvr::initServerSocket(atoi(g_apsconfig->APSvr_Port_PARAM), &ApsParaSvr::apsSvr))
	 {
	 printf("[APS] can not start server!");
	 return 1;
	 }
	 ApsParaSvr::isRun=true;
	 printf("[APS] startup now.\n");
	 //ApsParaSvr::apsProcess(&ApsParaSvr::apsSvr);

	 pthread_create(&apsProcHandle, NULL, ApsParaSvr::apsProcess, &ApsParaSvr::apsSvr);

	 pthread_create(&apsMsgHandle, NULL, ApsParaSvr::apsProcessMsg,( void*) 0);

	 pthread_create(&apsPubHandle, NULL, ApsParaSvr::publishTimer,( void*) 0);

	 pthread_create(&apsEventHandle, NULL, ApsParaSvr::eventTimer,( void*) 0);*/
	//sleep(1);
	//ApsParaSvr::aps_start(true);

	ApsHDSvr::apsHd_start(true);
	/*
	 cout << "start...[APS-HD]" << getpid()<< endl;
	 hdSvr->initServerSocket(atoi(g_apsconfig->APSvr_Port_AIDCP), &hdSvr->sockHDpub);
	 hdSvr->initClientSocket(g_apsconfig->AIDCP_Addr, atoi(g_apsconfig->AIDCP_Port), &hdSvr->sockHDsvr);
	 cout << "start...init sockets.." << getpid()<< endl;



	 ApsHDSvr::registHDSvr(hdSvr->sockHDsvr);
	 cout << "start...registHDSvr" << endl;

	 ApsHDSvr::subscribeData(hdSvr->sockHDsvr, &g_hparameter);
	 cout << "start...subscribeData" << endl;

	 pthread_create(&hdHandle, NULL, ApsHDSvr::pubListen, (void*) &hdSvr->sockHDpub);
	 cout << "start...pubListen" << endl;

	 pthread_create(&heartHandle, NULL, ApsHDSvr::heartBeat, (void*) &hdSvr->sockHDsvr);
	 cout << "start...heartbeat" << endl;
	 */

	//hdSvr->pubListen((void*) &hdSvr->sockHDpub);
	int i = 0;

	while (true) {
		sleep(5);
		/*if(1){
		 //ApsHDSvr::isRegist = false;
		 g_hparameter->ParamVale.value1 = i;
		 g_hparameter->ParamVale.value2 = i;
		 g_hparameter->ParamVale.value3 = i;
		 g_hparameter->ParamVale.value4 = i;
		 g_hparameter->ParamVale.value5 = i;
		 g_hparameter->ParamVale.value6 = i%2;
		 }*/
		if (i == 3) {
			int ports[5] = { 64001, 0, 0, 0, 0 };
			printf("====>I need a break now.\n");
			/*if (ApsHDSvr::sockHDsvr > 0)
				ApsHDSvr::unSubscribe(ApsHDSvr::sockHDsvr, ports);*/
		};
		printf("** %d ***\n", i);
		i++;
		/*if (i == 16)
			ApsParaSvr::aps_stop();*/
		/*if (i == 5)
			ApsHDSvr::apsHd_stop();*/
	}

	//hdSvr->stopServer();
	cout << paramnum << " end\n" << sizeof(CParamData);

	return 0;
}

