#include "FStringUtils.h"
#include "Kismet/KismetStringLibrary.h"

template <>
FString FStringUtils::ToString<FString>(const FString value)
{
	return value;
}

template <>
FString FStringUtils::ToString<FName>(const FName value)
{
	return value.ToString();
}

template <>
FString FStringUtils::ToString<int>(const int value)
{
	return UKismetStringLibrary::Conv_IntToString(value);
}

template <>
FString FStringUtils::ToString<float>(const float value)
{
	return UKismetStringLibrary::Conv_FloatToString(value);
}

template <>
FString FStringUtils::ToString<FVector>(const FVector value)
{
	return value.ToString();
}

template <>
FString FStringUtils::ToString<FRotator>(const FRotator value)
{
	return value.ToString();
}
