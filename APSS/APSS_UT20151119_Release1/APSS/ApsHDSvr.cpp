/*
 * ApsHDSvr.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: root
 */

#include "ApsHDSvr.h"

using namespace HDServer;

ApsHDSvr::ApsHDSvr() {
	// TODO Auto-generated constructor stub
	sockHDsvr = -1;
	sockHDpub = -1;
}

ApsHDSvr::~ApsHDSvr() {
	// TODO Auto-generated destructor stub
}

bool ApsHDSvr::registHDSvr(int sockfd) {
	char buffer[1024];
	int ret;
	memset(buffer, 0, 1024);
	ST_APP_REGISTER_DATA data;
	ST_APP_REGISTER_RESPONSE_DATA response;

	memset(&data, 0, sizeof(ST_APP_REGISTER_DATA));
	strcpy(data.methodName, "applicationRegister");
	strcpy(data.appName, g_apsconfig->APS_Name);
	strcpy(data.partNum, g_apsconfig->PartNumber);
	strcpy(data.passwd, g_apsconfig->Password);
	strcpy(data.platform, g_apsconfig->Platform);
	data.processID = g_apsconfig->ProcessId;

	HDMessageHandle::SetRegisterData(data, buffer);
	for (int i = 0; i < 4; i++) {
		ret = send(sockfd, buffer, strlen(buffer), 0);
		if (ret != strlen(buffer)) {
			printf("[APS-HD] regist send error %d, %d\n", ret, strlen(buffer));
			continue;
		}
		usleep(200 * 1000);
		memset(buffer, 0, 1024);
		ret = recv(sockfd, buffer, 1023, 0);
		if (ret <= 0) {
			continue;
		}
		//parseRespnse
		HDMessageHandle::GetRegisterResponseData(&response, buffer, strlen(
				buffer));
		//printf("[APS-HD] recv %d:%s\n", ret, buffer);
		if (response.errorCode == 0 && strcmp(response.resName,
				"applicationRegisterResponse") == 0) {
			isRegist = true;
			strcpy(g_apsconfig->Signature, response.signature);
			break;
		} else {
			printf("[APS-HD] response errcode: %d:%s\n", response.errorCode,
					response.resName);
		}

	}
	if (!isRegist) {
		printf("[APS-HD] regist error\n");
		return false;
	}
	printf("[APS-HD] regist OK\n");
	return true;
}

bool ApsHDSvr::subscribeDataSend(int sockfd, ST_DATA_REQUEST *request,
		ST_DATA_RESPONSE *response) {
	char buffer[2048];
	int ret;
	memset(buffer, 0, 2048);
	HDMessageHandle::SetSubscribeAvionicsData(*request, buffer);
	for (int i = 0; i < 3; i++) {
		ret = send(sockfd, buffer, strlen(buffer), 0);
		if (ret != strlen(buffer)) {
			printf("[APS-HD] subscribeData send error %d, %d\n", ret, strlen(
					buffer));
			continue;
		}
		usleep(200 * 1000);
		memset(buffer, 0, 2048);
		for (int j = 0; j < 3; j++) {
			ret = recv(sockfd, buffer, 2048, 0);
			if (ret <= 0) {
				usleep(200 * 1000);
				continue;
			}

			//parseRespnse
			HDMessageHandle::GetSubscribeAvionicsDataPesponse(response, buffer,
					strlen(buffer));
			//printf("[APS-HD] recv %d:%s\n", ret, buffer);
			if (response->errorCode == 0 && strcmp(response->resName,
					"subscribeAvionicsDataResponse") == 0) {
				return true;
			} else {
				printf("[APS-HD] response errcode: %d:%s\n",
						response->errorCode, response->resName);
			}
		}

	}
	return false;
}

