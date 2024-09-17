#pragma once

#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class DialogueEditor : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook {
public:
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject);

    class UDialogueAsset* GetWorkingAsset() { return _workingAsset; }
    class UEdGraph* GetWorkingGraph() { return _workingGraph; }

    void SetWorkingGraphUi(TSharedPtr<SGraphEditor> workingGraphUi) { _workingGraphUi = workingGraphUi; }
    void SetSelectedNodeDetailView(TSharedPtr<class IDetailsView> detailsView);
    void OnGraphSelectionChanged(const FGraphPanelSelectionSet& selection);

public:
    virtual FName GetToolkitFName() const override { return FName(TEXT("DialogueEditorApp")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("DialogueEditorApp")); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("DialogueEditorApp"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f); }
	virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override { }
	virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override { }

    virtual void OnClose() override;
    void OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& event);
    void OnWorkingAssetPreSave();

protected:
    void UpdateWorkingAssetFromGraph();
    void UpdateEditorGraphFromWorkingAsset();
    class UDialogueGraphNodeBase* GetSelectedNode(const FGraphPanelSelectionSet& selection);

private:
    UPROPERTY()
    class UDialogueAsset* _workingAsset;

    UPROPERTY()
    class UEdGraph* _workingGraph;

    TSharedPtr<SGraphEditor> _workingGraphUi;
    TSharedPtr<class IDetailsView> _selectedNodeDetailView;
};
