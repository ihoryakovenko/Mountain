#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeInfo.h"
#include "DialogueGraphNodeBase.h"

#include "DialogueEndGraphNode.generated.h"

class UToolMenu;
class UGraphNodeContextMenuContext;

UCLASS()
class UDialogueEndGraphNode : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	FText GetNodeTitle(ENodeTitleType::Type Type) const override;
	FLinearColor GetNodeTitleColor() const override;
	bool CanUserDeleteNode() const override;
	void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Direction, FName Name) override;
	UEdGraphPin* CreateDefaultInputPin() override; 

	void InitNodeInfo(UObject* inInfo);
	void SetNodeInfo(UDialogueNodeInfoBase* inInfo) override;
	UDialogueNodeInfoBase* GetNodeInfo() const override;
	EDialogueNodeType GetDialogueNodeType() const override;

	void OnPropertiesChanged();

private:
	UPROPERTY()
	UDialogueEndNodeInfo* Info;
};
