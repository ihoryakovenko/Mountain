#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "DialogueSubsystem.h"

// TODO: Test!
#include "DialogueAsset.h"

#include "DialogueWidget.generated.h"

class UDialogueOptionViewData;

DECLARE_LOG_CATEGORY_EXTERN(DialogueWidget, Log, All);

UCLASS()
class DIALOGUEMANAGER_API UDialogueOptionEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void Init(int Index, const FText& Text);

public:
	UPROPERTY(meta = (BindWidget))
	UButton* DialogueButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IndexText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResponseText;

	int OptionIndex = -1;

private:
	UFUNCTION()
	void OnDialogueButtonClicked();

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};

UCLASS()
class DIALOGUEMANAGER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetViewData(const UDialogueOptionViewData* ViewData);

protected:
	void NativeConstruct() override;

private:
	void SelectDialogueOption(int Index);

	UFUNCTION()
	void OnContinueButtonPressed();

	UFUNCTION()
	void OnDialogueStateChanged(EDialogueState NewState);

public:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ModeSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(meta = (BindWidget))
	UListView* DialogueOptions;

	// TODO: Test!
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftObjectPtr<UDialogueAsset> TestAsset;
};
