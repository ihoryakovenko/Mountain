#include "DialogueAssetHelpers.h"

#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "IDetailsView.h"
#include "GraphEditor.h"
#include "Kismet2/KismetEditorUtilities.h"

#include "DialogueGraphSchema.h"
#include "DialogueGraphNode.h"
#include "DialogueStartGraphNode.h"
#include "DialogueEndGraphNode.h"
#include "DialogueNodeInfo.h"
#include "DialogueEditor.h"
#include "DialogueAsset.h"

FDialogueAssetAction::FDialogueAssetAction(EAssetTypeCategories::Type Category)
{
    AssetCategory = Category;
}

FText FDialogueAssetAction::GetName() const
{
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_DialogueAsset", "Dialogue Asset");
}

FColor FDialogueAssetAction::GetTypeColor() const
{
    return FColor::Orange;
}

UClass* FDialogueAssetAction::GetSupportedClass() const
{
    return UDialogueAsset::StaticClass();
}

void FDialogueAssetAction::OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (UObject* object : inObjects)
    {
        UDialogueAsset* DialogueAsset = Cast<UDialogueAsset>(object);
        if (DialogueAsset != nullptr)
        {
            TSharedRef<DialogueEditor> editor(new DialogueEditor());
            editor->InitEditor(mode, EditWithinLevelEditor, DialogueAsset);
        }
    }
}

uint32 FDialogueAssetAction::GetCategories()
{
    return AssetCategory;
}

DialogueAssetAppMode::DialogueAssetAppMode(TSharedPtr<DialogueEditor> inApp) : FApplicationMode(TEXT("DialogueAssetAppMode"))
{
    App = inApp;
    Tabs.RegisterFactory(MakeShareable(new DialogueAssetPrimaryTabFactory(inApp)));
    Tabs.RegisterFactory(MakeShareable(new DialogueAssetPropertiesTabFactory(inApp)));

    TabLayout = FTabManager::NewLayout("DialogueAssetAppMode_Layout_v1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Vertical)
            ->Split
            (
                FTabManager::NewSplitter()
                ->SetOrientation(Orient_Horizontal)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.75)
                    ->AddTab(FName(TEXT("DialogueAssetPrimaryTab")), ETabState::OpenedTab)
                )
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.25)
                    ->AddTab(FName(TEXT("DialogueAssetPropertiesTab")), ETabState::OpenedTab)
                )
            )
        );
}

void DialogueAssetAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
    TSharedPtr<DialogueEditor> EditorApp = App.Pin();
    EditorApp->PushTabFactories(Tabs);
    FApplicationMode::RegisterTabFactories(InTabManager);
}

void DialogueAssetAppMode::PreDeactivateMode()
{
    FApplicationMode::PreDeactivateMode();
}

void DialogueAssetAppMode::PostActivateMode()
{
    FApplicationMode::PostActivateMode();
}

DialogueAssetPropertiesTabFactory::DialogueAssetPropertiesTabFactory(TSharedPtr<DialogueEditor> app) : FWorkflowTabFactory(FName("DialogueAssetPropertiesTab"), app)
{
    App = app;
    TabLabel = FText::FromString(TEXT("Properties"));
    ViewMenuDescription = FText::FromString(TEXT("Displays the properties view for the current asset."));
    ViewMenuTooltip = FText::FromString(TEXT("Show the properties view."));
}

TSharedRef<SWidget> DialogueAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
    TSharedPtr<DialogueEditor> EditorApp = App.Pin();
    FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    FDetailsViewArgs DetailsViewArgs;
    {
        DetailsViewArgs.bAllowSearch = false;
        DetailsViewArgs.bHideSelectionTip = true;
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.bSearchInitialKeyFocus = true;
        DetailsViewArgs.bUpdatesFromSelection = false;
        DetailsViewArgs.NotifyHook = nullptr;
        DetailsViewArgs.bShowOptions = true;
        DetailsViewArgs.bShowModifiedPropertiesOption = false;
        DetailsViewArgs.bShowScrollBar = false;
    }

    TSharedPtr<IDetailsView> DetailsView = propertyEditorModule.CreateDetailView(DetailsViewArgs);
    DetailsView->SetObject(EditorApp->GetWorkingAsset());

    TSharedPtr<IDetailsView> selectedNodeDetailsView = propertyEditorModule.CreateDetailView(DetailsViewArgs);
    selectedNodeDetailsView->SetObject(nullptr);
    EditorApp->SetSelectedNodeDetailView(selectedNodeDetailsView);

    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            DetailsView.ToSharedRef()
        ]
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            selectedNodeDetailsView.ToSharedRef()
        ];
}

FText DialogueAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
    return FText::FromString(TEXT("A properties view for the current asset."));
}

DialogueAssetPrimaryTabFactory::DialogueAssetPrimaryTabFactory(TSharedPtr<DialogueEditor> InApp) : FWorkflowTabFactory(FName("DialogueAssetPrimaryTab"), InApp)
{
    App = InApp;
    TabLabel = FText::FromString(TEXT("Primary"));
    ViewMenuDescription = FText::FromString(TEXT("Displays a primary view for whatever you want to do."));
    ViewMenuTooltip = FText::FromString(TEXT("Show the primary view."));
}

TSharedRef<SWidget> DialogueAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
    TSharedPtr<DialogueEditor> EditorApp = App.Pin();

    SGraphEditor::FGraphEditorEvents GraphEvents;
    GraphEvents.OnSelectionChanged.BindRaw(EditorApp.Get(), &DialogueEditor::OnGraphSelectionChanged);

    TSharedPtr<SGraphEditor> GraphEditor =
        SNew(SGraphEditor)
        .IsEditable(true)
        .GraphEvents(GraphEvents)
        .GraphToEdit(EditorApp->GetWorkingGraph());
    EditorApp->SetWorkingGraphUi(GraphEditor);

    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            GraphEditor.ToSharedRef()
        ];
}

FText DialogueAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
    return FText::FromString(TEXT("A primary view for doing primary things."));
}