
#ifndef MAINCODE_H_
#define MAINCODE_H_
#include <modules/uc/UserCodeBase.h>
#include "Usercodes/APSSUCDef.h"
#include "ApsXmlOp.h"
#include "ApsHDSvr.h"
#include "ApsParaSvr.h"

/**
 * User code Description implementation class
 */
class DescAPSS : public GC::UserCodeDescription
{
public:
	void set(void)
	{
		declareDescription("q","ljq","xx");
		return;
	}
};

/*
 * User code implementation class
 * 
 * Two methods must always be implemented in custom codes:
 * - step(), this method is called for each period. When RTC call run() method,
 * automatically UserCodeBase call preRun() method from GC::UCDefInterface,
 * then step() and finally postRun() method also from GC::UCDefInterface. preRun()
 * and postRun() methods are used to retrieve locally variables values and to store
 * them at the end.
 * - extraConfigure(),
 */
class APSS : public GC::UserCodeBase
{

private:

	/**
	 * User code definition shared object
	 */
	boost::shared_ptr<APSSUCDef> _ucDef;

public:
	/**
	 * Constructor initializing ucdef 
	 * Using User code Description implementation
	 */
	APSS()
	:_ucDef(new APSSUCDef(this)) 
	{
		setUCDefinition(_ucDef);
	}

	/**
	 * Default implementation of run method: call preRun/step/postRun
	 */
	eResult run(void);

	/**
	 * Default implementation of start method: do nothing
	 */
	eResult start(void);
	/**
	 * Default implementation of stop method: do nothing
	 */
	eResult stop(void);
	/**
	 * Default implementation of cleanup method: do nothing
	 */
	eResult cleanup(void);

	/**
	 * Set user code definition object
	 *
	 */
	eResult setUCDefinition(boost::shared_ptr<GC::UCDefInterface> ucDef);
protected:
	/**
	 * Method called during configure phase and
	 * should be used to confiure usercode.
	 */
	eResult extraConfigure(const GC::UserCodeConfig& config);
	/**
	 * Method implemented by UserCode. It will be executed
	 * periodically between the preRun and postRun methods
	 * of the UserCodeDefinition given during constructor
	 * This default implementation do nothing
	 */
	eResult step();

};

#endif //MAINCODE_H_
