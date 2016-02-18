/*
 * ApsParaSvr.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: root
 */

#include "ApsParaSvr.h"

using namespace HDServer;

ApsParaSvr::ApsParaSvr() {
	// TODO Auto-generated constructor stub
	// FD_ZERO(&cliSet);
}

ApsParaSvr::~ApsParaSvr() {
	// TODO Auto-generated destructor stub
}

void* ApsParaSvr::flightModeSvr(void* args) {
	int sockfd;
	int ret;
	char buf[2048];
	bool isFound;
	APS_DATA_REQUEST request;
	APS_DATA_RESPONSE response;
	list<int> *flight = &flightModes;
	list<int>::iterator it;
	for(it=flight->begin(); it!=flight->end();it++)
	{
		isFound = false;
		for (int j = 0; j < paramnum; j++) {
			//printf("[APS] %d vs %d\n", g_hparameter[j].Id, *it);
			if(g_hparameter[j].Id == *it){
				isFound=true;
				strcpy(request.signal[request.signalnum].signame,
						g_hparameter[j].Name);
				request.signalnum++;
			}
		}
		if(! isFound){
			printf("[APS] flightModeSvr: can not find id%d\n", *it);
		}
		if(request.signalnum>=10){
			break;
		}
	}
	request.eventType = subscribeAvionicParameters;

	if (!initClientUdpSocket(g_apsconfig->Multicast_Addr,
			atoi(g_apsconfig->Multicast_Port),
					&sockfd)) {
		printf("[APS] can not setup socket for flight modes\n");
		return (void *) 1;
	}
	printf("[APS] flight modes begin\n");
	int counter =0;
	while (isRun) {
			sleep(1); //reduce 5s to 1s; for stop facility
			if (counter++ % 5 != 0) {
				continue;
			}
			memset(buf, 0, 2048);
			memset(&response, 0, sizeof(APS_DATA_RESPONSE));
			updateSigValueFromAPSList(&request, &response);
			HDMessageHandle::SetApsResponseMsg(response, buf, true);
			ret=send(sockfd, buf, strlen(buf), 0);
			if (ret != strlen(buf)) {
				printf("[APS] send flight modes failed%d\n", ret);
			}
	}
	close(sockfd);
	printf("[APS] flight modes quit\n");

	return (void *) 0;
}
// Abandoned now.
void* ApsParaSvr::apsListen(void* args) {
	int *_serverSockfd = (int*) args;
	sockaddr_in clientAddr;
	size_t len = sizeof(clientAddr);

	while (isRun) {
		usleep(100000);
		int clientSockfd = accept(*_serverSockfd,
				(struct sockaddr*) &clientAddr, &len);
		if (clientSockfd < 0) {
			//printf("Server accept failed! (error code %d: %s)\n",errno,strerror(errno));
		} else {
			FD_SET(clientSockfd, &allSet);
			printf("[APS] connected from ip:%s, port:%d\n",
					inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);
		}
	}
	printf("[APS] ApsListen quit\n");
	return (void *) 0;
}

