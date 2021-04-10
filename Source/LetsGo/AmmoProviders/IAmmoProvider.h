#pragma once

DECLARE_EVENT_OneParam(IAmmoProvider, FAmmoCountChanged, const int);

class IAmmoProvider
{
public:
	virtual const FName& GetAmmoId() const = 0;

	virtual int GetCurrent() const = 0;

	virtual int GetMin() const = 0;

	virtual int GetMax() const = 0;

	virtual bool IsMin() const = 0;

	virtual bool IsMax() const = 0;

	virtual int Add(const int amount) = 0;

	virtual int Remove(const int amount) = 0;

	FAmmoCountChanged AmmoCountChanged;

protected:
	virtual ~IAmmoProvider() = default;
};
