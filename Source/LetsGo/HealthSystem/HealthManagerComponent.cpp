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

	auto const owner = GetOwner();
	_healthComponent = owner->FindComponentByClass<UHealthComponent>();
	AssertIsNotNull(_healthComponent);

	_healthComponent->HealthChanged.AddUObject(this, &UHealthManagerComponent::OnHealthChanged);
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

bool UHealthManagerComponent::CanItemHealAboveNormal(HealthItem* healthItem) const
{
	auto const itemId = healthItem->GetId();
	return _itemsCanHealAboveNormal.Contains(itemId);
}

void UHealthManagerComponent::OnHealthChanged(UHealthComponent* healthComponent, const float delta)
{
	AssertIsTrue(_healthComponent == healthComponent);

	if (delta >= 0)
	{
		return;
	}

	if (_healthComponent->IsDead())
	{
		OnDied();
		return;
	}

	OnInjured();
}

void UHealthManagerComponent::OnDied()
{
	_healthComponent->HealthChanged.RemoveAll(this);

	auto const owner = _healthComponent->GetOwner();
	AssertIsNotNull(owner);

	if(_healthComponent->GetCurrentValue() < _overkillHealth)
	{
		BpOnOverkill();
	}

	// Disable pawn-pawn collision
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

	// UnPossess
	auto const controller = owner->GetInstigatorController();
	AssertIsNotNull(controller);

	controller->UnPossess();
}

void UHealthManagerComponent::OnInjured()
{
	auto const lastDamage = _healthComponent->GetLastDamage();
	auto const isHitted = lastDamage.GetIsHitted();

	if(!isHitted)
	{
		return;
	}

	BpOnHit(lastDamage.GetHitResult());
}
