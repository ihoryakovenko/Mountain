#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueEndNodeInfo.generated.h"

UENUM(BlueprintType)
enum class EDialogueNodeAction : uint8 {
    None,
    StartQuest // ActionData is the QuestId
};

UCLASS(BlueprintType)
class DIALOGUEMANAGER_API UDialogueEndNodeInfo : public UDialogueNodeInfoBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    EDialogueNodeAction Action = EDialogueNodeAction::None;

    UPROPERTY(EditAnywhere)
    FString ActionData = TEXT("");
};
