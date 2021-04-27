#pragma once

#include "Math/UnrealMathUtility.h"

class FVectorUtils
{
public:
	static float GetUnsignedAngleDegrees(const FVector& v1, const FVector& v2)
	{
		auto const dot = FVector::DotProduct(v1, v2);
		return GetUnsignedAngleDegrees(dot);
	}

	static float GetUnsignedAngleDegrees(const float dot)
	{
		auto const radians = FMath::Acos(dot);
		auto const degrees = FMath::RadiansToDegrees(radians);
		return degrees;
	}
	
	static float GetSignOfAngle(const FVector& v1, const FVector& v2, const FVector& planeNormal = FVector::UpVector)
	{
		auto const cross = FVector::CrossProduct(v1, v2);
		auto const dot = FVector::DotProduct(planeNormal, cross);
		auto const sign = FMath::Sign(dot);
		return sign;
	}
	
	static float GetSignedAngleDegrees(const FVector& v1, const FVector& v2, const FVector& planeNormal = FVector::UpVector)
	{
		return GetSignOfAngle(v1, v2, planeNormal) * GetUnsignedAngleDegrees(v1, v2);
	}

	/// <summary>
	/// For testing other normalization functions.
	/// Reminder: FVector::Normalize() can lead to weird results - use FVector::GetSafeNormal instead.
	/// </summary>
	static FVector NormalizeSlow(const FVector& v)
	{
		auto const size = v.Size();
		return v / size;
	}

	static FVector2D ToFVector2D(const FVector& v)
	{
		return FVector2D(v.X, v.Y);
	}

private:
	FVectorUtils() = delete;
};