#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeInfo.h"

#include "DialogueGraphNodeBase.generated.h"

UCLASS()
class UDialogueGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection direction, FName name) { return nullptr; };
	virtual UEdGraphPin* CreateDefaultInputPin() { return nullptr; }
	virtual void CreateDefaultOutputPins() { }

	virtual void InitNodeInfo(UObject* outer) { }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* nodeInfo) { }
	virtual UDialogueNodeInfoBase* GetNodeInfo() const { return nullptr; }

	virtual EDialogueNodeType GetDialogueNodeType() const { return EDialogueNodeType::Unknown; }

	virtual void OnPropertiesChanged() { }
};
