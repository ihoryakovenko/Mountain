#include "DialogueEndGraphNode.h"

FText UDialogueEndGraphNode::GetNodeTitle(ENodeTitleType::Type Type) const
{ 
	if (Info != nullptr && Info->Action != EDialogueNodeAction::None)
	{
		FString Result = UEnum::GetDisplayValueAsText(Info->Action).ToString();
		if (!Info->ActionData.IsEmpty())
		{
			FString ActionData = Info->ActionData;
			if (ActionData.Len() > 15)
			{
				ActionData = ActionData.Left(15) + TEXT("...");
			}

			Result += TEXT(" - ") + ActionData;
		}

		return FText::FromString(Result);
	}

	return FText::FromString(TEXT("End"));
}

FLinearColor UDialogueEndGraphNode::GetNodeTitleColor() const
{
	return FLinearColor(FColor::Blue);
}

bool UDialogueEndGraphNode::CanUserDeleteNode() const
{
	return true;
}

void UDialogueEndGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("End Node Actions")));
	UDialogueEndGraphNode* Node = const_cast<UDialogueEndGraphNode*>(this);

	Section.AddMenuEntry( TEXT("DeleteEntry"),FText::FromString(TEXT("Delete Node")), FText::FromString(TEXT("Deletes the node")),
		FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueEditor.NodeDeleteNodeIcon")), FUIAction(FExecuteAction::CreateLambda(
			[Node] ()
			{
				Node->GetGraph()->RemoveNode(Node);
			})));
}

UEdGraphPin* UDialogueEndGraphNode::CreateDialoguePin(EEdGraphPinDirection Direction, FName Name)
{
	const FName Category = TEXT("Inputs");
	const FName Subcategory = TEXT("EndPin");

	UEdGraphPin* Pin = CreatePin(Direction, Category, Name);
	Pin->PinType.PinSubCategory = Subcategory;
	return Pin;
}

UEdGraphPin* UDialogueEndGraphNode::CreateDefaultInputPin()
{ 
	return CreateDialoguePin(EEdGraphPinDirection::EGPD_Input, FName(TEXT("Finish")));
}

void UDialogueEndGraphNode::InitNodeInfo(UObject* inInfo)
{
	Info = NewObject<UDialogueEndNodeInfo>(inInfo);
}

void UDialogueEndGraphNode::SetNodeInfo(UDialogueNodeInfoBase* inInfo)
{
	Info = Cast<UDialogueEndNodeInfo>(inInfo);
}

UDialogueNodeInfoBase* UDialogueEndGraphNode::GetNodeInfo() const
{
	return Info;
}

EDialogueNodeType UDialogueEndGraphNode::GetDialogueNodeType() const
{
	return EDialogueNodeType::End;
}

void UDialogueEndGraphNode::OnPropertiesChanged()
{
	Modify();
}
