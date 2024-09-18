#include "DialogueEditor.h"

#include "Kismet2/BlueprintEditorUtils.h"

#include "DialogueAsset.h"
#include "DialogueAssetHelpers.h"
#include "DialogueGraphSchema.h"
#include "DialogueGraphNode.h"
#include "DialogueStartGraphNode.h"
#include "DialogueEndGraphNode.h"
#include "DialogueNodeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(DialogueEditorSub, Log, All);

void DialogueEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void DialogueEditor::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject)
{
	TArray<UObject*> objectsToEdit;
	objectsToEdit.Add(inObject);
	
	_workingAsset = Cast<UDialogueAsset>(inObject);
	_workingAsset->SetPreSaveListener([this] () { OnWorkingAssetPreSave(); });

	_workingGraph = FBlueprintEditorUtils::CreateNewGraph(
		_workingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UDialogueGraphSchema::StaticClass()
	);

	InitAssetEditor( 
		mode, 
		initToolkitHost, 
		TEXT("DialogueEditor"), 
		FTabManager::FLayout::NullLayout, 
		true, // createDefaultStandaloneMenu 
		true,  // createDefaultToolbar
		objectsToEdit);

	// Add our modes (just one for this example)
	AddApplicationMode(TEXT("DialogueAssetAppMode"), MakeShareable(new DialogueAssetApplicationMode(SharedThis(this))));

	// Set the mode
	SetCurrentMode(TEXT("DialogueAssetAppMode"));

	UpdateEditorGraphFromWorkingAsset();
}

void DialogueEditor::OnClose()
{
	UpdateWorkingAssetFromGraph();
	_workingAsset->SetPreSaveListener(nullptr);
	FAssetEditorToolkit::OnClose();
}

void DialogueEditor::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& event)
{
	if (_workingGraphUi != nullptr)
{
		// Get the node being modified
		UDialogueGraphNodeBase* DialogueNode = GetSelectedNode(_workingGraphUi->GetSelectedNodes());
		if (DialogueNode != nullptr)
{
			DialogueNode->OnPropertiesChanged();
		}
		_workingGraphUi->NotifyGraphChanged();
	}
}

void DialogueEditor::OnWorkingAssetPreSave()
{
	// Update our asset from the graph just before saving it
	UpdateWorkingAssetFromGraph();
}

void DialogueEditor::UpdateWorkingAssetFromGraph()
{
	if (_workingAsset == nullptr || _workingGraph == nullptr)
{
		return;
	}

	UDialogueGraph* runtimeGraph = NewObject<UDialogueGraph>(_workingAsset);
	_workingAsset->Graph = runtimeGraph;

	TArray<std::pair<FGuid, FGuid>> connections;
	TMap<FGuid, UDialoguePin*> idToPinMap;

	for (UEdGraphNode* uiNode : _workingGraph->Nodes)
{
		UDialogueNode* runtimeNode = NewObject<UDialogueNode>(runtimeGraph);
		runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);

		for (UEdGraphPin* uiPin : uiNode->Pins)
{
			UDialoguePin* runtimePin = NewObject<UDialoguePin>(runtimeNode);
			runtimePin->PinName = uiPin->PinName;
			runtimePin->PinId = uiPin->PinId;
			runtimePin->Parent = runtimeNode;

			if (uiPin->HasAnyConnections() && uiPin->Direction == EEdGraphPinDirection::EGPD_Output)
{
				std::pair<FGuid, FGuid> connection = std::make_pair(uiPin->PinId, uiPin->LinkedTo[0]->PinId);
				connections.Add(connection);
			}

			idToPinMap.Add(uiPin->PinId, runtimePin);
			if (uiPin->Direction == EEdGraphPinDirection::EGPD_Input)
{
				runtimeNode->InputPin = runtimePin;
			} else
{
				runtimeNode->OutputPins.Add(runtimePin);
			}
		}

		UDialogueGraphNodeBase* uiDialogueNode = Cast<UDialogueGraphNodeBase>(uiNode);
		runtimeNode->Info = DuplicateObject(uiDialogueNode->GetNodeInfo(), runtimeNode);
		runtimeNode->Type = uiDialogueNode->GetDialogueNodeType();

		runtimeGraph->Nodes.Add(runtimeNode);
	}

	for (std::pair<FGuid, FGuid> connection : connections)
{
		UDialoguePin* pin1 = idToPinMap[connection.first];
		UDialoguePin* pin2 = idToPinMap[connection.second];
		pin1->Connection = pin2;
	};
}

