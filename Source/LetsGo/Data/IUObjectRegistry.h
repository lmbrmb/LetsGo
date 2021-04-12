#pragma once

// ReSharper disable once CppInconsistentNaming
class IUObjectRegistry
{
public:
	virtual void RegisterUObject(UObject* uObject) = 0;

	virtual void UnRegisterUObject(UObject* uObject) = 0;

protected:
	virtual ~IUObjectRegistry() = default;
};
