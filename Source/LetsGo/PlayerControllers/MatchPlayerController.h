#pragma once

#include "GameFramework/PlayerController.h"
#include "LetsGo/Avatars/Avatar.h"

#include "MatchPlayerController.generated.h"

DECLARE_EVENT_OneParam(AMatchPlayerController, EAvatarChanged, const AAvatar* avatar);

///<summary>
/// Match player controller
///</summary>
UCLASS()
class LETSGO_API AMatchPlayerController final : public APlayerController
{
	GENERATED_BODY()

public:
	AAvatar* GetAvatar() const;

	EAvatarChanged AvatarChanged;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Custom)
	FName _mainMenuLevelName = "MainMenu";

	AAvatar* _avatar = nullptr;

	int _viewPortX = 0;

	int _viewPortY = 0;

	void OnEscape();

	void SetupInput();

	void SetMousePositionToCenter();
};
