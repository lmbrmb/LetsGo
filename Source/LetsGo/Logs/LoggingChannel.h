#pragma once

#include "LogSeverity.h"

/// <summary>
/// Logging channel base class
/// </summary>
class LETSGO_API LoggingChannel
{
public:
	LoggingChannel();

	virtual ~LoggingChannel();

	virtual void Log(const FString str, LogSeverity::LogSeverity severity = LogSeverity::Debug) = 0;
};
