#pragma once

#include "IAmmoProvider.h"
#include "LetsGo/Data/SharedValue.h"

class AmmoProvider final : public IAmmoProvider
{
public:
	AmmoProvider(const FName& ammoId, const int max, const int current);

	virtual ~AmmoProvider() override;

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

	SharedValue<int>* _sharedValue;

	void OnSharedValueChanged(int delta) const;
};
