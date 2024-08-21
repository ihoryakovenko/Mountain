#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "BasePlayerState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BasePlayerStateLog, Log, All);

UCLASS()
class MOUNTAIN_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABasePlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

public:
	UPROPERTY()
	class UBaseAttributeSet* AttributeSetBase;

	UPROPERTY()
	class UBaseAbilitySystemComponent* AbilitySystemComponent;

private:
	void BeginPlay() override;

	void HealthChanged(const FOnAttributeChangeData& Data);
	void MaxHealthChanged(const FOnAttributeChangeData& Data);
	void LevelChanged(const FOnAttributeChangeData& Data);

	void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

private:
	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle LevelChangedDelegateHandle;
};