bool ApsParaSvr::acceptClient(int sockfd) {
	int i, clientFd;
	sockaddr_in addr;
	size_t len = sizeof(addr);

	clientFd = accept(sockfd, (struct sockaddr*) &addr, &len);
	if (clientFd < 0) {
		//Nothing to do
	} else if (clientFd > 0) {
		printf("[APS] New connect accepted![sockfd:%d,ip:%s,port:%d]\n",
				clientFd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
		for (i = 0; i < MAX_REMOTE_NUMBER; i++) {
			if (cliPair[i].sockfd == -1) {
				cliPair[i].sockfd = clientFd;
				cliPair[i].addr = addr;
				break;
			}
		}
		FD_SET(clientFd, &allSet);
		if (clientFd > maxFd) {
			maxFd = clientFd;
		}
		if (i > maxCliIndex) {
			maxCliIndex = i;
		}
	}

	return true;
}

void * ApsParaSvr::publishTimer(void* arg) {
	char buf[2048];
	int rtn;
	printf("[APS] publish Timer begin\n");
	while (isRun) {
		ApsPubMsg::iterator nxt, it = pubMsgs.begin();
		while (it != pubMsgs.end()) {
			/* exist publish port, change parameters only */
			APS_DATA_RESPONSE response;
			CAirPubMsg *tmp = &(*it);
			nxt = it;
			nxt++;
			tmp->oldPeriod--;
			if (tmp->oldPeriod == 0) {
				tmp->oldPeriod = tmp->period;
				memset(buf, 0, 2048);
				memset(&response, 0, sizeof(APS_DATA_RESPONSE));
				updateSigValueFromAPSList(&tmp->airdatarequst, &response);
				strcpy(response.methogname, "publishAvionicParameters");
				HDMessageHandle::SetApsResponseMsg(response, buf,true);
				rtn = send(tmp->sockfd, buf, strlen(buf), 0);
				if (rtn != strlen(buf)) {
					printf("[APS] send cycle msg failed, remove sock: ++%d\n",
							tmp->sockfd);
					boost::mutex::scoped_lock(muPub);
					pubMsgs.erase(it);
				}
			}
			it = nxt;
		}
		usleep(1000000);
	}
	printf("[APS] publish Timer end\n");
	return (void*) 0;
}

void * ApsParaSvr::eventTimer(void* arg) {
	char buf[2048];
	int rtn;
	printf("[APS] event Timer begin\n");
	while (isRun) {
		ApsPubMsg::iterator nxt, it = eventMsgs.begin();
		while (it != eventMsgs.end()) {
			/* exist publish port, change parameters only */
			APS_DATA_RESPONSE response;
			CAirPubMsg *tmp = &(*it);
			nxt = it;
			nxt++;
			updateSigValueFromAPSList(&tmp->airdatarequst, &response);
			rtn = isSignalChange(tmp, &response);
			/*if (debug)
				printf("[APS--event data changed] %d\n", rtn);*/
			if (rtn) {
				memset(buf, 0, 2048);
				//updateSigValueFromAPSList(&tmp->airdatarequst, &response);
				strcpy(response.methogname, "onEventAvionicParameters");
				HDMessageHandle::SetApsResponseMsg(response, buf,true);
				rtn = send(tmp->sockfd, buf, strlen(buf), 0);
				if (rtn != strlen(buf)) {
					printf("[APS] send event msg failed, remove sock: ++%d\n",
							tmp->sockfd);
					boost::mutex::scoped_lock(muEvent);
					pubMsgs.erase(it);
				}
			}
			it = nxt;
		}
		usleep(1000000); // 1 s
	}
	printf("[APS] event Timer end\n");
	return (void*) 0;
}

bool ApsParaSvr::publishMgr(RequestWithSock* request) {
	bool isFound = false;
	ApsPubMsg::iterator it = pubMsgs.begin();
	if (debug)
		printf("[APS] pubMsgs size:%d\n", pubMsgs.size());
	while (it != pubMsgs.end()) {
		/* exist publish port, change parameters only
		 * multiple port is not handle now */
		if (request->cli.addr.sin_addr.s_addr == (*it).addr.sin_addr.s_addr
				&& request->req.publishPort[0] == (*it).port) {
			if (false) {
				(*it).oldPeriod = request->req.refreshperiod / 1000;
				(*it).period = request->req.refreshperiod / 1000;
				(*it).airdatarequst = request->req;
			}
			isFound = true;
			if (debug)
				printf("[APS] is found\n, discard this request!\n");
		}
		it++;

	}
	if (debug)
		printf("[APS] while found: %d\n", isFound);
	/* add new msg into pubMsgs for new port */
	if (!isFound) {
		CAirPubMsg tmp;
		memset(&tmp, 0, sizeof(CAirPubMsg));
		tmp.addr = request->cli.addr;
		tmp.airdatarequst = request->req;
		tmp.oldPeriod = request->req.refreshperiod / 1000;
		tmp.period = request->req.refreshperiod / 1000;
		tmp.port = request->req.publishPort[0];
		if (initClientSocket(inet_ntoa(tmp.addr.sin_addr), tmp.port,
				&tmp.sockfd)) {
			boost::mutex::scoped_lock(muPub);
			pubMsgs.push_back(tmp);
			if (debug)
				printf("[APS] New sock in\n");
		}
		//printf("[APS] New sock in2\n");
	}

	return true;
}

bool ApsParaSvr::eventMgr(RequestWithSock* request,
		APS_DATA_RESPONSE * response) {
	bool isFound = false;
	ApsPubMsg::iterator it = eventMsgs.begin();
	if (debug)
		printf("[APS] eventMsgs size:%d\n", eventMsgs.size());
	while (it != eventMsgs.end()) {
		/*printf("[compare: %d, %d --  %d, %d]\n",
		 request->cli.addr.sin_addr.s_addr,
		 (*it).addr.sin_addr.s_addr,
		 request->req.publishPort[0],
		 (*it).port
		 );*/
		/* exist publish port, change parameters only
		 * multiple port is not handle now.*/
		if (request->cli.addr.sin_addr.s_addr == (*it).addr.sin_addr.s_addr
				&& request->req.publishPort[0] == (*it).port) {
			if (false) {
				(*it).airdatarequst = request->req;
				for (int i = 0; i < request->req.signalnum; i++) {
					memset((*it).apsparameter[i].value, 0, 255);
					strcpy((char*) (*it).apsparameter[i].value,
							(char*) response->signal[i].value);
					// need switch here for supported types.
					(*it).ParamVale[i].value1 = response->ParamVale[i].value1;
				}
			}
			isFound = true;
			if (debug)
				printf("[APS] is found, discard this request!\n");
		}
		it++;

	}
	if (debug)
		printf("[APS] while found: %d\n", isFound);
	/* add new msg into eventMsgs for new port */
	if (!isFound) {
		CAirPubMsg tmp;
		memset(&tmp, 0, sizeof(CAirPubMsg));
		tmp.addr = request->cli.addr;
		tmp.airdatarequst = request->req;
		tmp.port = request->req.publishPort[0];
		for (int i = 0; i < request->req.signalnum; i++) {
			memset(tmp.apsparameter[i].value, 0, 255);
			strcpy((char*) tmp.apsparameter[i].value,
					(char*) response->signal[i].value);
			// need switch here for supported types.
			tmp.ParamVale[i].value1 = response->ParamVale[i].value1;
		}
		if (initClientSocket(inet_ntoa(tmp.addr.sin_addr), tmp.port,
				&tmp.sockfd)) {
			boost::mutex::scoped_lock(muEvent);
			eventMsgs.push_back(tmp);
			if (debug)
				printf("[APS] New event sock in\n");
		}
		//printf("[APS] New sock in2\n");
	}

	return true;
}

/* remove publish/event msg from list */
bool ApsParaSvr::unSubscribeMgr(RequestWithSock* request, bool needDel) {
	int ports[5] = { 0, 0, 0, 0, 0 };
	bool isSucc = true;
	/*printf("[APS] we are in ports:%d:\n", request->req.publishPortNum);
	 for (int i = 0; i < request->req.publishPortNum; i++) {
	 printf("i:%d:%d:\n", i, request->req.publishPort[i]);
	 }*/
	/*********** check event msg list *************/
	ApsPubMsg::iterator nxt, it = eventMsgs.begin();
	if (debug)
		printf("[APS] eventMsgs size:%d\n", eventMsgs.size());
	while (it != eventMsgs.end()) {
		nxt = it;
		nxt++;
		/* exist publish port, then remove it */
		if (request->cli.addr.sin_addr.s_addr == (*it).addr.sin_addr.s_addr) {
			for (unsigned int i = 0; i < request->req.publishPortNum; i++) {
				if (request->req.publishPort[i] == (*it).port) {
					ports[i]++;
					if (needDel) {
						close((*it).sockfd);
						boost::mutex::scoped_lock(muEvent);
						eventMsgs.erase(it);
					}
					if (debug)
						printf("[APS] unsubscribe:ip:%s,port:%d,tport:%d\n",
								inet_ntoa(request->cli.addr.sin_addr),
								request->cli.addr.sin_port, (*it).port);
				}

			}
		}
		it = nxt;
	}
	/*********** check publish msg list *************/
	it = pubMsgs.begin();
	if (debug)
		printf("[APS] pubMsgs size:%d\n", pubMsgs.size());
	while (it != pubMsgs.end()) {
		nxt = it;
		nxt++;
		/* exist publish port, then remove it */
		if (request->cli.addr.sin_addr.s_addr == (*it).addr.sin_addr.s_addr) {
			for (unsigned int i = 0; i < request->req.publishPortNum; i++) {
				if (request->req.publishPort[i] == (*it).port) {
					ports[i]++;
					if (needDel) {
						close((*it).sockfd);
						boost::mutex::scoped_lock(muPub);
						pubMsgs.erase(it);
					}
					if (debug)
						printf("[APS] unsubscribe:ip:%s,port:%d,tport:%d\n",
								inet_ntoa(request->cli.addr.sin_addr),
								request->cli.addr.sin_port, (*it).port);
				}
			}
		}
		it = nxt;
	}
	/*********** check all ports are handle? *************/
	for (unsigned int i = 0; i < request->req.publishPortNum; i++) {
		if (ports[i] == 0) {
			isSucc = false;
			printf("[APS] unsubscribe [%d -- %d]\n", ports[i],
					request->req.publishPort[i]);
			if (debug)
				printf("[APS] unsubscribe [port not found]:ip:%s,port:%d\n",
						inet_ntoa(request->cli.addr.sin_addr),
						request->req.publishPort[i]);
		}
	}

	return isSucc;
}

void* ApsParaSvr::apsProcessMsg(void* args) {
	char buf[2048];
	int rtn;
	bool isPortValid;
	printf("[APS] process Msg begin\n");
	while (isRun) {
		usleep(100 * 1000);
		{
			boost::mutex::scoped_lock(muMsg);
			ClientMsg::iterator it = cliMsgs.begin();
			while (it != cliMsgs.end()) {
				APS_DATA_RESPONSE response;
				RequestWithSock tmp = *it;
				memset(buf, 0, 2048);
				memset(&response, 0, sizeof(APS_DATA_RESPONSE));
				if (debug)
					printf("--->%d: %s\n", tmp.req.eventType,
							tmp.req.methodname);
				updateSigValueFromAPSList(&tmp.req, &response);
				// check request constrains, and set err code.
				validateRequest(&tmp.req, &response);
				if (tmp.req.eventType == apsunSubscribe) {
					isPortValid = unSubscribeMgr(&tmp, false);
					if (!isPortValid) {
						response.errcode = 7; // port not found in list
					}
				}
				HDMessageHandle::SetApsResponseMsg(response, buf, false);
				//if (response.errcode != 0 && response.errcode == 8){
				rtn = send(tmp.cli.sockfd, buf, strlen(buf), 0);
				if (rtn != strlen(buf)) {
					printf(
							"[APS] send out response error:%d:\n%s\n-------------\n",
							rtn, buf);
				}
				//}

				// only handle good request.
				if (response.errcode == 0 || response.errcode == 8) {
					switch (tmp.req.eventType) {
					case getAvionicParameters:
						//already send out one response.
						break;
					case subscribeAvionicParameters:
						if (debug)
							printf("into case cycle subs-->%d\n",
									tmp.req.eventType);
						publishMgr(&tmp);
						break;
					case subscribeAvionicParametersOnEvent:
						if (debug)
							printf("into case event subs-->%d\n",
									tmp.req.eventType);
						eventMgr(&tmp, &response);
						break;
					case apsunSubscribe:
						//UnSubscribe handle
						unSubscribeMgr(&tmp, true);
						break;
					default:
						break;
					}
				}
				it++;
				cliMsgs.pop_front();
			}
		}

	}
	printf("[APS] process Msg stop\n");
	return (void *) 0;
}

void* ApsParaSvr::apsProcess(void* args) {
	int serverSockfd = *(int*) args;
	int ret, i, nready;
	int maxFd = serverSockfd;
	struct timeval timeout;
	char readBuffer[2048];
	fd_set readSet;
	//add server sock into allSet
	FD_ZERO(&allSet);
	FD_SET(serverSockfd, &allSet);
	memset(cliPair, 0, sizeof(EthernetPair) * MAX_REMOTE_NUMBER);
	for (i = 0; i < MAX_REMOTE_NUMBER; i++) {
		cliPair[i].sockfd = -1;
	}
	printf("[APS] Listen begin\n");
	while (isRun) {
		// check time 100 ms
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;
		readSet = allSet;
		nready = select(maxFd + 1, &readSet, NULL, NULL, &timeout);
		//printf("nready:%d, %d\n",nready, allSet.fds_bits[0]);
		if (nready < 0) {
			printf("[APS] select error! errorCode:%d,errorMsg:%s\n", errno,
					strerror(errno));
			continue;
		} else if (nready == 0) {
			// timeout
			// nothing else changed,if in block mode,no need to have this section
			continue;
		}
		if (debug)
			printf("nready:%d\n", nready);
		//**************************************************
		//server socket handle,add new connect requested
		//**************************************************
		if (FD_ISSET(serverSockfd, &readSet)) {
			//acceptClient(serverSockfd); why does not work in a sub function?
			int i, clientFd;
			sockaddr_in addr;
			size_t len = sizeof(addr);

			clientFd = accept(serverSockfd, (struct sockaddr*) &addr, &len);
			if (clientFd < 0) {
				//Nothing to do
			} else if (clientFd > 0) {
				printf("[APS] New connect accepted![sockfd:%d,ip:%s,port:%d]\n",
						clientFd, inet_ntoa(addr.sin_addr),
						ntohs(addr.sin_port));

				for (i = 0; i < MAX_REMOTE_NUMBER; i++) {
					if (cliPair[i].sockfd == -1) {
						cliPair[i].sockfd = clientFd;
						cliPair[i].addr = addr;
						break;
					}
				}
				FD_SET(clientFd, &allSet);
				if (clientFd > maxFd) {
					maxFd = clientFd;
				}
				if (i > maxCliIndex) {
					maxCliIndex = i;
				}
			}
			//skip client handle in case of only svr sock change.
			if (--nready <= 0) {
				continue;
			}
		}
		//**************************************************
		// client socket handle
		//**************************************************
		for (i = 0; i <= maxCliIndex; i++) {
			if (cliPair[i].sockfd < 0) {
				continue;
			}
			if (FD_ISSET(cliPair[i].sockfd, &readSet)) {
				memset(readBuffer, 0, 2048);
				if ((ret = recv(cliPair[i].sockfd, readBuffer, 2048, 0)) == 0) {// disconnect! but fd still can be read,close remote fd here.
					close(cliPair[i].sockfd);
					FD_CLR(cliPair[i].sockfd, &allSet);
					cliPair[i].sockfd = -1;
				} else {
					//deal with received data
					if (debug)
						printf(
								"*********************************************\n");
					if (debug)
						printf("[APS] data received! sockfd:%d,data:%s\n",
								cliPair[i].sockfd, readBuffer);
					if (debug)
						printf("[client ip:%s,port:%d]\n",
								inet_ntoa(cliPair[i].addr.sin_addr),
								ntohs(cliPair[i].addr.sin_port));
					if (debug)
						printf(
								"*********************************************\n");
					APS_DATA_REQUEST request;
					memset(&request, 0, sizeof(APS_DATA_REQUEST));
					HDMessageHandle::GetApsRequestMsg(&request, readBuffer,
							ret);
					if (debug)
						printf("[eventType:%d @@@ %s]\n", request.eventType,
								request.methodname);
					if (request.eventType != apsInvalidMsg) {
						boost::mutex::scoped_lock(muMsg);
						RequestWithSock client = { cliPair[i], request };
						cliMsgs.push_back(client);
					}
				}
			}
		}
	}
	printf("[APS] Listen quit\n");
	return (void *) 0;
}

bool ApsParaSvr::initServerSocket(int localPort, int* sockfd) {
	int ret = 0;
	sockaddr_in addr;
	int bReuseaddr = 1;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(localPort);
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	*sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (true) {
		if ((ret = fcntl(*sockfd, F_SETFL, O_NONBLOCK)) < 0) {
			printf("[APS] TCPServer set nonblock failed!\n");
			return false;
		}
	}
	if (true) {
		if ((ret = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR,
				(char *)&bReuseaddr, sizeof(bReuseaddr))) < 0) {
			printf("[APS-HD] TCPServer set bReuseaddr failed! %d\n", ret);
			return false;
		}
	}
	ret = bind(*sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0) {
		printf("[APS] TCPServer Could not bind requested port:%u\n)",
				localPort);
		return false;
	}

	ret = listen(*sockfd, MAX_REMOTE_NUMBER);
	if (ret < 0) {
		printf("[APS] Could not listen requested address (error code %d)\n",
		errno);
		return false;
	}
	printf("[APS] start at port: %d\n", localPort);

	return true;
}

