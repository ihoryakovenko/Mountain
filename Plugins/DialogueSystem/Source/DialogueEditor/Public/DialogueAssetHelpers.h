#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FDialogueAssetAction : public FAssetTypeActions_Base
{
public:
	FDialogueAssetAction(EAssetTypeCategories::Type Category);

	FText GetName() const override;
	FColor GetTypeColor() const override;
	UClass* GetSupportedClass() const override;
	void OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type AssetCategory;
};

class DialogueAssetAppMode : public FApplicationMode
{
public:
	DialogueAssetAppMode(TSharedPtr<class DialogueEditor> inApp);

	void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	void PreDeactivateMode() override;
	void PostActivateMode() override;

private:
	TWeakPtr<class DialogueEditor> App;
	FWorkflowAllowedTabSet Tabs;
};

class DialogueAssetPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	DialogueAssetPrimaryTabFactory(TSharedPtr<class DialogueEditor> inApp);

	TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class DialogueEditor> App;
};

class DialogueAssetPropertiesTabFactory : public FWorkflowTabFactory
{
public:
	DialogueAssetPropertiesTabFactory(TSharedPtr<class DialogueEditor> inApp);

	TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class DialogueEditor> App;
};