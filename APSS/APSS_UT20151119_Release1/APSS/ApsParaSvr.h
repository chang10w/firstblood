/*
 * ApsParaSvr.h
 *
 *  Created on: Oct 30, 2015
 *      Author: root
 */

#ifndef APSPARASVR_H_
#define APSPARASVR_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <list.h>
#include <cstdlib>
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "HDMessageHandle.h"

#define MAX_REMOTE_NUMBER 100



typedef struct
{
    struct sockaddr_in addr;
    char msg[2048];
    int size;
    int offset;
    int sockfd;
} EthernetPair;

typedef struct {
	EthernetPair cli;
	APS_DATA_REQUEST req;
} RequestWithSock;

/*for both cycle and on event msgs*/
typedef struct
{
	int sockfd;
	struct sockaddr_in addr;
	uint16_t port;
	int period;
	int oldPeriod;
	APS_DATA_REQUEST airdatarequst;
	APS_DATA_APPLICATION apsparameter[10];
	CAPSParaVal ParamVale[10];
}CAirPubMsg;

typedef list<CAirPubMsg> ApsPubMsg;
typedef list<RequestWithSock> ClientMsg;

class ApsParaSvr {

public:
	ApsParaSvr();
	virtual ~ApsParaSvr();

	static void* flightModeSvr(void* args);
	static void* apsListen(void* args);
	static bool acceptClient(int sockfd);
	static void* apsProcess(void* args);
	static void* apsProcessMsg(void* args);
	static bool publishMgr(RequestWithSock* request);
	static bool eventMgr(RequestWithSock* request, APS_DATA_RESPONSE *response);
	static void * publishTimer(void* arg);
	static void * eventTimer(void* arg);

	static bool validateRequest(APS_DATA_REQUEST *request, APS_DATA_RESPONSE * response);
	static bool unSubscribeMgr(RequestWithSock* request, bool needDel);
	static bool updateSigValueFromAPSList(APS_DATA_REQUEST *request,
			APS_DATA_RESPONSE* response);

	static	bool isSignalChange(CAirPubMsg *tmp, APS_DATA_RESPONSE* response);
	static bool initServerSocket(int localPort, int* sockfd);
	static bool initClientSocket(char* ip, int port, int *sockfd);
	static bool initClientUdpSocket(char* ip, int port, int *sockfd);

	static bool aps_start(bool isDebug);
	static bool aps_stop();

	static CParamData *g_hparameter;
	static int paramnum;
	static APS_CONFIG* g_apsconfig;
	static list<int> flightModes;

	static int apsSvr;
	static bool isRun;
	static bool debug;

	static boost::mutex muEvent, muPub, muMsg;
	// private
	static pthread_t  apsProcHandle, apsMsgHandle, apsPubHandle, apsEventHandle;
	static fd_set allSet;
	static EthernetPair cliPair[MAX_REMOTE_NUMBER];
	static int maxFd;
	static int maxCliIndex;

	static ClientMsg cliMsgs;
	static ApsPubMsg pubMsgs;
	static ApsPubMsg eventMsgs;
};

#endif /* APSPARASVR_H_ */
