/// DPad info
/// Required settings: RawInput plugin, "Key" = Axis5, "Inverted" = false, "Gamepad Stick" = false stick, "Offset" = 0
/// Tested with PS4 Dualshock controller
// Absolute D-pad values:
// Up = 0
// Right = 0.285
// Down = 0.571
// Left = 0.857

#pragma once

class InputUtils
{
public:
	static int GetDpadHorizontalAxis(float rawAxisValue);
	
	static int GetDpadVerticalAxis(float rawAxisValue);
private:
	InputUtils() = delete;
};

/// <summary>
/// Get D-pad horizontal axis value
/// </summary>
/// <returns>
/// -1 - left pressed
/// 0 - not pressed
/// +1 - right pressed
/// </returns>
inline int InputUtils::GetDpadHorizontalAxis(float rawAxisValue)
{
	return rawAxisValue > 0.25f && rawAxisValue < 0.35f ? 1 :
		rawAxisValue > 0.8f && rawAxisValue <= 0.9f ? -1 :
		0;
}

/// <summary>
/// Get D-pad horizontal axis value
/// </summary>
/// <returns>
/// -1 - down pressed
/// 0 - not pressed
/// +1 - up pressed
/// </returns>
inline int InputUtils::GetDpadVerticalAxis(float rawAxisValue)
{
	return rawAxisValue > 0.0f && rawAxisValue <= 0.1f ? 1 :
		rawAxisValue > 0.5f && rawAxisValue <= 0.6f ? -1 :
		0;
}