bool ApsHDSvr::unSubscribe(int sockfd, int ports[5]) {
	char buffer[1024];
	ST_DATA_REQUEST request;
	ST_DATA_RESPONSE response;
	int ret, num;
	memset(buffer, 0, 1024);
	memset(&request, 0, sizeof(ST_DATA_REQUEST));
	memset(&response, 0, sizeof(ST_DATA_RESPONSE));
	strcpy(request.methodName, "unSubscribe");
	strcpy(request.appName, g_apsconfig->APS_Name);
	strcpy(request.signature, g_apsconfig->Signature);
	num = 0;
	for (int i = 0; i < 5; i++) {
		if (ports[i] != 0) {
			request.unSubscribePublishPort[num++] = ports[i];
		}
	}
	request.unSubscribePublishPortNum = num;
	if (num == 0) {
		return false;
	}
	HDMessageHandle::SetUnSubscribe(request, buffer);
	ret = send(sockfd, buffer, strlen(buffer), 0);
	if (debug)
		printf("[APS-HD] unSubscribe send %d, %s\n", ret, buffer);
	if (ret != strlen(buffer)) {
		printf("[APS-HD] unSubscribe send error %d, %d\n", ret, strlen(buffer));
		return false;
	}
	usleep(100 * 1000);
	memset(buffer, 0, 1024);
	ret = recv(sockfd, buffer, 1023, 0);
	if (debug)
		printf("[APS-HD] unSubscribe recv  %d, %s\n", ret, buffer);
	if (ret <= 0) {
		printf("[APS-HD] unSubscribe recv error %d, %d\n", ret, strlen(buffer));
		return false;
	}
	//parseRespnse
	HDMessageHandle::GetUnSubscribePesponse(&response, buffer, strlen(buffer));
	if (response.errorCode == 0) {
		printf("[APS-HD] unSubscribe done\n");
	} else {
		printf("[APS-HD] unSubscribe error:%d\n", response.errorCode);
		return false;
	}
	return true;
}

bool ApsHDSvr::unRegister(int sockfd) {
	char buffer[1024];
	ST_APP_UNREGISTER_DATA request;
	ST_APP_UNREGISTER_RESPONSE_DATA response;
	int ret, num;
	memset(buffer, 0, 1024);
	memset(&request, 0, sizeof(ST_APP_UNREGISTER_DATA));
	memset(&response, 0, sizeof(ST_APP_UNREGISTER_RESPONSE_DATA));
	strcpy(request.methodName, "applicationUnRegister");
	strcpy(request.appName, g_apsconfig->APS_Name);
	strcpy(request.signature, g_apsconfig->Signature);

	HDMessageHandle::SetUnregisterData(request, buffer);
	ret = send(sockfd, buffer, strlen(buffer), 0);
	if (debug)
		printf("[APS-HD] UnRegister send %d, %s\n", ret, buffer);
	if (ret != strlen(buffer)) {
		printf("[APS-HD] UnRegister send error %d, %d\n", ret, strlen(buffer));
		return false;
	}
	usleep(100 * 1000);
	memset(buffer, 0, 1024);
	ret = recv(sockfd, buffer, 1023, 0);
	if (debug)
		printf("[APS-HD] UnRegister recv  %d, %s\n", ret, buffer);
	if (ret <= 0) {
		printf("[APS-HD] UnRegister recv error %d, %d\n", ret, strlen(buffer));
		return false;
	}
	if (debug){
		printf("recv:%s\n", buffer);
	}
	//parseRespnse
	HDMessageHandle::GetUnRegisterDataPesponse(&response, buffer, strlen(buffer));
	if (response.errorCode == 0) {
		printf("[APS-HD] UnRegister done\n");
	} else {
		printf("[APS-HD] UnRegister error:%d\n", response.errorCode);
		return false;
	}
	return true;
}

