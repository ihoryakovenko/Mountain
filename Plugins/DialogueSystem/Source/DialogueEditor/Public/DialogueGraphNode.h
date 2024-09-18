#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfo.h"

#include "DialogueGraphNode.generated.h"

UCLASS()
class UDialogueGraphNode : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	FText GetNodeTitle(ENodeTitleType::Type Type) const override;
	FLinearColor GetNodeTitleColor() const override;
	bool CanUserDeleteNode() const override;
	void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* Context) const override;

	UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Direction, FName Name) override;
	UEdGraphPin* CreateDefaultInputPin() override;
	void CreateDefaultOutputPins() override;

	EDialogueNodeType GetDialogueNodeType() const override;

	void OnPropertiesChanged();

	void SyncPinsWithResponses();

	void InitNodeInfo(UObject* inInfo);
	void SetNodeInfo(UDialogueNodeInfoBase* inInfo) override;
	UDialogueNodeInfoBase* GetNodeInfo() const override;
	UDialogueNodeInfo* GetDialogueNodeInfo();

private:
	UPROPERTY()
	UDialogueNodeInfo* Info;
};
