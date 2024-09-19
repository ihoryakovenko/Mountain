#include "Ui/InGameHUDWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "DialogueSubsystem.h"

void UInGameHUDWidget::NativeConstruct()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->SetShowMouseCursor(false);
	}

	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);

	UDialogueSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UDialogueSubsystem>(GetOwningLocalPlayer());
	Subsystem->DialogueOptionSelected.AddUObject(DialogueWidget, &UDialogueWidget::SetViewData);
	Subsystem->DialogueStateChanged.AddUObject(this, &UInGameHUDWidget::OnDialogueStateChanged);
}

void UInGameHUDWidget::OnDialogueStateChanged(EDialogueState NewState)
{
	switch (NewState)
	{
		case EDialogueState::Started:
		{
			DialogueWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (APlayerController* PlayerController = GetOwningPlayer())
			{
				CachedShowMouseCursor = PlayerController->bShowMouseCursor;
				PlayerController->SetShowMouseCursor(true);
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, nullptr,
					EMouseLockMode::DoNotLock, true);
			}

			break;
		}
		case EDialogueState::Ongoing:
		{
			break;
		}
		case EDialogueState::Finished:
		{
			DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);

			if (APlayerController* PlayerController = GetOwningPlayer())
			{
				PlayerController->SetShowMouseCursor(CachedShowMouseCursor);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController);
			}

			break;
		}
	}
}
