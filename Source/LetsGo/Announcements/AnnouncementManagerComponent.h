#pragma once

#include "Components/ActorComponent.h"

#include "IAnnouncementManager.h"

#include "AnnouncementManagerComponent.generated.h"

DECLARE_EVENT(UWeaponManagerComponent, EInitialized_UAnnouncementManagerComponent);

///<summary>
///Announcement manager component
///</summary>
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class LETSGO_API UAnnouncementManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UAnnouncementManagerComponent();

	~UAnnouncementManagerComponent();

	IAnnouncementManager* GetAnnouncementManager() const;

	bool IsInitialized() const;

	EInitialized_UAnnouncementManagerComponent Initialized;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchWarmUpAnnouncementDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchStartAnnouncementDuration = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _matchEndAnnouncementDuration = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _firstPlayerAnnouncementDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom, meta = (AllowPrivateAccess = "true"))
	float _playerAnnouncementDuration = 1.5f;

	IAnnouncementManager* _announcementManager = nullptr;
};
