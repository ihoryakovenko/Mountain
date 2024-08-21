#include "GAS/BaseAbilitySystemComponent.h"

void UBaseAbilitySystemComponent::ReceiveDamage(UBaseAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
