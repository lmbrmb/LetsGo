#pragma once

/// <summary>
/// Math utilities
/// </summary>
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

	static bool TestProbability(const float probability)
	{
		auto const random = FMath::RandRange(0.0f, 1.0f);
		return random >= probability;
	}

	static float GetNormalizedValue(const float currentValue, const float minValue, const float maxValue)
	{
		auto const current = currentValue - minValue;
		auto const max = maxValue - minValue;
		if (FMath::IsNearlyZero(max))
			return 0;
		return current / max;
	}

private:
	MathUtils() = delete;
};

template <class T>
bool MathUtils::IsNearlyZero(T value, double errorTolerance)
{
	return value <= errorTolerance;
};
