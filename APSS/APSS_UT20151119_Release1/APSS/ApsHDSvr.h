/*
 * ApsHDSvr.h
 *
 *  Created on: Oct 30, 2015
 *      Author: root
 */

#ifndef APSHDSVR_H_
#define APSHDSVR_H_
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <boost/thread/thread.hpp>

#include "HDMessageHandle.h"

#define MAX_REMOTE_NUMBER 100

class ApsHDSvr {
private:

public:
	ApsHDSvr();
	virtual ~ApsHDSvr();
	static bool initClientSocket(char* addr, int port, int *sockfd);
	static bool initServerSocket(int localPort, int* sockfd);

	static bool registHDSvr(int sockfd);
	static bool subscribeData(int sockfd, CParamData **pItem);
	static bool unSubscribe(int sockfd, int ports[]);
	static bool unRegister(int sockfd);
	static bool subscribeDataSend(int sockfd, ST_DATA_REQUEST *request,
			ST_DATA_RESPONSE *response);
	static bool updateParams(ST_DATA_RESPONSE *response);
	static double get429Value(unsigned char *data, unsigned int startbit,
			unsigned int len, float scale);
	static void hex2str(char* src, char* des, int len);
	static void* heartBeat(void* args);

	static void* pubListen(void* args);
	static void* pubParaParse(void* args);

	static bool apsHd_start(bool isDebug);
	static bool apsHd_stop(bool isTimeout);

	static int sockHDsvr;
	static int sockHDpub;
	//static int sockAPSsvr;

	static bool isRegist;
	// reference to global values.
	static APS_CONFIG *g_apsconfig;
	static CParamData *g_hparameter;
	static int paramnum;

	static bool debug;

	static pthread_t hdHandle;
	static pthread_t heartBeatHandle;


};

#endif /* APSHDSVR_H_ */
