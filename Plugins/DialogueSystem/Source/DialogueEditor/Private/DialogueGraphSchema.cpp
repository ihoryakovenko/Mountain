#include "DialogueGraphSchema.h"
#include "DialogueGraphNode.h"
#include "DialogueStartGraphNode.h"
#include "DialogueEndGraphNode.h"
#include "DialogueNodeInfo.h"

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const {
    TSharedPtr<FNewNodeAction> newDialogueNodeAction(
        new FNewNodeAction(
            UDialogueGraphNode::StaticClass(),
            FText::FromString(TEXT("Nodes")),
            FText::FromString(TEXT("New Dialogue Node")),
            FText::FromString(TEXT("Makes a new Dialogue node")),
            0
        )
    );

    TSharedPtr<FNewNodeAction> newEndNodeAction(
        new FNewNodeAction(
            UDialogueEndGraphNode::StaticClass(),
            FText::FromString(TEXT("Nodes")),
            FText::FromString(TEXT("New End Node")),
            FText::FromString(TEXT("Makes a new end node")),
            0
        )
    );

    contextMenuBuilder.AddAction(newDialogueNodeAction);
    contextMenuBuilder.AddAction(newEndNodeAction);
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const {
    if (a == nullptr || b == nullptr) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
    }

    if (a->Direction == b->Direction) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

void UDialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& graph) const {
    UDialogueStartGraphNode* startNode = NewObject<UDialogueStartGraphNode>(&graph);
    startNode->CreateNewGuid();
    startNode->NodePosX = 0;
    startNode->NodePosY = 0;

    startNode->CreateDialoguePin(EEdGraphPinDirection::EGPD_Output, FName(TEXT("Start")));
    
    graph.AddNode(startNode, true, true);
    graph.Modify();
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode) {
    UDialogueGraphNodeBase* result = NewObject<UDialogueGraphNodeBase>(parentGraph, _classTemplate);
    result->CreateNewGuid();
    result->NodePosX = location.X;
    result->NodePosY = location.Y;
    result->InitNodeInfo(result);

    UEdGraphPin* inputPin = result->CreateDefaultInputPin();
    result->CreateDefaultOutputPins();

    if (fromPin != nullptr) {
        result->GetSchema()->TryCreateConnection(fromPin, inputPin);
    }

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
