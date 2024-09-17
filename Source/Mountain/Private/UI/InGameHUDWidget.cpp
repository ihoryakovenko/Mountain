#include "Ui/InGameHUDWidget.h"

#include "DialogueSubsystem.h"

void UInGameHUDWidget::NativeConstruct()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->SetShowMouseCursor(false);
	}

	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
}
