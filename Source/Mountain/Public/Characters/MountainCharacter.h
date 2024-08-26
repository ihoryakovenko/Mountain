#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "GAS/BaseAbilitySystemComponent.h"

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

	// TDOD: Move UInputAction to controller?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TMap<class UInputAction*, TSubclassOf<class UBaseGameplayAbility>> CharacterAbilities;

	// TDOD: Move UInputAction to controller?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TMap<UInputAction*, FGameplayAbilitySpecHandle> InputToAbilityMap;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> OnDeathEffect;

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
	TWeakObjectPtr<class UBaseAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UBaseAttributeSet> AttributeSetBase;

	FGameplayEffectContextHandle EffectContext;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	bool ASCInputBound = false;
};