bool ApsHDSvr::subscribeData(int sockfd, CParamData **pItems) {
	CParamData *pItem;
	ST_DATA_REQUEST request;
	ST_DATA_RESPONSE response;
	char rawSigNames[100][50];
	int rawNum = 0;
	bool isFound;

	memset(&request, 0, sizeof(ST_DATA_REQUEST));
	memset(&response, 0, sizeof(ST_DATA_RESPONSE));

	strcpy(request.methodName, "subscribeAvionicsData");
	strcpy(request.appName, g_apsconfig->APS_Name);
	strcpy(request.signature, g_apsconfig->Signature);
	request.publishPort = atoi(g_apsconfig->APSvr_Port_AIDCP);
	request.refreshPeriod = 1000;
	request.rawDataNum = 0;

	pItem = g_hparameter;

	// retrieve all signal names(ICD) from all parameters
	for (int i = 0; i < paramnum; i++) {
		for (int j = 0; j < pItem[i].paramnum; j++) {
			//check if signal is already added.
			isFound = false;
			for (int k = 0; k < rawNum; k++) {
				if (strcmp(rawSigNames[k], pItem[i].signel[j].signalName) == 0) {
					isFound = true;
					break;
				}
			}
			if (!isFound) {
				strcpy(rawSigNames[rawNum], pItem[i].signel[j].signalName);
				rawNum++;
			}
		}
		if (rawNum >= 50) {
			printf("[Aps-HD] too many SIGNAL to subscribe\n");
			break;
		}
	}

	int n = 0;
	int groupNum = 10; // request data every x signals.
	for (int i = 0; i < rawNum; i++) {

		strcpy(request.rawDataApp[n].rawDataName, rawSigNames[i]);
		n++;
		request.rawDataNum++;
		// in case of last one.
		if ((n % groupNum == 0) || (i == (rawNum - 1))) {
			// ============subscribe handle=========
			if (subscribeDataSend(sockfd, &request, &response)) {
				printf("[APS-HD] subscribeDataSend %d OK\n", request.rawDataNum);
			} else {
				printf("[APS-HD] subscribeDataSend %d Error\n",
						request.rawDataNum);
			}
			n = 0;
			memset(request.rawDataApp, 0, sizeof(ST_RAWDATA_APPLICATION) * 10);
			request.rawDataNum = 0;
		}
	}

	return true;
}
/* send heartbeat package every 5s, block num always increase */
void* ApsHDSvr::heartBeat(void* args) {
	int *_serverSockfd = (int*) args;
	char buffer[1024];
	ST_HEARTBEAT_REQUEST request;
	ST_HEARTBEAT_RESPONSE response;
	memset(&request, 0, sizeof(ST_HEARTBEAT_REQUEST));
	strcpy(request.signature, g_apsconfig->Signature);
	strcpy(request.appName, g_apsconfig->APS_Name);
	strcpy(request.methodName, "heartbeat");
	int ret;
	int lostNum = 0;
	int counter = 0;
	request.block = -1;

	printf("[APS-HD] heart beat start\n");
	while (isRegist) {
		sleep(1); //reduce 5s to 1s; for stop facility
		if (counter++ % 5 != 0) {
			continue;
		}
		request.block = (request.block + 1) % 65536;
		lostNum++;
		// exceeds 12 times
		if (lostNum >= 12) {
			printf("[APS-HD] heart beat timeout!\n");
			apsHd_stop(true);
		}
		HDMessageHandle::SetHeartBeat(request, buffer);
		ret = send(*_serverSockfd, buffer, strlen(buffer), 0);
		if (ret != strlen(buffer)) {
			printf("[APS-HD] heart send error %d, %d\n", ret, strlen(buffer));
			continue;
		}
		usleep(100 * 1000);
		memset(buffer, 0, 1024);
		ret = recv(*_serverSockfd, buffer, 1023, 0);
		if (ret <= 0) {
			printf("[APS-HD] heart recv error %d, %d\n", ret, strlen(buffer));
			continue;
		}
		//parseRespnse
		HDMessageHandle::GetHeartBeatResponse(&response, buffer, strlen(buffer));
		if (response.errorCode == 0 && response.block == request.block) {
			if (debug) printf("[APS-HD] heart beat:%d\n", response.block);
			lostNum = 0;
		} else {
			printf("[APS-HD] heart beat error:%d, block:%d\n",
					response.errorCode, response.block);
		}
	}
	printf("[APS-HD] heart beat quit\n");
	return (void *) 0;
}

