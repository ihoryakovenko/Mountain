#include "DialogueGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"
#include "DialogueNodeInfo.h"

FText UDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type titalType) const { 
    UDialogueNodeInfo* nodeInfo = Cast<UDialogueNodeInfo>(_nodeInfo);
    if (nodeInfo->Title.IsEmpty()) {
        FString DialogueTextStr = nodeInfo->DialogueText.ToString();
        if (DialogueTextStr.Len() > 15) {
            DialogueTextStr = DialogueTextStr.Left(15) + TEXT("...");
        }
        return FText::FromString(DialogueTextStr);
    }
    return nodeInfo->Title;
}

void UDialogueGraphNode::GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const {
    FToolMenuSection& section = menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Dialogue Node Actions")));

    UDialogueGraphNode* node = (UDialogueGraphNode*)this;
    section.AddMenuEntry(
        TEXT("AddPinEntry"),
        FText::FromString(TEXT("Add Response")),
        FText::FromString(TEXT("Creates a new response")),
        FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueEditor.NodeAddPinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [node] () {
                node->GetDialogueNodeInfo()->DialogueOptions.Add(FText::FromString(TEXT("Response")));
                node->SyncPinsWithResponses();
                node->GetGraph()->NotifyGraphChanged();
                node->GetGraph()->Modify();
            }
        ))
    );
    
    section.AddMenuEntry(
        TEXT("DeletePinEntry"),
        FText::FromString(TEXT("Delete Response")),
        FText::FromString(TEXT("Deletes the last response")),
        FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueEditor.NodeDeletePinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [node] () {
                UEdGraphPin* pin = node->GetPinAt(node->Pins.Num() - 1);
                if (pin->Direction != EEdGraphPinDirection::EGPD_Input) {
                    UDialogueNodeInfo* info = node->GetDialogueNodeInfo();
                    info->DialogueOptions.RemoveAt(info->DialogueOptions.Num() - 1);
                    node->SyncPinsWithResponses();

                    node->GetGraph()->NotifyGraphChanged();
                    node->GetGraph()->Modify();
                }
            }
        ))
    );

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

UEdGraphPin* UDialogueGraphNode::CreateDialoguePin(EEdGraphPinDirection direction, FName name) {
    FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
    FName subcategory = TEXT("DialoguePin");

    UEdGraphPin* pin = CreatePin(
        direction,
        category,
        name
    );
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}

UEdGraphPin* UDialogueGraphNode::CreateDefaultInputPin() { 
    return CreateDialoguePin(EEdGraphPinDirection::EGPD_Input, TEXT("Display"));
}

void UDialogueGraphNode::CreateDefaultOutputPins() {
    FString defaultResponse = TEXT("Continue");
    CreateDialoguePin(EEdGraphPinDirection::EGPD_Output, FName(defaultResponse));
    GetDialogueNodeInfo()->DialogueOptions.Add(FText::FromString(defaultResponse));
}

void UDialogueGraphNode::SyncPinsWithResponses() {
    // Sync the pins on the node with the Dialogue responses
    // We're going to assume the first pin is always the
    // input pin
    UDialogueNodeInfo* nodeInfo = GetDialogueNodeInfo();
    int numGraphNodePins = Pins.Num() - 1;
    int numInfoPins = nodeInfo->DialogueOptions.Num();

    while (numGraphNodePins > numInfoPins) {
        RemovePinAt(numGraphNodePins - 1, EEdGraphPinDirection::EGPD_Output);
        numGraphNodePins--;
    }
    while (numInfoPins > numGraphNodePins) {
        CreateDialoguePin(
            EEdGraphPinDirection::EGPD_Output,
            FName(nodeInfo->DialogueOptions[numGraphNodePins].ToString())
        );
        numGraphNodePins++;
    }

    int index = 1;
    for (const FText& option : nodeInfo->DialogueOptions) {
        GetPinAt(index)->PinName = FName(option.ToString());
        index++;
    }
}
