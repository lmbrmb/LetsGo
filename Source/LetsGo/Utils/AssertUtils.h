#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "BoolUtils.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "LetsGo/Logs/DevLogger.h"

#define ASSERTION_FAILED __FUNCTION__ + FString(" | Line: ") + FStringUtils::ToString(__LINE__) + FString(" | Assertion failed | ")

#define AssertIsNotNull(value, ...) \
if (BoolUtils::IsNull(value)) \
{\
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is null.", LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertStringIsNotEmpty(value, ...) \
if (BoolUtils::IsStringEmpty(value)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is empty", LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertNameIsAssigned(value, ...) \
if (value.IsNone()) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Name is not assigned", LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsEqual(leftHandValue, rightHandValue, ...) \
if (!BoolUtils::IsEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Values are not equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsNotEqual(leftHandValue, rightHandValue, ...) \
if (BoolUtils::IsEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Values are equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsGreater(leftHandValue, rightHandValue, ...) \
if (!BoolUtils::IsGreater(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not greater", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsGreaterOrEqual(leftHandValue, rightHandValue, ...) \
if (!BoolUtils::IsGreaterOrEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not greater or equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsLess(leftHandValue, rightHandValue, ...) \
if (!BoolUtils::IsLess(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not less", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsLessOrEqual(leftHandValue, rightHandValue, ...) \
if (!BoolUtils::IsLessOrEqual(leftHandValue, rightHandValue)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not less or equal", LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Left hand value: ", leftHandValue, LogSeverity::Error); \
	DevLogger::GetLoggingChannel()->LogValue("Right hand value: ", rightHandValue, LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsTrue(value, ...) \
if (!BoolUtils::IsTrue(value)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not true", LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertIsFalse(value, ...) \
if (!BoolUtils::IsFalse(value)) \
{ \
	DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Value is not false", LogSeverity::Error); \
	return __VA_ARGS__; \
};

#define AssertDefaultImplementationIsOverriden(...) \
DevLogger::GetLoggingChannel()->Log(ASSERTION_FAILED + "Default implementation is not overriden or base class implementation is called in derived class", LogSeverity::Error); \
return __VA_ARGS__; \

#define AssertIndexIsInArrayBounds(index, arr, ...) \
if (!BoolUtils::IsGreaterOrEqual(index, 0) || !BoolUtils::IsLessOrEqual(index, _weaponPivots.Num() - 1)) \
{ \
	DevLogger::GetLoggingChannel()->LogValue(ASSERTION_FAILED + "Index is out of bounds of array. Index:", index, LogSeverity::Error); \
	return __VA_ARGS__; \
};