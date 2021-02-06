#pragma once

#include "LetsGo/Logs/DevLogger.h"
#include "LetsGo/Logs/LogSeverity.h"

class FactoryUtils
{
public:
	template <class TFactory, class TResult>
	static TResult CreateSingle(TArray<TFactory> factories, TFunction<TResult(TFactory)> function);
	
private:
	FactoryUtils() = delete;
};

template <class TFactory, class TResult>
TResult FactoryUtils::CreateSingle(TArray<TFactory> factories, TFunction<TResult(TFactory)> function)
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
