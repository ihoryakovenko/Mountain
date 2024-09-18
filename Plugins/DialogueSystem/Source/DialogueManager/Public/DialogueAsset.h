#pragma once

#include "CoreMinimal.h"
#include "DialogueGraph.h"
#include <functional>

#include "DialogueAsset.generated.h"

UCLASS(BlueprintType)
class DIALOGUEMANAGER_API UDialogueAsset : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString DialogueName;

    UPROPERTY()
    UDialogueGraph* Graph;

public:
    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener; }
    void PreSave(FObjectPreSaveContext saveContext) override;

private:
    std::function<void()> _onPreSaveListener = nullptr;
};
