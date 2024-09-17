#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueEndNodeInfo.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueEndGraphNode.generated.h"

UCLASS()
class UDialogueEndGraphNode : public UDialogueGraphNodeBase {
    GENERATED_BODY()

public: // UDialogueStartGraphNode interface
    virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Blue); }
    virtual bool CanUserDeleteNode() const override { return true; }
    virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const override;

public: // UDialogueGraphNodeBase interface
    virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection direction, FName name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override; 

    virtual void InitNodeInfo(UObject* outer) { _nodeInfo = NewObject<UDialogueEndNodeInfo>(outer); }
    virtual void SetNodeInfo(UDialogueNodeInfoBase* nodeInfo) override { _nodeInfo = Cast<UDialogueEndNodeInfo>(nodeInfo); }
    virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return _nodeInfo; }

    virtual EDialogueNodeType GetDialogueNodeType() const override { return EDialogueNodeType::End; }

    virtual void OnPropertiesChanged() { Modify(); }

protected:
    UPROPERTY()
    class UDialogueEndNodeInfo* _nodeInfo;
};
