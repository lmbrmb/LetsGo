#pragma once

class MathUtils
{
public:
	static const float Pi;

	static const float TwoPi;
	
	/// <summary>
	/// Generic version of Unreal math IsNearlyZero
	/// </summary>
	template <class T>
	static bool IsNearlyZero(T value, double errorTolerance = SMALL_NUMBER);

	static FVector2D GetRandomPointOnUnitCircle()
	{
		auto const seed = FMath::FRand() * TwoPi - Pi;
		auto const x = FMath::Cos(seed);
		auto const y = FMath::Sin(seed);
		return FVector2D(x, y);
	}

private:
	MathUtils() = delete;
};

template <class T>
bool MathUtils::IsNearlyZero(T value, double errorTolerance)
{
	return value <= errorTolerance;
};
