#pragma once

#include "CoreMinimal.h"
#include "DialogueGraph.h"
#include <functional>
#include "DialogueAsset.generated.h"

UCLASS(BlueprintType)
class DIALOGUEMANAGER_API UDialogueAsset : public UObject {
    GENERATED_BODY()

public: // Properties
    UPROPERTY(EditAnywhere)
    FString DialogueName = TEXT("Enter Dialogue name here");

    UPROPERTY()
    UDialogueGraph* Graph;

public: // Our interface
    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener; }

public: // UObject interface
    virtual void PreSave(FObjectPreSaveContext saveContext) override;

private: // Members
    std::function<void()> _onPreSaveListener = nullptr;
};
