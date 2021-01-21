#include "ProtagonistPawn.h"
#include "InputConstant.h"
#include "Kismet/KismetStringLibrary.h"
#include "LetsGoGameModeBase.h"
#include "Logs/DevLogger.h"
#include "PawnControls/PawnControllerManagerFactory.h"

const float MIN_INPUT_AMOUNT = 0.1f;

const float MOVEMENT_SPEED_CM_PER_SECOND = 750.0f;

const float ROTATION_SPEED_DEGREES_PER_SECOND = 180.0f;

#define RETURN_IF_MIN_INPUT(x) \
	if(FMath::Abs(x) < MIN_INPUT_AMOUNT) \
	{return;}

AProtagonistPawn::AProtagonistPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	TheRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TheRootComponent"));
	SetRootComponent(TheRootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(TheRootComponent);
}

AProtagonistPawn::~AProtagonistPawn()
{
	if(_pawnControllerManager)
	{
		delete _pawnControllerManager;
	}
}

void AProtagonistPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!_pawnControllerManager)
	{
		const PawnControllerManagerFactory pawnControllerManagerFactory;
		_pawnControllerManager = pawnControllerManagerFactory.CreatePawnControllerManager(this);
	}

	if(!PlayerInputComponent)
	{
		DevLogger::GetLoggingChannel()->Log("No player input component", LogSeverity::Error);
		return;
	}
	
	PlayerInputComponent->RemoveActionBinding(InputConstant::ActionSwitchView, EInputEvent::IE_Pressed);
	check(_pawnControllerManager);
	_pawnControllerManager->SetInputComponent(PlayerInputComponent);
	_pawnControllerManager->CycleController();
	InputComponent->BindAction(InputConstant::ActionSwitchView, EInputEvent::IE_Pressed, this, &AProtagonistPawn::OnActionSwitchView);
}

void AProtagonistPawn::MoveForward(float amount)
{
	RETURN_IF_MIN_INPUT(amount);
	const auto forwardVector = GetRootComponent()->GetForwardVector();
	Move(forwardVector, amount);
}

void AProtagonistPawn::MoveRight(float amount)
{
	RETURN_IF_MIN_INPUT(amount);
	const auto rightVector = GetRootComponent()->GetRightVector();
	Move(rightVector, amount);
}

void AProtagonistPawn::Move(FVector direction, float amount)
{
	RETURN_IF_MIN_INPUT(amount);
	const auto deltaTime = GetWorld()->GetDeltaSeconds();
	auto const translationDelta = direction * MOVEMENT_SPEED_CM_PER_SECOND * amount * deltaTime;
	GetRootComponent()->MoveComponent(translationDelta, RootComponent->GetComponentQuat(), false, nullptr, MOVECOMP_NoFlags, ETeleportType::None);
}

void AProtagonistPawn::RotateRight(float amount)
{
	RETURN_IF_MIN_INPUT(amount);
	const auto deltaTime = GetWorld()->GetDeltaSeconds();
	auto const rotationDelta = ROTATION_SPEED_DEGREES_PER_SECOND * amount * deltaTime;
	const FRotator rotator(0, rotationDelta, 0);
	GetRootComponent()->AddLocalRotation(rotator);
}

void AProtagonistPawn::Jump()
{
	DevLogger::GetLoggingChannel()->Log("Jump");
}

void AProtagonistPawn::OnActionSwitchView()
{
	_pawnControllerManager->CycleController();
}
