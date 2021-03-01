#pragma once

#include "LetsGo/Logs/DevLogger.h"

class FactoryUtils
{
public:
	template <class TResult, class TFactory, class TFactoryArray>
	static TResult CreateSingle(TFactoryArray factories, TFunction<TResult(TFactory)> function);

	template <class TResult, class TFactory, class TFactoryArray>
	static TResult GetSingle(TFactoryArray factories, TFunction<TResult(TFactory)> function);

private:
	FactoryUtils() = delete;
};

template <class TResult, class TFactory, class TFactoryArray>
TResult FactoryUtils::CreateSingle(TFactoryArray factories, TFunction<TResult(TFactory)> function)
{
	TArray<TResult> createdItems;
	for (auto const factory : factories)
	{
		auto const result = function(factory);
		if (result)
		{
			createdItems.Add(result);
		}
	}

	auto const createdItemsCount = createdItems.Num();
	if (createdItemsCount == 0)
	{
		DevLogger::GetLoggingChannel()->Log("No items created", LogSeverity::Error);
		return nullptr;
	}

	if (createdItemsCount > 1)
	{
		DevLogger::GetLoggingChannel()->LogValue("More than one item created", createdItemsCount, LogSeverity::Error);
		createdItems.Empty();
		return nullptr;
	}
	
	return createdItems[0];
}

template <class TResult, class TFactory, class TFactoryArray>
TResult FactoryUtils::GetSingle(TFactoryArray factories, TFunction<TResult(TFactory)> function)
{
	TArray<TResult> items;
	for (auto const factory : factories)
	{
		auto const result = function(factory);
		if (result)
		{
			items.Add(result);
		}
	}

	auto const itemsCount = items.Num();
	if (itemsCount == 0)
	{
		DevLogger::GetLoggingChannel()->Log("No items created", LogSeverity::Error);
		return nullptr;
	}

	if (itemsCount > 1)
	{
		DevLogger::GetLoggingChannel()->LogValue("Got more than one item", itemsCount, LogSeverity::Error);
		return nullptr;
	}

	return items[0];
}
