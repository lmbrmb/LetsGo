#pragma once

#include "IPlayerSetting.h"

template<class T>
class PlayerSetting : public IPlayerSetting
{
public:
	explicit PlayerSetting(const T value) :
		_value(value)
	{
	}

	T GetValue()
	{
		return _value;
	}

	void SetValue(const T value)
	{
		_value = value;
	}

protected:
	virtual ~PlayerSetting() = default;

private:
	T _value;
};
