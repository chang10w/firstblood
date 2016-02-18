
#ifndef APSSUCDEF_H_
#define APSSUCDEF_H_

#include <modules/uc/UCDefInterface.h>
#include <modules/uc/UserCodeBase.h>
#include <Vs.h>
#include <log/Log.h>

class APSSUCDef: public GC::UCDefInterface
{
private:
	GC::UserCodeBase *_uc;
public:
	/**
		INPUTS
	**/
	std::auto_ptr<GC::IntegerReader>startCmdReader;
	long startCmd;
	long long startCmdTS;
	std::auto_ptr<GC::IntegerReader>isDebugReader;
	long isDebug;
	long long isDebugTS;
	/**
		OUTPUTS
	**/
	std::auto_ptr<GC::IntegerWriter>isHdRegistWriter;
	long isHdRegist;
	std::auto_ptr<GC::IntegerWriter>isParaRunWriter;
	long isParaRun;
public:
	APSSUCDef(GC::UserCodeBase *uc) : _uc(uc)
	{
	}

	std::string getName()
	{
		return "APSS";
	}

	/// @return UC log level
	LOG::Logger::LogLevel getLogLevel() const {
		return _uc->getLogLevel();
	}

	eResult initialize()
	{
		/**
			Init inputs variables
		**/
		VARIABLE_CONTROLLER()->importVariable(_uc->local("startCmd"),GC::VS::INTEGER);
		startCmdReader = std::auto_ptr<GC::IntegerReader>(
			VARIABLE_CONTROLLER()->createIntegerReader(_uc->local("startCmd")));
		if(startCmdReader.get() == 0) return eResult_UnkErr;
		VARIABLE_CONTROLLER()->importVariable(_uc->local("isDebug"),GC::VS::INTEGER);
		isDebugReader = std::auto_ptr<GC::IntegerReader>(
			VARIABLE_CONTROLLER()->createIntegerReader(_uc->local("isDebug")));
		if(isDebugReader.get() == 0) return eResult_UnkErr;

		/**
			Init outputs variables
		**/
		VARIABLE_CONTROLLER()->importVariable(_uc->local("isHdRegist"),GC::VS::INTEGER);
		isHdRegistWriter = std::auto_ptr<GC::IntegerWriter>(
			VARIABLE_CONTROLLER()->createIntegerWriter(_uc->local("isHdRegist")));
		if(isHdRegistWriter.get() == 0) return eResult_UnkErr;
		VARIABLE_CONTROLLER()->importVariable(_uc->local("isParaRun"),GC::VS::INTEGER);
		isParaRunWriter = std::auto_ptr<GC::IntegerWriter>(
			VARIABLE_CONTROLLER()->createIntegerWriter(_uc->local("isParaRun")));
		if(isParaRunWriter.get() == 0) return eResult_UnkErr;
		return eResult_OK;
	}
	eResult preRun()
	{
		int ReturnValue;
		ReturnValue = startCmdReader->get(startCmd, startCmdTS);
		if( ReturnValue != GC::VS::Ok)
		{
			gcLogModuleDebug("Can't read variable \"startCmd\", return error code: %d",ReturnValue );
		}
		ReturnValue = isDebugReader->get(isDebug, isDebugTS);
		if( ReturnValue != GC::VS::Ok)
		{
			gcLogModuleDebug("Can't read variable \"isDebug\", return error code: %d",ReturnValue );
		}
		return eResult_OK;
	}
	eResult postRun()
	{
		int ReturnValue;
		ReturnValue = isHdRegistWriter->set(isHdRegist);
		if( ReturnValue != GC::VS::Ok)
		{
			gcLogModuleDebug("Can't write variable \"isHdRegist\", return error code: %d",ReturnValue );
		}
		ReturnValue = isParaRunWriter->set(isParaRun);
		if( ReturnValue != GC::VS::Ok)
		{
			gcLogModuleDebug("Can't write variable \"isParaRun\", return error code: %d",ReturnValue );
		}
		return eResult_OK;
	}
};

#endif /* APSSUCDEF_H_ */
