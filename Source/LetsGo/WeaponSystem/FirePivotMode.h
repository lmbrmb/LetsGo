#pragma once

UENUM()
enum class FirePivotMode
{
	AlwaysFirst UMETA(DisplayName = "Always first"),
	Cycle UMETA(DisplayName = "Cycle"),
	AllSimultaneously UMETA(DisplayName = "All simultaneously"),
};