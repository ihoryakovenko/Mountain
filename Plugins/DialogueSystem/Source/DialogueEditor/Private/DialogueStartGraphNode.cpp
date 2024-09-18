#include "DialogueStartGraphNode.h"

FText UDialogueStartGraphNode::GetNodeTitle(ENodeTitleType::Type Type) const
{
	return FText::FromString("Start");
}

FLinearColor UDialogueStartGraphNode::GetNodeTitleColor() const
{
	return FLinearColor(FColor::Red);
}

UEdGraphPin* UDialogueStartGraphNode::CreateDialoguePin(EEdGraphPinDirection Direction, FName Name)
{
	const FName category = TEXT("Outputs");
	const FName subcategory = TEXT("StartPin");

	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, category, Name);
	Pin->PinType.PinSubCategory = subcategory;

	return Pin;
}

EDialogueNodeType UDialogueStartGraphNode::GetDialogueNodeType() const
{
	return EDialogueNodeType::Start;
}
