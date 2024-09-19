#include "DialogueWidget.h"

#include "DialogueWidgetViewData.h"
#include "DialogueSubsystem.h"

DEFINE_LOG_CATEGORY(DialogueWidget);

void UDialogueWidget::NativeConstruct()
{
	DialogueOptions->SetSelectionMode(ESelectionMode::Single);

	DialogueOptions->OnItemSelectionChanged().AddUObject(this, &UDialogueWidget::OnDialogueOptionSelected);
}

void UDialogueWidget::SetViewData(const UDialogueOptionViewData* ViewData)
{
	DialogueOptions->ClearListItems();

	DialogueText->SetText(ViewData->MainText);
	ModeSwitcher->SetActiveWidgetIndex(0);

	for (UDialogueResponseViewData* ResponseData : ViewData->Responses)
	{
		DialogueOptions->AddItem(ResponseData);
	}

	SetFocus();
}

void UDialogueWidget::OnContinueButtonPressed()
{
	ModeSwitcher->SetActiveWidgetIndex(1);

	DialogueOptions->SetFocus();
	DialogueOptions->SetKeyboardFocus();
}

FReply UDialogueWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	return FReply::Handled();
}

void UDialogueWidget::OnDialogueOptionSelected(UObject* SelectedItem)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnDialogueOptionSelected"));
	//if (UDialogueOptionEntryWidget* Entry = Cast<UDialogueOptionEntryWidget>(SelectedItem))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnDialogueOptionSelected cast"));
	//}
}

FReply UDialogueWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnContinueButtonPressed();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDialogueWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Virtual_Accept)
	{
		OnContinueButtonPressed();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UDialogueOptionEntryWidget::Init(int Index, const FText& Text)
{
	// TODO: support string table
	const FText Format = FText::Format(NSLOCTEXT("Namespace", "Key", "{0}:"), FText::AsNumber(Index + 1));

	OptionIndex = Index;
	ResponseText->SetText(Text);
	IndexText->SetText(Format);
}

void UDialogueOptionEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DialogueButton->OnClicked.AddDynamic(this, &UDialogueOptionEntryWidget::OnDialogueButtonClicked);
}

void UDialogueOptionEntryWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnItemSelectionChanged"));
	if (bIsSelected)
	{
		DialogueButton->SetFocus();
		DialogueButton->SetButtonHovered(true);
	}
	else
	{
		DialogueButton->SetButtonHovered(false);
	}
}

// TODO: Expose as delegate?
void UDialogueOptionEntryWidget::OnDialogueButtonClicked()
{
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
