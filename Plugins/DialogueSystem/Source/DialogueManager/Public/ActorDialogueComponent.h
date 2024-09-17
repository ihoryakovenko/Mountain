#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "ActorDialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGUEMANAGER_API UActorDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActorDialogueComponent();

	void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FGameplayTag DialogueActorTag;

	FGameplayTagContainer GrantedDialogueTags;
};
