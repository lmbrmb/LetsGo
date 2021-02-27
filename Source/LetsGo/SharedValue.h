#pragma once

#include "Utils/AssertUtils.h"

template<class T>
class SharedValue
{
public:
	explicit SharedValue(const T min, const T max, const T current);

	T Get() const;

	T Add(const T amount);

	T Remove(const T amount);

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
	AssertIsGreaterOrEqual(min, max);
	AssertIsGreaterOrEqual(current, min);
	AssertIsLessOrEqual(current, max);
}

template <class T>
T SharedValue<T>::Get() const
{
	return _current;
}

template <class T>
T SharedValue<T>::Add(const T amount)
{
	auto delta = _max - _current;
	auto amountToAdd = FMath::Min(delta, amount);
	_current += amountToAdd;
	return amountToAdd;
}

template <class T>
T SharedValue<T>::Remove(const T amount)
{
	auto delta = _current - _min;
	auto amountToRemove = FMath::Min(delta, amount);
	_current -= amountToRemove;
	return amountToRemove;
}