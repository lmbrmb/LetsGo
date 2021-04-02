#pragma once

#include "LetsGo/Utils/FStringUtils.h"

/// <summary>
/// Generic entity id. Immutable
/// </summary>
/// <typeparam name="T">Id value type</typeparam>
template<class T>
struct EntityId
{
protected:
	virtual ~EntityId() = default;

public:
	explicit EntityId(const T id);
	
	bool operator==(const EntityId& other) const
	{
		return _id == other.GetId();
	}

	bool operator!=(const EntityId& other) const
	{
		return _id != other.GetId();
	}

	FString ToString() const;
	
	T GetId() const;

	virtual bool IsValid() const = 0;

private:
	T _id;
};

template <class T>
EntityId<T>::EntityId(const T id) :
	_id(id)
{
}

template <class T>
FString EntityId<T>::ToString() const
{
	return FStringUtils::ToString(_id);
}

template <class T>
T EntityId<T>::GetId() const
{
	return _id;
}
