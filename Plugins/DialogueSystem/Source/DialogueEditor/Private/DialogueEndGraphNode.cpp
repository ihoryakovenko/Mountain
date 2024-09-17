#include "DialogueEndGraphNode.h"

FText UDialogueEndGraphNode::GetNodeTitle(ENodeTitleType::Type titalType) const
{ 
    if (_nodeInfo != nullptr && _nodeInfo->Action != EDialogueNodeAction::None)
    {
        FString result = UEnum::GetDisplayValueAsText(_nodeInfo->Action).ToString();
        if (!_nodeInfo->ActionData.IsEmpty()) {
            FString actionData = _nodeInfo->ActionData;
            if (actionData.Len() > 15) {
                actionData = actionData.Left(15) + TEXT("...");
            }
            result += TEXT(" - ") + actionData;
        }
        return FText::FromString(result);
    }

    return FText::FromString(TEXT("End"));
}

void UDialogueEndGraphNode::GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const {
    FToolMenuSection& section = menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("End Node Actions")));

    UDialogueEndGraphNode* node = (UDialogueEndGraphNode*)this;
    section.AddMenuEntry(
        TEXT("DeleteEntry"),
        FText::FromString(TEXT("Delete Node")),
        FText::FromString(TEXT("Deletes the node")),
        FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueEditor.NodeDeleteNodeIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [node] () {
                node->GetGraph()->RemoveNode(node);
            }
        ))
    );
}

UEdGraphPin* UDialogueEndGraphNode::CreateDialoguePin(EEdGraphPinDirection direction, FName name) {
    FName category = TEXT("Inputs");
    FName subcategory = TEXT("EndPin");

    UEdGraphPin* pin = CreatePin(
        direction,
        category,
        name
    );
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}

UEdGraphPin* UDialogueEndGraphNode::CreateDefaultInputPin() { 
    return CreateDialoguePin(EEdGraphPinDirection::EGPD_Input, FName(TEXT("Finish")));
}