void* ApsHDSvr::pubListen(void* args) {
	int *_serverSockfd = (int*) args;
	sockaddr_in clientAddr;
	size_t len = sizeof(clientAddr);
	pthread_t pubParaHandle;
	if (debug)
		printf("[APS-HD] paramnum:%d\n", paramnum);
	while (isRegist) {
		usleep(100000);
		int clientSockfd = accept(*_serverSockfd,
				(struct sockaddr*) &clientAddr, &len);
		if (clientSockfd < 0) {
			//printf("Server accept failed! (error code %d: %s)\n",errno,strerror(errno));
		} else {
			printf("[APS-HD] connected from ip:%s, port:%d\n", inet_ntoa(
					clientAddr.sin_addr), clientAddr.sin_port);
			pthread_create(&pubParaHandle, NULL, pubParaParse,
					(void*) &clientSockfd);
		}
	}
	printf("[APS-HD] pubListen quit\n");
	return (void *) 0;
}

void* ApsHDSvr::pubParaParse(void* args) {
	int *sockfd = (int*) args;
	int clientSock = *sockfd;
	int ret;
	char recvBuf[2048];
	ST_DATA_RESPONSE response;
	memset(recvBuf, 0, 2048);
	memset(&response, 0, sizeof(ST_DATA_RESPONSE));
	if ((ret = fcntl(clientSock, F_SETFL, O_NONBLOCK)) < 0) {
		printf("[APS-HD] clientSock set nonblock failed!\n");
	}
	while (isRegist) {
		usleep(100000);
		if ((ret = recv(clientSock, recvBuf, 2048, 0)) <= 0) {
			//close(clientSock);
			//break;
			continue;
		} else if (ret > 100) {
			//deal with received data
			HDMessageHandle::GetPublishAvionicsDataPesponse(&response, recvBuf,
					strlen(recvBuf));
			if (response.rawDataNum > 0) {
				//update g_para
				updateParams(&response);
				if (debug)
					printf("[APS-HD] %s:%s\n",
							response.rawDataApp[0].rawDataName,
							response.rawDataApp[0].data);
			}
			if (debug)
				printf(
						"[APS-HD] *********************************************\n");
			if (debug)
				printf("[APS-HD] data received! sockfd:%d,data:%s\n",
						clientSock, (char*) recvBuf);
			memset(recvBuf, 0, 2048);
			memset(&response, 0, sizeof(ST_DATA_RESPONSE));
			//printf("[APS-HD] recv %d:%s\n", ret, buffer);

		}
		if (debug) printf("[APS-HD] pubParaParse %d,\n", ret);
	}
	close(clientSock);
	printf("[APS-HD] pubParaParse quit:%d\n", clientSock);
	return (void *) 0;
}

// the bit direction of 429 bit is from low to high.
double ApsHDSvr::get429Value(unsigned char *data, unsigned int startbit,
		unsigned int len, float scale) {
	unsigned int num;
	memcpy(&num, data, 4);
	return ((num << (32 - startbit - len)) >> len) * scale;
}

/* convert raw data to chars */
void ApsHDSvr::hex2str(char* src, char* des, int len) {
	for (int i = 0; i < len; i++) {
		sscanf(src, "%02x", &des[i]);
		src += 2;
	}
}

