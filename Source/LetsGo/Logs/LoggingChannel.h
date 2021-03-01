#pragma once

#include "LogSeverity.h"
#include "LetsGo/Utils/FStringUtils.h"

/// <summary>
/// [Abstract] Logging channel base class
/// </summary>
class LETSGO_API LoggingChannel
{
public:
	LoggingChannel() = default;

	virtual ~LoggingChannel() = default;

	template<class T>
	void Log(T value, LogSeverity severity = LogSeverity::Debug)
	{
		LogImplementation(FStringUtils::ToString(value), severity);
	}
	
	template<class T>
	void LogValue(const FString message, T value, LogSeverity severity = LogSeverity::Debug)
	{
		Log(message + FStringUtils::ToString(value), severity);
	}

protected:
	virtual void LogImplementation(const FString message, LogSeverity severity = LogSeverity::Debug) = 0;
};