void DialogueEditor::UpdateEditorGraphFromWorkingAsset()
{
	if (_workingAsset->Graph == nullptr)
{
		UDialogueGraph* runtimeGraph = NewObject<UDialogueGraph>(_workingAsset);
		_workingGraph->GetSchema()->CreateDefaultNodesForGraph(*_workingGraph);
		return;
	}

	TArray<std::pair<FGuid, FGuid>> connections;
	TMap<FGuid, UEdGraphPin*> idToPinMap;

	for (UDialogueNode* runtimeNode : _workingAsset->Graph->Nodes)
	{
		UDialogueGraphNodeBase* newNode = nullptr;
		if (runtimeNode->Type == EDialogueNodeType::Start)
		{
			newNode = NewObject<UDialogueStartGraphNode>(_workingGraph);
		}
		else if (runtimeNode->Type == EDialogueNodeType::Dialogue)
		{
			newNode = NewObject<UDialogueGraphNode>(_workingGraph);
		}
		else if (runtimeNode->Type == EDialogueNodeType::End)
		{
			newNode = NewObject<UDialogueEndGraphNode>(_workingGraph);
		}
		else
		{
			UE_LOG(DialogueEditorSub, Error, TEXT("DialogueEditor::UpdateEditorGraphFromWorkingAsset: Unknown node type"));
			continue;
		}
		newNode->CreateNewGuid();
		newNode->NodePosX = runtimeNode->Position.X;
		newNode->NodePosY = runtimeNode->Position.Y;
		
		if (runtimeNode->Info != nullptr)
		{
			newNode->SetNodeInfo(DuplicateObject(runtimeNode->Info, newNode));
		}
		else
		{
			newNode->InitNodeInfo(newNode);
		}

		if (runtimeNode->InputPin != nullptr)
		{
			UDialoguePin* pin = runtimeNode->InputPin;
			UEdGraphPin* uiPin = newNode->CreateDialoguePin(EEdGraphPinDirection::EGPD_Input, pin->PinName);
			uiPin->PinId = pin->PinId;

			if (pin->Connection != nullptr)
			{
				connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
			}
			idToPinMap.Add(pin->PinId, uiPin);
		}

		for (UDialoguePin* pin : runtimeNode->OutputPins)
		{
			UEdGraphPin* uiPin = newNode->CreateDialoguePin(EEdGraphPinDirection::EGPD_Output, pin->PinName);
			uiPin->PinId = pin->PinId;

			if (pin->Connection != nullptr)
			{
				connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
			}
			idToPinMap.Add(pin->PinId, uiPin);
		}

		_workingGraph->AddNode(newNode, true, true);
	}

	for (std::pair<FGuid, FGuid> connection : connections)
	{
		UEdGraphPin* fromPin = idToPinMap[connection.first];
		UEdGraphPin* toPin = idToPinMap[connection.second];
		fromPin->LinkedTo.Add(toPin);
		toPin->LinkedTo.Add(fromPin);
	}
}


UDialogueGraphNodeBase* DialogueEditor::GetSelectedNode(const FGraphPanelSelectionSet& selection)
{
	for (UObject* obj : selection)
	{
		UDialogueGraphNodeBase* node = Cast<UDialogueGraphNodeBase>(obj);
		if (node != nullptr)
		{
			return node;
		}
	}

	return nullptr;
}

void DialogueEditor::SetSelectedNodeDetailView(TSharedPtr<class IDetailsView> detailsView)
{ 
	_selectedNodeDetailView = detailsView;
	_selectedNodeDetailView->OnFinishedChangingProperties().AddRaw(this, &DialogueEditor::OnNodeDetailViewPropertiesUpdated);
}

void DialogueEditor::OnGraphSelectionChanged(const FGraphPanelSelectionSet& selection)
{
	UDialogueGraphNodeBase* selectedNode = GetSelectedNode(selection);
	if (selectedNode != nullptr)
	{
		_selectedNodeDetailView->SetObject(selectedNode->GetNodeInfo());
	}
	else
	{
		_selectedNodeDetailView->SetObject(nullptr);
	}
}
