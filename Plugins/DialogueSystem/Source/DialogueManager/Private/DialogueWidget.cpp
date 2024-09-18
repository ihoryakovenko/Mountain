#include "DialogueWidget.h"

#include "DialogueWidgetViewData.h"

DEFINE_LOG_CATEGORY(DialogueWidget);

void UDialogueWidget::SetViewData(const UDialogueOptionViewData* ViewData)
{
	DialogueOptions->ClearListItems();

	DialogueText->SetText(ViewData->MainText);
	ModeSwitcher->SetActiveWidgetIndex(0);

	for (UDialogueResponseViewData* ResponseData : ViewData->Responses)
	{
		DialogueOptions->AddItem(ResponseData);
	}

	DialogueOptions->RequestRefresh();
}

void UDialogueWidget::SelectDialogueOption(int Index)
{
	
}

void UDialogueWidget::OnContinueButtonPressed()
{
	ModeSwitcher->SetActiveWidgetIndex(1);
}

void UDialogueWidget::OnDialogueStateChanged(EDialogueState NewState)
{
	switch (NewState)
	{
		case EDialogueState::Started:
			SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		case EDialogueState::Ongoing:
			break;
		case EDialogueState::Finished:
			SetVisibility(ESlateVisibility::Collapsed);
			break;
	}
}

void UDialogueWidget::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Collapsed);

	ContinueButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnContinueButtonPressed);

	// TODO: Test!
	TestAsset.LoadSynchronous();
	UDialogueSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UDialogueSubsystem>(GetOwningLocalPlayer());
	Subsystem->DialogueOptionSelected.AddUObject(this, &UDialogueWidget::SetViewData);
	Subsystem->DialogueStateChanged.AddUObject(this, &UDialogueWidget::OnDialogueStateChanged);

	Subsystem->InitiateDialogue(TestAsset.Get());
}

void UDialogueOptionEntryWidget::Init(int Index, const FText& Text)
{
    OptionIndex = Index;
	ResponseText->SetText(Text);
	IndexText->SetText(FText::AsNumber(Index + 1)); // TODO: support localization
    DialogueButton->OnClicked.AddDynamic(this, &UDialogueOptionEntryWidget::OnDialogueButtonClicked);
}

void UDialogueOptionEntryWidget::OnDialogueButtonClicked()
{
	// TODO: Test!
	UDialogueSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UDialogueSubsystem>(GetOwningLocalPlayer());
	Subsystem->SelectDialogueOption(OptionIndex);
}

void UDialogueOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UDialogueResponseViewData* ViewData = Cast<UDialogueResponseViewData>(ListItemObject))
	{
		Init(ViewData->Index, ViewData->Text);
	}
}
