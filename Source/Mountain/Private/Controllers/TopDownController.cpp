#include "Controllers/TopDownController.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/MountainCharacter.h"
#include "PlayerStates/BasePlayerState.h"
#include "GAS/BaseAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(TopDownControllerLog);

ATopDownController::ATopDownController()
{
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Default;
}

void ATopDownController::Tick(float DeltaSeconds)
{
	if (APawn* ControlledCharacter = GetPawn())
	{
		UAbilitySystemComponent* ASC = CastChecked<IAbilitySystemInterface>(GetPawn())->GetAbilitySystemComponent();
		if (ASC != nullptr && !ASC->HasMatchingGameplayTag(MovementBlockedTag))
		{
			// Rotates character to mouse position
			FVector WorldLocation;
			FVector WorldDirection;
			if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
			{
				const FVector ActorLocation = ControlledCharacter->GetActorLocation();

				const float Distance = (ActorLocation.Z - WorldLocation.Z) / WorldDirection.Z;
				const FVector IntersectionPoint = WorldLocation + (WorldDirection * Distance);

				const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, IntersectionPoint);
				SetControlRotation(FRotator(0, NewRotation.Yaw, 0));
			}
		}
	}
}

void ATopDownController::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		PS->AbilitySystemComponent->InitAbilityActorInfo(PS, InPawn);
	}
}

void ATopDownController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ensureAlways(JumpAction != nullptr);
	ensureAlways(MoveAction != nullptr);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(TopDownMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATopDownController::OnJump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownController::OnMove);
	}
	else
	{
		UE_LOG(TopDownControllerLog, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

void ATopDownController::OnJump()
{
	if (AMountainCharacter* ControlledCharacter = Cast<AMountainCharacter>(GetPawn()))
	{
		UAbilitySystemComponent* ASC = ControlledCharacter->GetAbilitySystemComponent();
		if (ASC != nullptr && !ASC->HasMatchingGameplayTag(MovementBlockedTag))
		{
			ControlledCharacter->Jump();
		}
	}
	else
	{
		UE_LOG(TopDownControllerLog, Error, TEXT("'%s' Failed to get AMountainCharacter"), *GetNameSafe(this));
	}
}

void ATopDownController::OnMove(const FInputActionValue& Value)
{
	if (AMountainCharacter* ControlledCharacter = Cast<AMountainCharacter>(GetPawn()))
	{
		UAbilitySystemComponent* ASC = ControlledCharacter->GetAbilitySystemComponent();
		if (ASC != nullptr && !ASC->HasMatchingGameplayTag(MovementBlockedTag))
		{
			const FVector2D Direction = Value.Get<FVector2D>().GetSafeNormal();
			ControlledCharacter->AddMovementInput(FVector(Direction, 0));
		}
	}
	else
	{
		UE_LOG(TopDownControllerLog, Error, TEXT("'%s' Failed to get AMountainCharacter"), *GetNameSafe(this));
	}
}
