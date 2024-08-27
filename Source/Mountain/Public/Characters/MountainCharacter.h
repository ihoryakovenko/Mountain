#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "GAS/BaseAbilitySystemComponent.h"

// TODO: Test
#include "AssetClasses/PlayerAbilitiesInputConfig.h"
#include "AssetClasses/PlayerGameplayConfig.h"

#include "MountainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AMountainCharacter*, Character);

UCLASS(Blueprintable)
class AMountainCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMountainCharacter(const FObjectInitializer& ObjectInitializer);

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void RemoveCharacterAbilities();

	int32 GetAbilityLevel(EAbilityInputID AbilityID) const;
	int32 GetLevel() const;
	bool IsAlive() const;

	void Die();

	void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FCharacterDiedDelegate OnCharacterDied;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> OnDeathEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Config")
	FGameplayTag GameplayClassTag;

	// TDOD: Move UInputAction to controller?
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Config")
	TSoftObjectPtr<UPlayerAbilitiesInputConfig> InputConfig;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Config")
	TSoftObjectPtr<UPlayerGameplayConfig> GameplayConfig;

private:
	void FinishDying();

	void AddCharacterAbilities();
	void InitializeAttributes();
	void AddStartupEffects();

	void BindASCInput();

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	void PossessedBy(AController* NewController) override;

	void OnRep_PlayerState() override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; //TODO: Check

private:
	// TDOD: Move UInputAction to controller?
	UPROPERTY(Transient)
	TMap<UInputAction*, FGameplayAbilitySpecHandle> InputToAbilityMap;

	TWeakObjectPtr<class UBaseAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UBaseAttributeSet> AttributeSetBase;

	FGameClassConfig ClassConfig;

	FGameplayEffectContextHandle EffectContext;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	bool ASCInputBound = false;
};

