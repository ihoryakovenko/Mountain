#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"

#include "DialogueStartGraphNode.generated.h"

UCLASS()
class UDialogueStartGraphNode : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	FText GetNodeTitle(ENodeTitleType::Type Type) const override;
	FLinearColor GetNodeTitleColor() const override;
	bool CanUserDeleteNode() const override { return false; }

	UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Direction, FName Name) override;
	EDialogueNodeType GetDialogueNodeType() const override;
};
