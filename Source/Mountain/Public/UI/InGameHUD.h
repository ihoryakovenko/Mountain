#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "InGameHUDWidget.h"

#include "InGameHUD.generated.h"

UCLASS()
class MOUNTAIN_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInGameHUDWidget> InGameHUDWidgetBPClass;

	UPROPERTY(Transient)
	UInGameHUDWidget* InGameHUDWidget;
};
