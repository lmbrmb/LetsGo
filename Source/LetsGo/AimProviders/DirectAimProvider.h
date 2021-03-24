#pragma once

#include "IAimProvider.h"

class DirectAimProvider final : public IAimProvider
{
public:
	DirectAimProvider();

	// IAimProvider implementation
	virtual FVector GetTargetAimLocation() override;

	void SetTargetAimLocation(FVector aimLocation);

private:
	FVector _aimLocation;
};
