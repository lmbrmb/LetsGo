#pragma once

#include "IAmmoProvider.h"

class InfiniteAmmoProvider final : public IAmmoProvider
{
public:
	explicit InfiniteAmmoProvider(const FName& ammoId);

	virtual const FName& GetAmmoId() const override;

	virtual int GetCurrent() const override;

	virtual int GetMin() const override;

	virtual int GetMax() const override;

	virtual bool IsMin() const override;

	virtual bool IsMax() const override;

	virtual int Add(const int amount) override;

	virtual int Remove(const int amount) override;

private:
	FName _ammoId;
};
