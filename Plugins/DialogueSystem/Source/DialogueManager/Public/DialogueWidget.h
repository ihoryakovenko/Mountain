#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "DialogueWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DialogueWidget, Log, All);

class UDialogueAsset;
class UDialogueNode;

UCLASS()
class DIALOGUEMANAGER_API UDialogueOptionEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(int Index, const FText& Text);

public:
	UPROPERTY(meta = (BindWidget))
	UButton* DialogueButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonText;

	int OptionIndex = -1;

private:
	void OnDialogueButtonClicked();
};

UCLASS()
class DIALOGUEMANAGER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void StartDialogue(UDialogueAsset* Asset);

private:
	void SelectDialogueOption(int Index);

public:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ModeSwitcher;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(meta = (BindWidget))
	UListView* DialogueOptions;

private:
	UPROPERTY(Transient)
	UDialogueNode* ActiveNode;
};
