#pragma once

#include "CoreMinimal.h"
#include "LogSeverity.h"
#include "LetsGo/Utils/FStringUtils.h"

/// <summary>
/// Logging channel base class
/// </summary>
class LETSGO_API LoggingChannel
{
public:
	LoggingChannel();

	virtual ~LoggingChannel();

	virtual void Log(const FString message, LogSeverity::LogSeverity severity = LogSeverity::Debug) = 0;

	template<class T>
	void LogValue(const FString message, T value, LogSeverity::LogSeverity severity = LogSeverity::Debug)
	{
		Log(message + FStringUtils::ToString(value), severity);
	}
};
