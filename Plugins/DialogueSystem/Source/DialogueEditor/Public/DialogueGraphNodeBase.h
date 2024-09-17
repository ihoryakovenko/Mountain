#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueGraphNodeBase.generated.h"

UCLASS()
class UDialogueGraphNodeBase : public UEdGraphNode {
    GENERATED_BODY()

public: // Our interface
    virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection direction, FName name) { /* Must be overidden */ return nullptr; };
    virtual UEdGraphPin* CreateDefaultInputPin() { return nullptr; }
    virtual void CreateDefaultOutputPins() { /* Nothing to do by default */ }

    virtual void InitNodeInfo(UObject* outer) { /* None by default */ }
    virtual void SetNodeInfo(UDialogueNodeInfoBase* nodeInfo) { /* None by default */ }
    virtual UDialogueNodeInfoBase* GetNodeInfo() const { /* None by default */ return nullptr; }

    virtual EDialogueNodeType GetDialogueNodeType() const { return EDialogueNodeType::Unknown; }

    virtual void OnPropertiesChanged() { /* Nothing to do by default */ }
};
