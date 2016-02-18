#include "APSS.h"

//=============================================================================
// @author: ljq
// @description: q
//=============================================================================
using namespace GC;


/**
 * Default implementation of cleanup method: do nothing
 */
eResult APSS::cleanup() {
	return eResult_OK;
}
//
/**
 * Method called during configure phase and
 * should be used to configure usercode.
 */
eResult APSS::extraConfigure(const UserCodeConfig& config) {

	return eResult_OK;
}
//
/**
 * Default implementation of run method: call preRun/step/postRun
 */
eResult APSS::run() {
	eResult result = UserCodeBase::run();
	return result;
}
//
/**
 * Set user code definition object
 *
 */
eResult APSS::setUCDefinition(boost::shared_ptr<UCDefInterface> ucDef) {
	eResult result = UserCodeBase::setUCDefinition(ucDef);
	return eResult_OK;
}
//
/**
 * Default implementation of start method: do nothing
 */

eResult APSS::start() {

	char filename[] = "/tmp/aps_config.xml";

	ApsXmlOp::getDataFromXMLFile(filename);
	strcpy(ApsXmlOp::g_apsconfig->APS_Name, "APS");
	ApsXmlOp::g_apsconfig->ProcessId = getpid();

	ApsHDSvr::g_apsconfig = ApsXmlOp::g_apsconfig;
	ApsHDSvr::g_hparameter = ApsXmlOp::g_hparameter;
	ApsHDSvr::paramnum = ApsXmlOp::paramnum;
	ApsParaSvr::g_hparameter = ApsXmlOp::g_hparameter;
	ApsParaSvr::g_apsconfig = ApsXmlOp::g_apsconfig;
	ApsParaSvr::paramnum = ApsXmlOp::paramnum;
	ApsParaSvr::flightModes = ApsXmlOp::flightModes;

	ApsHDSvr::apsHd_start(false);
	ApsParaSvr::aps_start(false);


	return eResult_OK;
}
//
/**
 * Method executed periodically between 
 * the preRun and postRun methods
 * You can access to your input/output variables
 * through _ucDef
 * ex:
 * _ucDef->myOutputVar = 2 * _ucDef->myInputVar;
 */
eResult APSS::step() {

	ApsParaSvr::debug = (_ucDef->isDebug == 2);
	ApsHDSvr::debug = (_ucDef->isDebug == 1);
	_ucDef->isHdRegist = ApsHDSvr::isRegist;
	// 10101
	_ucDef->isParaRun = ApsParaSvr::isRun + ApsParaSvr::pubMsgs.size() *100 +
	ApsParaSvr::eventMsgs.size() * 100000;

	/*if(ApsHDSvr::isRegist == false) {
		ApsHDSvr::apsHd_start(false);
	}

	if(ApsParaSvr::isRun == false) {
		ApsParaSvr::aps_start(false);
	}*/

	return eResult_OK;
}
//
/**
 * Default implementation of stop method: do nothing
 */
eResult APSS::stop() {
	ApsParaSvr::aps_stop();
	ApsHDSvr::apsHd_stop(false);
	return eResult_OK;
}
//
GC_USERCODE_EXPORT(DescAPSS, APSS)