bool ApsParaSvr::initClientSocket(char* ip, int port, int *sockfd) {
	int ret = 0;

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	*sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ret = connect(*sockfd, (struct sockaddr*) &addr, sizeof(addr));
	if (ret < 0) {
		printf("[APS] Create connect to server(IP:%s,port:%d) failed\n", ip,
				port);
		return false;
	} else {
		printf("[APS] Create connect to server(IP:%s,port:%d) success\n", ip,
				port);
	}

	return true;

}

bool ApsParaSvr::initClientUdpSocket(char* ip, int port, int *sockfd) {
	int ret = 0;

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	*sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ret = connect(*sockfd, (struct sockaddr*) &addr, sizeof(addr));
	if (ret < 0) {
		printf("[APS] Create connect to server(IP:%s,port:%d) failed\n", ip,
				port);
		return false;
	} else {
		printf("[APS] Create connect to server(IP:%s,port:%d) success\n", ip,
				port);
	}

	return true;

}

/* aps will stop process on error 1,2,5,7 */
bool ApsParaSvr::validateRequest(APS_DATA_REQUEST *request,
		APS_DATA_RESPONSE * response) {
	switch (request->eventType) {
	case getAvionicParameters:
		// always equal, as NF is kept.
		if (request->signalnum != response->signalnum
				|| request->signalnum == 0) {
			response->errcode = 8; // data not found
		}
		for (int i = 0; i < request->signalnum; i++) {
			if (request->signal[i].status == 4) {
				response->errcode = 8; // data not found
				break;
			}
		}

		break;
	case subscribeAvionicParameters:
		if (request->signalnum != response->signalnum
				|| request->signalnum == 0) {
			response->errcode = 8; // data not found
		} else if (request->publishPort[0] <= 0
				|| request->publishPort[0] > 65536) {
			response->errcode = 7; // invalid port
			//printf("--------------->++ %d\n", request->publishPort[0]);
			//printf("--------------->++ %d\n", request->publishPort[1]);
		} else if (request->refreshperiod % 1000 != 0
				|| request->refreshperiod > 3600000
				|| request->refreshperiod < 1000) {
			response->errcode = 5; // invalid refresh period
		} else {
			for (int i = 0; i < request->signalnum; i++) {
				if (request->signal[i].status == 4) {
					response->errcode = 8; // data not found
					break;
				}
			}
		}
		break;
	case subscribeAvionicParametersOnEvent:
		if (request->signalnum != response->signalnum
				|| request->signalnum == 0) {
			response->errcode = 8; // data not found
		} else if (request->publishPort[0] <= 0
				|| request->publishPort[0] > 65536) {
			response->errcode = 7; // invalid port
		} else {
			for (int i = 0; i < request->signalnum; i++) {
				if (request->signal[i].status == 4) {
					response->errcode = 8; // data not found
					break;
				}
			}
		}
		break;
	case apsunSubscribe:
		for (unsigned int i = 0; i < request->publishPortNum; i++) {
			if (request->publishPort[i] <= 0
					|| request->publishPort[i] > 65536) {
				response->errcode = 7; // invalid port
			}
		}
		if (request->publishPortNum == 0) {
			response->errcode = 7;
		}
		break;
	case apsInvalidMsg:
		response->errcode = 2;
		break;

	case apsInvalidXML:
		response->errcode = 1;
		break;
	default:
		break;
	}
	return true;
}

