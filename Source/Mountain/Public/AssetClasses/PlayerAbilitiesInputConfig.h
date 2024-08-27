#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "PlayerAbilitiesInputConfig.generated.h"

class UInputAction;

UCLASS()
class MOUNTAIN_API UPlayerAbilitiesInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UInputAction> EmptyAction;

	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UInputAction>> AbilityInputs;
};
