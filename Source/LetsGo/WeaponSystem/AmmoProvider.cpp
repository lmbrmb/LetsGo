#include "AmmoProvider.h"

AmmoProvider::AmmoProvider(const int min, const int max, const int current)
	: SharedValue<int>(min, max, current)
{
	//Do nothing
}