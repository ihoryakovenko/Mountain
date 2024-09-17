#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfo.h"
#include "DialogueGraphNode.generated.h"

UCLASS()
class UDialogueGraphNode : public UDialogueGraphNodeBase {
    GENERATED_BODY()

public: // UEdGraphNode interface
    virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Green); }
    virtual bool CanUserDeleteNode() const override { return true; }
    virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const override;

public: // UDialogueGraphNodeBase interface
    virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection direction, FName name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override;
    virtual void CreateDefaultOutputPins() override;

    virtual EDialogueNodeType GetDialogueNodeType() const override { return EDialogueNodeType::Dialogue; }

    virtual void OnPropertiesChanged() { SyncPinsWithResponses(); }

public: // Our interface
    void SyncPinsWithResponses();

    virtual void InitNodeInfo(UObject* outer) { _nodeInfo = NewObject<UDialogueNodeInfo>(outer); }
    virtual void SetNodeInfo(UDialogueNodeInfoBase* nodeInfo) override { _nodeInfo = Cast<UDialogueNodeInfo>(nodeInfo); }
    virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return _nodeInfo; }
    UDialogueNodeInfo* GetDialogueNodeInfo() { return _nodeInfo; }

protected:
    UPROPERTY()
    class UDialogueNodeInfo* _nodeInfo;
};
