#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "BoolUtils.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "LetsGo/Logs/DevLogger.h"

#define ASSERTION_FAILED __FUNCTION__ + FString(" | Line: ") + FStringUtils::ToString(__LINE__) + FString(" | Assertion failed | ")

#define AssertIsNotNull(value) \
if (BoolUtils::IsNull(value)) \
{\
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is null.", LogSeverity::Error); \
	return; \
};

#define AssertStringIsNotEmpty(value) \
if (BoolUtils::IsStringEmpty(value)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is empty", LogSeverity::Error); \
	return; \
};

#define AssertIsEqual(leftHandValue, rightHandValue) \
if (!BoolUtils::IsEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Values are not equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return; \
};

#define AssertIsGreaterOrEqual(leftHandValue, rightHandValue) \
if (!BoolUtils::IsGreaterOrEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not greater or equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return; \
};

#define AssertIsLessOrEqual(leftHandValue, rightHandValue) \
if (!BoolUtils::IsLessOrEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not less or equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return; \
};
