#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfo.generated.h"

UCLASS(BlueprintType)
class DIALOGUEMANAGER_API UDialogueNodeInfo : public UDialogueNodeInfoBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FText Title;

    UPROPERTY(EditAnywhere)
    FText DialogueText;

    UPROPERTY(EditAnywhere)
    TArray<FText> DialogueOptions;
};
