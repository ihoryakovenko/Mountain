#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"

#include "GameplayAbilitiesDataAsset.generated.h"

class UInputAction;

USTRUCT()
struct FGameplayInputAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "GameplayInputAbilityInfo")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditAnywhere, Category = "GameplayInputAbilityInfo")
	TObjectPtr<UInputAction> InputAction;
};

UCLASS()
class MOUNTAIN_API UGameplayAbilitiesDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	
	
	
};
