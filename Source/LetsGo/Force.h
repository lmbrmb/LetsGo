#pragma once

class Force
{
public:
	static Force* CreateInfiniteForce(FName id, FVector direction, float magnitude);

	static Force* CreateFiniteForce(FName id, float duration, FVector direction, float magnitude);
	
	FName GetId() const;

	bool IsActive() const;

	bool IsInfinite() const;
	
	FVector Take(float timeAmount);

	void Tick(float timeAmount);
	
	FVector GetVector() const;

private:
	explicit Force(FName id, FVector direction, float magnitude);

	explicit Force(FName id, float duration, FVector direction, float magnitude);
	
	FName _id;
	
	float _duration = 0;

	bool _isInfinite = false;
	
	FVector _vector = FVector::ZeroVector;
};
