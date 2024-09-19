#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "DialogueWidget.h"

#include "InGameHUDWidget.generated.h"

UCLASS()
class UInGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	void NativeConstruct() override;

	UFUNCTION()
	void OnDialogueStateChanged(EDialogueState NewState);

public:
	UPROPERTY(meta = (BindWidget))
	UDialogueWidget* DialogueWidget;

private:
	bool CachedShowMouseCursor;
};