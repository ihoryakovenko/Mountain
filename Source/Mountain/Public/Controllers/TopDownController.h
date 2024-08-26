#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"

#include "TopDownController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(TopDownControllerLog, Log, All);

UCLASS()
class ATopDownController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownController();

public:
	void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* TopDownMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Init")
	FGameplayTag MovementBlockedTag;

private:
	void SetupInputComponent() override;
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

	void OnJump();
	void OnMove(const FInputActionValue& Value);

private:
	TWeakObjectPtr<class UBaseAbilitySystemComponent> AbilitySystemComponent;
};


