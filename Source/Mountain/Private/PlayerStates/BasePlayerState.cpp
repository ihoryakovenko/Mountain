#include "PlayerStates/BasePlayerState.h"

#include "GAS/BaseAbilitySystemComponent.h"
#include "GAS/BaseAttributeSet.h"

#include "Characters/MountainCharacter.h"

DEFINE_LOG_CATEGORY(BasePlayerStateLog);

ABasePlayerState::ABasePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game
	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void ABasePlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{ }

void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();

	ensureAlways(AbilitySystemComponent != nullptr);

	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ABasePlayerState::HealthChanged);
	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ABasePlayerState::MaxHealthChanged);
	LevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetLevelAttribute()).AddUObject(this, &ABasePlayerState::LevelChanged);

	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ABasePlayerState::StunTagChanged);
}

void ABasePlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(BasePlayerStateLog, Log, TEXT("HealthChanged"));

	if (Data.OldValue > 0.0f)
	{
		if (Data.NewValue <= 0.0f)
		{
			AMountainCharacter* Cahracter = Cast<AMountainCharacter>(GetPawn());
			Cahracter->Die();
		}
	}
}

void ABasePlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(BasePlayerStateLog, Log, TEXT("MaxHealthChanged"));
}

void ABasePlayerState::LevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(BasePlayerStateLog, Log, TEXT("LevelChanged"));
}

void ABasePlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	UE_LOG(BasePlayerStateLog, Log, TEXT("StunTagChanged"));

	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
