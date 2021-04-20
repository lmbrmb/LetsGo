#pragma once

struct CurveAnimationData
{
	TArray<UCurveFloat*> UpCurves;

	TArray<UCurveFloat*> RightCurves;

	TArray<UCurveFloat*> ForwardCurves;

	float Time = 0;

	bool Loop = false;

	bool IsEnabled = false;

	float CurveMagnitudeMultiplier = 1;

	float CurveTimeMultiplier = 1;
};
