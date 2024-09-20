#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI/DialogueWidget.h"

#include "InGameHUDWidget.generated.h"

UCLASS()
class UInGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UDialogueWidget* DialogueWidget;
};