#include "DialogueStartGraphNode.h"

UEdGraphPin* UDialogueStartGraphNode::CreateDialoguePin(EEdGraphPinDirection direction, FName name) {
    FName category = TEXT("Outputs");
    FName subcategory = TEXT("StartPin");

    UEdGraphPin* pin = CreatePin(
        EEdGraphPinDirection::EGPD_Output,
        category,
        name
    );
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}
