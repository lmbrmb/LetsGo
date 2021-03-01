#pragma once

class MathUtils
{
public:
	/// <summary>
	/// Generic version of Unreal math IsNearlyZero
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="value"></param>
	/// <returns></returns>
	template <class T>
	static bool IsNearlyZero(T value, double errorTolerance = SMALL_NUMBER);
};

template <class T>
bool MathUtils::IsNearlyZero(T value, double errorTolerance)
{
	return value <= errorTolerance;
}