bool ApsParaSvr::updateSigValueFromAPSList(APS_DATA_REQUEST *request,
		APS_DATA_RESPONSE * response) {
	static char *APSResponseSty[4] = { "getAvionicParametersResponse",
			"subscribeAvionicParametersResponse",
			"subscribeAvionicParametersOnEventResponse", "unSubscribeResponse" };
	bool isFound;
	CParamData *pItem;
	pItem = g_hparameter;
	response->type = request->eventType;

	switch (request->eventType) {
	case getAvionicParameters:
		strcpy(response->methogname, APSResponseSty[0]);
		break;
	case subscribeAvionicParameters:
		strcpy(response->methogname, APSResponseSty[1]);
		break;
	case subscribeAvionicParametersOnEvent:
		strcpy(response->methogname, APSResponseSty[2]);
		break;
	case apsunSubscribe:
		strcpy(response->methogname, APSResponseSty[3]);
		break;
	default:
		break;
	}
	if (request->eventType == apsInvalidMsg) {
		return false;
	}

	response->signalnum = 0;
	for (int i = 0; i < request->signalnum; i++) {
		isFound = false;
		for (int j = 0; j < paramnum; j++) {
			int n = strcmp(request->signal[i].signame, g_hparameter[j].Name);
			if (n == 0) {
				isFound = true;
				strcpy(response->signal[i].signame, request->signal[i].signame);
				// get from global parameters
				request->signal[i].status = g_hparameter[j].Status;
				request->signal[i].type = g_hparameter[j].Type;
				request->signal[i].length = g_hparameter[j].Length;
				// copy from request
				response->signal[i].type = request->signal[i].type;
				response->signal[i].status = request->signal[i].status;
				response->signal[i].length = request->signal[i].length;

				/*memset(request->signal[i].value, 0, 255);
				memset(response->signal[i].value, 0, 255);
				strncpy((char*) request->signal[i].value,
						(char*) g_hparameter[j].Value, g_hparameter[j].Length);
				strncpy((char*) response->signal[i].value,
						(char*) request->signal[i].value,
						request->signal[i].length);*/

				response->signal[i].time = g_hparameter[j].time;

				switch (response->signal[i].type) {
				case TYPE_F64B:
					response->ParamVale[i].value1 =
							g_hparameter[j].ParamVale.value1;
					break;
				case TYPE_S64B:
					response->ParamVale[i].value2 =
							g_hparameter[j].ParamVale.value2;
					break;
				case TYPE_F32B:
					response->ParamVale[i].value3 =
							g_hparameter[j].ParamVale.value3;
					break;
				case TYPE_U32B:
					// discard
					break;
				case TYPE_S32B:
				case TYPE_OPAQUE:
				case TYPE_ENUM:
					response->ParamVale[i].value5 =
							g_hparameter[j].ParamVale.value5;
					//printf("wel---->%d\n", response->ParamVale[i].value5);
					break;
				case TYPE_BOOL:
					response->ParamVale[i].value6 =
							g_hparameter[j].ParamVale.value6;
					break;
				case TYPE_STRING:
				case TYPE_DATE:
				case TYPE_TIME:
					memset(response->ParamVale[i].value, 0, 255);
					strcpy(response->ParamVale[i].value,
							g_hparameter[j].ParamVale.value);
					break;
				default:
					printf("[APS] Bad use type on TYPE_UNKNOW\n");
					break;

				}
				response->signalnum++;
			}

		}
		// for NF-4 parameters, still keep it.
		if (isFound == false) {
			strcpy(response->signal[i].signame, request->signal[i].signame);
			response->signal[i].status = 4;
			request->signal[i].status = 4;
			response->signalnum++;
			response->errcode = 8; //Data not found.
		}
	}

	return true;
}

