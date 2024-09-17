#include "ActorDialogueComponent.h"

UActorDialogueComponent::UActorDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
