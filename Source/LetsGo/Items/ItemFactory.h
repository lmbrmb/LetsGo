#pragma once

#include "IItemFactory.h"

////<summary>
/// [Abstract] Item factory
///</summary>
template<class T>
class LETSGO_API ItemFactory : public IItemFactory
{
public:
	ItemFactory() = default;
	
	virtual ~ItemFactory() = default;
	
	virtual T* Get(const FName itemId) override;

protected:
	virtual T* CreateItem(const FName itemId) = 0;

private:
	TMap<FName, T*> _items;
};

template <class T>
T* ItemFactory<T>::Get(const FName itemId)
{
	if (_items.Contains(itemId))
	{
		return _items[itemId];
	}

	auto const item = CreateItem(itemId);

	if(item == nullptr)
	{
		return nullptr;
	}

	_items.Add(itemId, item);
	return item;
}
