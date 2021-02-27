#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "BoolUtils.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "LetsGo/Logs/DevLogger.h"

const FString ASSERTION_FAILED = "Assertion failed. ";

#define AssertIsNotNull(value) \
if (BoolUtils::IsNull(value)) \
{\
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is null"); \
	return; \
};

#define AssertStringIsNotEmpty(value) \
if (BoolUtils::IsStringEmpty(value)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is empty"); \
	return; \
};