bool ApsParaSvr::isSignalChange(CAirPubMsg *tmp, APS_DATA_RESPONSE* response) {
	int changeNum = 0;
	for (int j = 0; j < tmp->airdatarequst.signalnum; j++) {
		//printf("[compare: %f, %f]\n", tmp->ParamVale[j].value1, response->ParamVale[j].value1);
		switch (tmp->airdatarequst.signal[j].type) {
		case TYPE_F64B:
			//  const float EPSINON = 0.00001; if ((x >= - EPSINON) && (x <= EPSINON)
			if (tmp->ParamVale[j].value1 != response->ParamVale[j].value1) {
				changeNum++;
				tmp->ParamVale[j].value1 = response->ParamVale[j].value1;
			}
			break;
		case TYPE_S64B:
			if (tmp->ParamVale[j].value2 != response->ParamVale[j].value2) {
				changeNum++;
				tmp->ParamVale[j].value2 = response->ParamVale[j].value2;
			}
			break;
		case TYPE_F32B:
			if (tmp->ParamVale[j].value3 != response->ParamVale[j].value3) {
				changeNum++;
				tmp->ParamVale[j].value3 = response->ParamVale[j].value3;
			}
			break;
		case TYPE_U32B:
			if (tmp->ParamVale[j].value4 != response->ParamVale[j].value4) {
				changeNum++;
				tmp->ParamVale[j].value4 = response->ParamVale[j].value4;
			}
			break;
		case TYPE_S32B:
		case TYPE_OPAQUE:
		case TYPE_ENUM:
			if (tmp->ParamVale[j].value5 != response->ParamVale[j].value5) {
				changeNum++;
				tmp->ParamVale[j].value5 = response->ParamVale[j].value5;
			}
			break;
		case TYPE_BOOL:
			if (tmp->ParamVale[j].value6 != response->ParamVale[j].value6) {
				changeNum++;
				tmp->ParamVale[j].value6 = response->ParamVale[j].value6;
			}
			break;
		case TYPE_STRING:
		case TYPE_DATE:
		case TYPE_TIME:
			if (strcmp(tmp->ParamVale[j].value, response->ParamVale[j].value)
					!= 0) {
				changeNum++;
				memset(tmp->ParamVale[j].value, 0, 80);
				strcpy(tmp->ParamVale[j].value, response->ParamVale[j].value);
			}
			break;
		default:
			break;

		}
		//end switch
	} //end for
	return changeNum != 0;
}