bool ApsHDSvr::updateParams(ST_DATA_RESPONSE *response) {
	CParamData *pItem;
	char data[1600] = { 0 };
	double value1;
	long long value2;
	float value3;
	//unsigned int value4;
	int value5;
	bool value6;
	//char value[80];
	pItem = g_hparameter;
	// for loops: raw data->signals->sub signal

	for (unsigned int i = 0; i < response->rawDataNum; i++) {
		memset(data, 0, 1600);
		/*hex2str("00000000000000000000010200000304000f0b1000121415004142430000000000006c616e6a697169616e0000006d753132333435367869616e30307368000000000001",
		data, 68);*/
		//printf("wowo%d, %s\n", response->rawDataApp[i].length / 2, data);
		hex2str(response->rawDataApp[i].data, data,
				response->rawDataApp[i].length);
		//printf("wowo%s\n", data);
		//printf("wowo%d, %s\n", response->rawDataApp[i].length / 2, data);
		for (int j = 0; j < paramnum; j++) {
			for (int n = 0; n < g_hparameter[j].paramnum; n++) {
				if (strcmp(response->rawDataApp[i].rawDataName,
						pItem[j].signel[n].signalName) == 0) {

					value1 = 0;
					value2 = 0;
					value3 = 0;
					//value4 = 0;
					value5 = 0;
					value6 = 0;
					/*memcpy(rawdata, response->rawDataApp[i].data,
					 response->rawDataApp[i].length);*/


					g_hparameter[j].time = response->rawDataApp[i].time;
					//if(debug) printf("update sig type,,%d, type %d\n", pItem[j].sigflag, g_hparameter[j].Type);
					if (pItem[j].sigflag == 1) {
						//A429
						value1 = get429Value((unsigned char*) data,
								pItem[j].signel[n].field[0].startBit,
								pItem[j].signel[n].field[0].feildSize,
								pItem[j].signel[n].field[0].scalarPattern);
						switch (g_hparameter[j].Type) {
						case TYPE_F64B:
							//memcpy(g_hparameter[j].signel[n].data, &value, sizeof(double));
							g_hparameter[j].ParamVale.value1 = value1;
							break;
						case TYPE_S64B:
							g_hparameter[j].ParamVale.value2
									= (long long) value1;
							break;
						case TYPE_F32B:
							g_hparameter[j].ParamVale.value3 = (float) value1;
							break;
						case TYPE_U32B:
							// discard
							break;
						case TYPE_S32B:
						case TYPE_OPAQUE:
						case TYPE_ENUM:
							g_hparameter[j].ParamVale.value5 = (int) value1;
							break;
						case TYPE_BOOL:
							g_hparameter[j].ParamVale.value6 = (bool) value1;
							break;
						case TYPE_STRING:
						case TYPE_DATE:
						case TYPE_TIME:
							printf("[APS] Bad use type on A429?\n");
							//memcpy(g_hparameter[j].ParamVale.value, &data,4);
							break;
						default:
							printf("[APS] Bad use type on A429? TYPE_UNKNOW\n");
							break;
						}

					} else {
						//AFDX
						int myOffset =
								g_hparameter[j].signel[n].field[0].offset;
						int myLength =
								g_hparameter[j].signel[n].field[0].length;
						switch (g_hparameter[j].Type) {
						case TYPE_F64B:
							memcpy(&value1, data + myOffset, myLength);
							g_hparameter[j].ParamVale.value1 = value1;
							//if(debug) printf("in update,,%f\n", g_hparameter[j].ParamVale.value1);
							break;
						case TYPE_S64B:
							memcpy(&value2, data + myOffset, myLength);
							g_hparameter[j].ParamVale.value2 = value2; /*bswap_64(value2)*/;
							break;
						case TYPE_F32B:
							memcpy(&value3, data + myOffset, myLength);
							g_hparameter[j].ParamVale.value3 = value3;
							break;
						case TYPE_U32B:
							// discard
							break;
						case TYPE_S32B:
						case TYPE_OPAQUE:
						case TYPE_ENUM:
							//becareful on different type.
							memcpy(&value5, data + myOffset, myLength);
							/*printf("----%d,%d, %08X\n", myOffset, myLength, value5);
							for(int i=0; i<myLength; i++){
								printf("%02X,", data[myOffset + i]);
							}*/
							/*if(myLength == 2){
								value5 = ntohs(value5);
							} else if (myLength==4){
								value5 = ntohl(value5);
							}else {

							}*/
							//if(debug) printf("\nin update1,,%d\n", value5);
							g_hparameter[j].ParamVale.value5 = value5;
							//if(debug) printf("in update,,%d\n", value5);

							break;
						case TYPE_BOOL:
							memcpy(&value6, data + myOffset, myLength);
							g_hparameter[j].ParamVale.value6 = value6;
							break;
						case TYPE_STRING:
						case TYPE_DATE:
						case TYPE_TIME:
							memset(g_hparameter[j].ParamVale.value, 0, 80);
							memcpy(g_hparameter[j].ParamVale.value, data
									+ myOffset, myLength);
							break;
						default:
							printf("[APS] Bad use type on AFDX? TYPE_UNKNOW\n");
							break;
						}
					}
				}
			}
		}
	}

	return true;
}

