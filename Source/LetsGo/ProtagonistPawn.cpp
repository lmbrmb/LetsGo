#include "ProtagonistPawn.h"
#include "InputConstant.h"
#include "Kismet/KismetStringLibrary.h"
#include "LetsGoGameModeBase.h"

const float MIN_AXIS_INPUT = 0.1f;

AProtagonistPawn::AProtagonistPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	TheRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TheRootComponent"));
	SetRootComponent(TheRootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	auto meshShapeId = TEXT("StaticEVisualLoggerShapeElement::Mesh'/Engine/BasicShapes/Sphere.Sphere'");
	auto meshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(meshShapeId);
	if (meshAsset.Object != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(meshAsset.Object);
	}
	StaticMeshComponent->SetupAttachment(TheRootComponent);
}

void AProtagonistPawn::BeginPlay()
{
	Super::BeginPlay();
	GetLoggingChannel()->Log(RootComponent->GetName());
}

void AProtagonistPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(InputConstant::AxisMoveForward, this, &AProtagonistPawn::OnAxisMoveForward);
	PlayerInputComponent->BindAxis(InputConstant::AxisMoveRight, this, &AProtagonistPawn::OnAxisMoveRight);
	PlayerInputComponent->BindAction(InputConstant::ActionJump, EInputEvent::IE_Pressed, this, &AProtagonistPawn::OnActionJump);
}

LoggingChannel* AProtagonistPawn::GetLoggingChannel() const
{
	const auto gameMode = GetWorld()->GetAuthGameMode();
	const auto gameModeBase = static_cast<ALetsGoGameModeBase*>(gameMode);
	return gameModeBase
		? gameModeBase->GetLoggingChannel()
		: nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AProtagonistPawn::OnAxisMoveForward(float amount)
{
	if(FMath::Abs(amount) < MIN_AXIS_INPUT)
	{
		return;
	}
	
	GetLoggingChannel()->Log("OnMoveForward " + UKismetStringLibrary::Conv_FloatToString(amount));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AProtagonistPawn::OnAxisMoveRight(float amount)
{
	if (FMath::Abs(amount) < MIN_AXIS_INPUT)
	{
		return;
	}
	GetLoggingChannel()->Log("OnMoveRight " + UKismetStringLibrary::Conv_FloatToString(amount));
}

void AProtagonistPawn::OnActionJump()
{
	GetLoggingChannel()->Log("OnActionJump");
}
