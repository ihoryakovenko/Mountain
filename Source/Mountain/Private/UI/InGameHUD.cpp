#include "UI/InGameHUD.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	// TODO: support blueprint compilation failure if InGameHUDWidgetClass == nullptr
	if (InGameHUDWidgetBPClass == nullptr)
	{
		return;
	}

	InGameHUDWidget = CreateWidget<UInGameHUDWidget>(GetWorld(), InGameHUDWidgetBPClass);
	if (InGameHUDWidget != nullptr)
	{
		InGameHUDWidget->AddToViewport();
	}
}
