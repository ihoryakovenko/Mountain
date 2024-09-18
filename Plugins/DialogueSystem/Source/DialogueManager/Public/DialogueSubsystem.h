#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "DialogueSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DialogueSubsystem, Log, All);

class UDialogueAsset;
class UDialogueNode;

UENUM()
enum class EDialogueState
{
	Started,
	Ongoing,
	Finished
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueStateChanged, EDialogueState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueOptionSelected, const class UDialogueOptionViewData*);

UCLASS()
class DIALOGUEMANAGER_API UDialogueSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void InitiateDialogue(UDialogueAsset* Asset);
	void FinishDialogue();

	void SelectDialogueOption(int Index);

public:
	FOnDialogueStateChanged DialogueStateChanged;
	FOnDialogueOptionSelected DialogueOptionSelected;

private:
	UPROPERTY(Transient)
	UDialogueAsset* ActiveAsset;

	UPROPERTY(Transient)
	UDialogueNode* ActiveNode;

	int DialoguePhase = 0;
	bool CanStartDialogue = true;
};
