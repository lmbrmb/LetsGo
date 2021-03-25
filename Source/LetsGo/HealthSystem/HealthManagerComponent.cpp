#include "HealthManagerComponent.h"


#include "Components/ShapeComponent.h"
#include "LetsGo/Utils/AssertUtils.h"
#include "LetsGo/WeaponSystem/WeaponManagerComponent.h"

UHealthManagerComponent::UHealthManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto const actor = GetOwner();
	_healthComponent = actor->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(_healthComponent);

	_healthComponent->Died.AddUObject(this, &UHealthManagerComponent::OnDied);
}

bool UHealthManagerComponent::TryProcessItem(Item* item)
{
	auto const healthItem = dynamic_cast<HealthItem*>(item);
	if (healthItem == nullptr)
	{
		return false;
	}

	return ProcessHealthItem(healthItem);
}

bool UHealthManagerComponent::ProcessHealthItem(HealthItem* healthItem) const
{
	if (!_healthComponent)
	{
		return false;
	}
	
	if (_healthComponent->IsDead() || _healthComponent->IsFullHealth())
	{
		return false;
	}

	auto healAmount = healthItem->GetHealAmount();

	auto const canItemHealAboveNormal = CanItemHealAboveNormal(healthItem);
	if (!canItemHealAboveNormal)
	{
		auto const normalHealthDelta = _healthComponent->GetMaxNormalHealth() - _healthComponent->GetCurrentValue();
		healAmount = FMath::Min(healAmount, normalHealthDelta);

		if (normalHealthDelta <= 0)
		{
			return false;
		}
	}

	return _healthComponent->TryHeal(healAmount);
}

bool UHealthManagerComponent::CanItemHealAboveNormal(HealthItem* healthItem)
{
	return healthItem->GetId() == "MajorHealth";
}

void UHealthManagerComponent::OnDied(const UHealthComponent*, float delta) const
{
	_healthComponent->Died.RemoveAll(this);

	// Disable pawn-pawn collision
	auto const owner = _healthComponent->GetOwner();
	AssertIsNotNull(owner);

	TArray<UShapeComponent*> shapeComponents;
	owner->GetComponents<UShapeComponent>(shapeComponents);
	for (auto shapeComponent : shapeComponents)
	{
		shapeComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}

	// Disable weapon
	auto const weaponManagerComponent = owner->FindComponentByClass<UWeaponManagerComponent>();
	AssertIsNotNull(weaponManagerComponent);

	weaponManagerComponent->HolsterWeapon();
}
