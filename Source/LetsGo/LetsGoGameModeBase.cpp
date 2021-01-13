#include "LetsGoGameModeBase.h"
#include "Logs/DebugLoggingChannelFactory.h"

ALetsGoGameModeBase::ALetsGoGameModeBase()
{
}

ALetsGoGameModeBase::~ALetsGoGameModeBase()
{
	if(_debugLoggingChannelFactory != nullptr)
	{
		delete _debugLoggingChannelFactory;
	}
}

LoggingChannelFactory* ALetsGoGameModeBase::GetLoggingChannelFactory()
{
	if(_debugLoggingChannelFactory == nullptr)
	{
		_debugLoggingChannelFactory = new DebugLoggingChannelFactory();
	}
	return _debugLoggingChannelFactory;
}
