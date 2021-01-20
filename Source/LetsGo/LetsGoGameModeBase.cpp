#include "LetsGoGameModeBase.h"
#include "Logs/DebugLoggingChannelFactory.h"

ALetsGoGameModeBase::ALetsGoGameModeBase()
{
	_loggingChannelFactory = new DebugLoggingChannelFactory();
	_loggingChannel = _loggingChannelFactory->CreateLoggingChannel();
}

ALetsGoGameModeBase::~ALetsGoGameModeBase()
{
	delete _loggingChannelFactory;
	delete _loggingChannel;
}

LoggingChannel* ALetsGoGameModeBase::GetLoggingChannel() const
{
	return _loggingChannel;
}
