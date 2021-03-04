#pragma once

/// <summary>
/// [Interface] Force contract
/// <summary>
class IForce
{
public:
	IForce(){};
	
	virtual ~IForce() = default;
	
	virtual FName GetId() const = 0;

	virtual FVector GetVector(const float deltaTime) = 0;
};