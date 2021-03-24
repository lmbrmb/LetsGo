#pragma once

class IAimProvider
{
public:
	virtual ~IAimProvider() = default;

	virtual FVector GetTargetAimLocation() = 0;
};
