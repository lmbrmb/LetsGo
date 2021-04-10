#include "AmmoProvider.h"

AmmoProvider::AmmoProvider(const FName& ammoId, const int max, const int current)
	: _ammoId(ammoId)
{
	_sharedValue = new SharedValue<int>(0, max, current);
	_sharedValue->Changed.AddRaw(this, &AmmoProvider::OnSharedValueChanged);
}

AmmoProvider::~AmmoProvider()
{
	delete _sharedValue;
}

const FName& AmmoProvider::GetAmmoId() const
{
	return _ammoId;
}

int AmmoProvider::GetCurrent() const
{
	return _sharedValue->GetCurrent();
}

int AmmoProvider::GetMin() const
{
	return _sharedValue->GetMin();
}

int AmmoProvider::GetMax() const
{
	return _sharedValue->GetMax();
}

bool AmmoProvider::IsMin() const
{
	return _sharedValue->IsMin();
}

bool AmmoProvider::IsMax() const
{
	return _sharedValue->IsMax();
}

int AmmoProvider::Add(const int amount)
{
	return _sharedValue->Add(amount);
}

int AmmoProvider::Remove(const int amount)
{
	return _sharedValue->Remove(amount);
}

void AmmoProvider::OnSharedValueChanged(const int delta) const
{
	AmmoCountChanged.Broadcast(delta);
}