bool ApsHDSvr::initServerSocket(int localPort, int* sockfd) {
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
			printf("[APS-HD] TCPServer set nonblock failed!\n");
			return false;
		}
	}

	if (true) {
		if ((ret = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR,
				&bReuseaddr, sizeof(bReuseaddr))) < 0) {
			printf("[APS-HD] TCPServer set bReuseaddr failed %d!\n", ret);
			return false;
		}
	}

	ret = bind(*sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0) {
		printf("[APS-HD] TCPServer Could not bind requested port:%u\n",
				localPort);
		return false;
	}

	ret = listen(*sockfd, MAX_REMOTE_NUMBER);
	if (ret < 0) {
		printf("[APS-HD] Could not listen requested address (error code %d)\n",
				errno);
		return false;
	}
	printf("[APS-HD] start at port: %d\n", localPort);

	return true;
}

bool ApsHDSvr::initClientSocket(char* ip, int port, int *sockfd) {
	int ret = 0;

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	*sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ret = connect(*sockfd, (struct sockaddr*) &addr, sizeof(addr));
	if (ret < 0) {
		printf("[APS-HD] Create connect to server(IP:%s,port:%d) failed\n", ip,
				port);
		return false;
	} else {
		printf("[APS-HD] Create connect to server(IP:%s,port:%d) success\n",
				ip, port);
	}

	return true;

}

bool ApsHDSvr::apsHd_start(bool isDebug) {
	int ret;
	debug = isDebug;
	//g_apsconfig = cfg;
	if (g_apsconfig == NULL) {
		printf("[APS-HD] Error: Invalid Config\n");
		return false;
	}
	ret = initServerSocket(atoi(g_apsconfig->APSvr_Port_AIDCP), &sockHDpub);
	if (!ret) {
		return false;
	}
	ret = initClientSocket(g_apsconfig->AIDCP_Addr, atoi(
			g_apsconfig->AIDCP_Port), &sockHDsvr);
	if (!ret) {
		return false;
	}

	if (true) {
		if ((ret = fcntl(sockHDsvr, F_SETFL, O_NONBLOCK)) < 0) {
			printf("[APS-HD] sockHDsvr set nonblock failed!\n");
			return false;
		}
	}

	ret = registHDSvr(sockHDsvr);
	if (!ret) {
		return false;
	}
	ret = subscribeData(sockHDsvr, &g_hparameter);
	if (!ret) {
		return false;
	}
	pthread_create(&hdHandle, NULL, ApsHDSvr::pubListen, (void*) &sockHDpub);

	pthread_create(&heartBeatHandle, NULL, ApsHDSvr::heartBeat,
	 (void*) &sockHDsvr);

	printf("[APS-HD] **********************\n");
	printf("[APS-HD] Connect to AIDCP!     \n");
	printf("[APS-HD] **********************\n");

	return true;
}
bool ApsHDSvr::apsHd_stop(bool isTimeout) {

	int ports[] = { atoi(g_apsconfig->APSvr_Port_AIDCP), 0, 0, 0, 0 };
	if(! isTimeout){
		if (isRegist) {
			unSubscribe(sockHDsvr, ports);
			unRegister(sockHDsvr);
		}
	}

	isRegist = false;
	sleep(1);
	close(sockHDpub);
	close(sockHDsvr);
	sockHDsvr = -1;
	sockHDpub = -1;
	printf("[APS-HD] **********************\n");
	printf("[APS-HD] Disconnect from AIDCP!\n");
	printf("[APS-HD] **********************\n");
	return true;
}

// static value initialize
bool ApsHDSvr::isRegist;
APS_CONFIG * ApsHDSvr::g_apsconfig;
CParamData * ApsHDSvr::g_hparameter;
int ApsHDSvr::paramnum;

bool ApsHDSvr::debug;

int ApsHDSvr::sockHDsvr;
int ApsHDSvr::sockHDpub;
pthread_t ApsHDSvr::hdHandle;
pthread_t ApsHDSvr::heartBeatHandle;