bool ApsParaSvr::aps_start(bool isDebug) {
	debug = isDebug;
//g_apsconfig = cfg;
	if (g_apsconfig == NULL) {
		printf("[APS] Error: Invalid Config\n");
		return false;
	}
//printf("port:%s\n", g_apsconfig->APSvr_Port_PARAM);
	int localPort = atoi(g_apsconfig->APSvr_Port_PARAM);
	if (!initServerSocket(localPort, &apsSvr)) {
		printf("[APS] can not start server!\n");
		return false;
	}
	isRun = true;
	printf("[APS] startup now.\n");
//ApsParaSvr::apsProcess(&ApsParaSvr::apsSvr);

	pthread_create(&apsProcHandle, NULL, ApsParaSvr::apsProcess, &apsSvr);

	pthread_create(&apsMsgHandle, NULL, ApsParaSvr::apsProcessMsg, (void*) 0);

	pthread_create(&apsPubHandle, NULL, ApsParaSvr::publishTimer, (void*) 0);

	pthread_create(&apsEventHandle, NULL, ApsParaSvr::eventTimer, (void*) 0);

	pthread_create(&apsEventHandle, NULL, ApsParaSvr::flightModeSvr, (void*) 0);

	printf("[APS] *************************\n");
	printf("[APS] * Parameter Server Start!\n");
	printf("[APS] *************************\n");
	return true;
}

