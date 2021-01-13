#pragma once

/// <summary>
/// Logging channel base class
/// </summary>
class LETSGO_API LoggingChannel
{
public:
	LoggingChannel();

	virtual ~LoggingChannel();

	virtual void Log(FString str) = 0;
};
