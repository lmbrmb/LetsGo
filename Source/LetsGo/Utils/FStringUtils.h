#pragma once

#include "Kismet/KismetStringLibrary.h"

/// <summary>
/// ToString conversion
/// </summary>
class FStringUtils
{
public:
	template <class T>
	static FString ToString(const T value);

private:
	FStringUtils() = delete;
};

template <class T>
inline FString FStringUtils::ToString(const T value)
{
	return FString("Default implementation");
};

template <>
inline FString FStringUtils::ToString<FString>(const FString value)
{
	return value;
};

template <>
inline FString FStringUtils::ToString<FName>(const FName value)
{
	return value.ToString();
};

template <>
inline FString FStringUtils::ToString<bool>(const bool value)
{
	return UKismetStringLibrary::Conv_BoolToString(value);
}

template <>
inline FString FStringUtils::ToString<short>(const short value)
{
	return UKismetStringLibrary::Conv_IntToString(value);
}

template <>
inline FString FStringUtils::ToString<int>(const int value)
{
	return UKismetStringLibrary::Conv_IntToString(value);
}

template <>
inline FString FStringUtils::ToString<float>(const float value)
{
	return UKismetStringLibrary::Conv_FloatToString(value);
}

template <>
inline FString FStringUtils::ToString<double>(const double value)
{
	return FString::Printf(TEXT("%f"), value);
}

template <>
inline FString FStringUtils::ToString<FVector>(const FVector value)
{
	return value.ToString();
}

template <>
inline FString FStringUtils::ToString<FRotator>(const FRotator value)
{
	return value.ToString();
}