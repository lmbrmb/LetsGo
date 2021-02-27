#pragma once

#include "FStringUtils.h"
#include "Kismet/KismetStringLibrary.h"

class BoolUtils
{
public:
	/// <summary>
	/// Is null.
	/// </summary>
	/// <typeparam name="T">Nullable type</typeparam>
	/// <param name="value">Nullable value</param>
	/// <returns>True if null, false otherwise</returns>
	template <class T>
	static bool IsNull(const T value);

	/// <summary>
	/// Is container empty.
	/// </summary>
	/// <typeparam name="T">Template class should have Num() method.</typeparam>
	/// <param name="value">TArray, TMap, TSet, etc</param>
	/// <returns>True if empty, false otherwise</returns>
	template<class T>
	static bool IsContainerEmpty(const T value);

	/// <summary>
	/// Is string empty
	/// </summary>
	/// <typeparam name="T">Type convertible to string</typeparam>
	/// <param name="value">FString, FName, etc</param>
	/// <returns>True if empty, false otherwise</returns>
	template<class T>
	static bool IsStringEmpty(const T value);
};

template <class T>
bool BoolUtils::IsNull(const T value)
{
	return value == nullptr;
}

template <class T>
bool BoolUtils::IsContainerEmpty(const T value)
{
	return value.Num() > 0;
}

template <class T>
bool BoolUtils::IsStringEmpty(const T value)
{
	auto const str = FStringUtils::ToString(value);
	return UKismetStringLibrary::IsEmpty(str);
}
