#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "PlayerGameplayConfig.generated.h"

USTRUCT()
struct FGameClassConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Init")
	FGameplayTag GameplayClassTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TArray<TSubclassOf<class UBaseGameplayAbility>> ClassAbilities;
};

UCLASS()
class MOUNTAIN_API UPlayerGameplayConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FGameClassConfig> Configs;
};