bool ApsParaSvr::aps_stop() {
//void* status;
	isRun = false;
	sleep(1);
	/*	pthread_join(apsProcHandle, &status);
	 pthread_join(apsMsgHandle, &status);
	 pthread_join(apsPubHandle, &status);
	 pthread_join(apsEventHandle, &status);*/
//close server
	close(apsSvr);
	if (debug)
		printf("close apsSvr %d\n", apsSvr);
//close clients.
	boost::mutex::scoped_lock(muEvent);
	boost::mutex::scoped_lock(muPub);
	ApsPubMsg::iterator it = eventMsgs.begin();
	while (it != eventMsgs.end()) {
		if ((*it).sockfd > 0) {
			close((*it).sockfd);
			if (debug)
				printf("close %d\n", (*it).sockfd);
		}
		it++;
	}
	eventMsgs.clear();
	it = pubMsgs.begin();
	while (it != pubMsgs.end()) {
		if ((*it).sockfd > 0) {
			close((*it).sockfd);
			if (debug)
				printf("close %d\n", (*it).sockfd);
		}
		it++;
	}
	pubMsgs.clear();
	printf("[APS] **********************\n");
	printf("[APS] Parameter Server Quit!\n");
	printf("[APS] **********************\n");

	return true;
}

bool ApsParaSvr::isRun;
int ApsParaSvr::maxFd;
int ApsParaSvr::maxCliIndex;
fd_set ApsParaSvr::allSet;
EthernetPair ApsParaSvr::cliPair[MAX_REMOTE_NUMBER];
int ApsParaSvr::apsSvr;
ClientMsg ApsParaSvr::cliMsgs;

CParamData * ApsParaSvr::g_hparameter;
int ApsParaSvr::paramnum;
APS_CONFIG* ApsParaSvr::g_apsconfig;
list<int> ApsParaSvr::flightModes;

ApsPubMsg ApsParaSvr::pubMsgs;
ApsPubMsg ApsParaSvr::eventMsgs;
bool ApsParaSvr::debug;

pthread_t ApsParaSvr::apsProcHandle;
pthread_t ApsParaSvr::apsMsgHandle;
pthread_t ApsParaSvr::apsPubHandle;
pthread_t ApsParaSvr::apsEventHandle;

boost::mutex ApsParaSvr::muEvent;
boost::mutex ApsParaSvr::muPub;
boost::mutex ApsParaSvr::muMsg;

