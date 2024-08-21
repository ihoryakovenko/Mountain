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
	int32 GetAbilityLevel(EAbilityInputID AbilityID) const;
	void RemoveCharacterAbilities();
	int32 GetLevel() const;

	void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FCharacterDiedDelegate OnCharacterDied;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UBaseGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

private:
	void Die();
	void FinishDying();

	void AddCharacterAbilities();
	void InitializeAttributes();
	void AddStartupEffects();

private:
	TWeakObjectPtr<class UBaseAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UBaseAttributeSet> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;
};

