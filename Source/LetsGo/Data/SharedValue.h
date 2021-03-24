#pragma once

#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/Utils/MathUtils.h"

/// <summary>
/// Generic shared value. Notifies if changed. Mutable
/// </summary>
/// <typeparam name="T">Value type</typeparam>
template<class T>
class SharedValue
{
public:
	explicit SharedValue(const T min, const T max, const T current);

	T GetCurrent() const;

	T GetMin() const;

	T GetMax() const;

	bool IsMin() const;

	bool IsMax() const;
	
	T Add(const T amount);

	T Remove(const T amount);

	DECLARE_EVENT_OneParam(SharedValue, FChanged, T);
	FChanged Changed;
	
	DECLARE_EVENT_OneParam(SharedValue, FAdded, T);
	FAdded Added;

	DECLARE_EVENT_OneParam(SharedValue, FRemoved, T);
	FRemoved Removed;

private:
	T _min;

	T _max;
	
	T _current;
};

template <class T>
SharedValue<T>::SharedValue(const T min, const T max, const T current) :
	_min(min),
	_max(max),
	_current(current)
{
	AssertIsLessOrEqual(min, max);
	AssertIsLessOrEqual(current, max);
	AssertIsGreaterOrEqual(current, min);
}

template <class T>
T SharedValue<T>::GetCurrent() const
{
	return _current;
}

template <class T>
T SharedValue<T>::GetMin() const
{
	return _min;
}

template <class T>
T SharedValue<T>::GetMax() const
{
	return _max;
}

template <class T>
bool SharedValue<T>::IsMin() const
{
	return MathUtils::IsNearlyZero(_current - _min);
}

template <class T>
bool SharedValue<T>::IsMax() const
{
	return MathUtils::IsNearlyZero(_max - _current);
}

template <class T>
T SharedValue<T>::Add(const T amount)
{
	auto delta = _max - _current;
	auto amountToAdd = FMath::Min(delta, amount);
	
	if (MathUtils::IsNearlyZero(amountToAdd))
	{
		return 0;
	}
	
	_current += amountToAdd;
	Added.Broadcast(amountToAdd);
	Changed.Broadcast(_current);
	
	return amountToAdd;
}

template <class T>
T SharedValue<T>::Remove(const T amount)
{
	auto delta = _current - _min;
	auto amountToRemove = FMath::Min(delta, amount);

	if (MathUtils::IsNearlyZero(amountToRemove))
	{
		return 0;
	}
	
	_current -= amountToRemove;
	Removed.Broadcast(amountToRemove);
	Changed.Broadcast(_current);
	
	return amountToRemove;
}