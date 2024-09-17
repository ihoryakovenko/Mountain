#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueStartGraphNode.generated.h"

UCLASS()
class UDialogueStartGraphNode : public UDialogueGraphNodeBase {
    GENERATED_BODY()

public: // UDialogueStartGraphNode interface
    virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override { return FText::FromString("Start"); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Red); }
    virtual bool CanUserDeleteNode() const override { return false; }

public: // UDialogueGraphNodeBase interface
    virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection direction, FName name) override;

    virtual EDialogueNodeType GetDialogueNodeType() const override { return EDialogueNodeType::Start; }
};
