#include "LetsGoGameModeBase.h"
#include "LetsGo/DependencyInjectionContainerFactory.h"

ALetsGoGameModeBase::ALetsGoGameModeBase()
{
	//Do nothing
}

ALetsGoGameModeBase::~ALetsGoGameModeBase()
{
	delete _diContainer;
}

void ALetsGoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	DependencyInjectionContainerFactory containerFactory;
	_diContainer = containerFactory.CreateContainer<ESPMode::Fast>();
}

TTypeContainer<ESPMode::Fast>* ALetsGoGameModeBase::GetDiContainer() const
{
	return _diContainer;
}
