#include "Characters/MountainCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

#include "GAS/BaseGameplayAbility.h"
#include "GAS/BaseAttributeSet.h"
#include "PlayerStates/BasePlayerState.h"

AMountainCharacter::AMountainCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0, 0.0, 640.0);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));

	bAlwaysRelevant = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
}

UAbilitySystemComponent* AMountainCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

int32 AMountainCharacter::GetAbilityLevel(EAbilityInputID AbilityID) const
{
	return 1;
}

void AMountainCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

int32 AMountainCharacter::GetLevel() const
{
	if (AttributeSetBase.IsValid())
	{
		return static_cast<int32>(AttributeSetBase->GetLevel());
	}

	return 0;
}

bool AMountainCharacter::IsAlive() const
{
	return AttributeSetBase.IsValid() && AttributeSetBase->GetHealth() > 0.0f;
}

void AMountainCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMountainCharacter::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		// TODO: Check
		if (GetLocalRole() == ROLE_Authority)
		{
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(OnDeathEffect, GetLevel(), EffectContext);
			ensureAlways(NewHandle.IsValid());
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	OnCharacterDied.Broadcast(this);

	FinishDying();
}

void AMountainCharacter::FinishDying()
{
	//Destroy();
}

void AMountainCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		const FGameplayAbilitySpec Spec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this);

		AbilitySystemComponent->GiveAbility(Spec);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AMountainCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("%s() AbilitySystemComponent is not valid"), *FString(__FUNCTION__), *GetName());
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s() NewHandle is not valid."), *FString(__FUNCTION__), *GetName());
	}
}

void AMountainCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetLevel(), EffectContext);
		ensureAlways(NewHandle.IsValid());
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void AMountainCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		// TODO: Check path
		FTopLevelAssetPath AbilityEnumAssetPath(FName("/Script/Mountain"), FName("EAbilityInputID"));
		//ensureAlways(AbilityEnumAssetPath.IsValid()); Bullshit checks only PackageName.IsNone() Unreal crashes if asset is not valid

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AMountainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = PS->AbilitySystemComponent;
		AttributeSetBase = PS->AttributeSetBase;

		// TODO: Need only for AI if it will be controll this Character FIX
		// AI won't have PlayerControllers so we can init again here just to be sure
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		// Respawn specific things that won't affect first possession.

		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// This is only necessary for *Respawn*.
		AttributeSetBase->SetHealth(AttributeSetBase->GetMaxHealth());
		AttributeSetBase->SetLevel(AttributeSetBase->GetLevel());
		// End respawn specific things

		AddStartupEffects();
		AddCharacterAbilities();
	}
}

void AMountainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		AbilitySystemComponent = PS->AbilitySystemComponent;
		AttributeSetBase = PS->AttributeSetBase;

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		
		BindASCInput();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();


		// Respawn specific things that won't affect first possession.

		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		AttributeSetBase->SetHealth(AttributeSetBase->GetMaxHealth());
		AttributeSetBase->SetLevel(AttributeSetBase->GetLevel());
	}
}

void AMountainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindASCInput();
}
