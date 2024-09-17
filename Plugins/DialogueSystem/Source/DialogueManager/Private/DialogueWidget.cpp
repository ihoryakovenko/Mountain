#include "DialogueWidget.h"

#include "DialogueGraph.h"
#include "DialogueAsset.h"
#include "DialogueNodeType.h"

DEFINE_LOG_CATEGORY(DialogueWidget);

void UDialogueWidget::StartDialogue(UDialogueAsset* Asset)
{
	UDialogueGraph* Graph = Asset->Graph;
	if (Graph->Nodes.Num() < 0 && Graph->Nodes[0]->Type != EDialogueNodeType::Start)
	{
		UE_LOG(DialogueWidget, Error, TEXT("Invalid asset or start node %s"), *Asset->GetName());
		return;
	}

	ActiveNode = Graph->Nodes[0];
}

void UDialogueWidget::SelectDialogueOption(int Index)
{
	check(Index > 0 && ActiveNode != nullptr);

	if (Index >= ActiveNode->OutputPins.Num())
	{
		UE_LOG(DialogueWidget, Error, TEXT("Invalid dialogue option %d"), Index);
		return;
	}

    UDialoguePin* OutputPin = ActiveNode->OutputPins[Index];
    if (OutputPin->Connection != nullptr)
    {
        ActiveNode = OutputPin->Connection->Parent;
    }
    else
    {
        ActiveNode = nullptr;
    }

    if (ActiveNode != nullptr && ActiveNode->Type == EDialogueNodeType::Dialogue)
    {
        UDialogueNodeInfo* Info = Cast<UDialogueNodeInfo>(ActiveNode->Info);
        Info->DialogueText;

        DialogueOptions->ClearListItems();

        for (int OptionIndex = 0; OptionIndex < Info->DialogueOptions.Num(); ++OptionIndex)
        {
            // DOTO: Check the performance impact of using CreateWidget
            if (UDialogueOptionEntryWidget* Entry = CreateWidget<UDialogueOptionEntryWidget>(this, UDialogueOptionEntryWidget::StaticClass()))
            {
                Entry->Init(OptionIndex, Info->DialogueOptions[OptionIndex]);
                DialogueOptions->AddItem(Entry);
            }

            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("int OptionIndex = 0; OptionIndex < Info->DialogueOptions.Num(); ++OptionIndex"));
        }
    }
    else if (ActiveNode == nullptr || ActiveNode->Type == EDialogueNodeType::End)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ActiveNode == nullptr || ActiveNode->Type == EDialogueNodeType::End"));
    }
}

void UDialogueOptionEntryWidget::Init(int Index, const FText& Text)
{
    OptionIndex = Index;
    ButtonText->SetText(Text);
    DialogueButton->OnClicked.AddDynamic(this, &UDialogueOptionEntryWidget::OnDialogueButtonClicked);
}

void UDialogueOptionEntryWidget::OnDialogueButtonClicked()
{

}
