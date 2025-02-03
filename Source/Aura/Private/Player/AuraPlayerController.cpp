// TechStartInnovations


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/IInteractable.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	
	/**
	 *Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null and ThisActor is null
	 *     - Do Nothing
	 *  B. LastActor is null and ThisActor is valid
	 *     - Hover on the actor for the first time.
	 *     - Hightlight ThisActor
	 *  C. LastActor is valid and ThisActor is null
	 *     - unhighlight the LastActor
	 *  D. Both actors are valid, but LastActor != ThisActor
	 *     - Unhighlight LastActor, and HIghlight ThisActor
	 *  E. Both actors are valid, and LastActor == ThisActor
	 *     - Do Nothing
	 */

	if (LastActor == nullptr && ThisActor == nullptr)
	{
		// Do Nothing
		return;
	}
	if (LastActor == nullptr && ThisActor != nullptr)
	{
		// Hover on the actor for the first time.
		// Highlight ThisActor
		ThisActor->HighlightActor();
		return;
	}
	if (LastActor != nullptr && ThisActor == nullptr)
	{
		// Unhighlight the LastActor
		LastActor->UnHighlightActor();
		return;
	}
	if (LastActor != nullptr && ThisActor != nullptr && LastActor != ThisActor)
	{
		// Unhighlight LastActor, and Highlight ThisActor
		LastActor->UnHighlightActor();
		ThisActor->HighlightActor();
		return;
	}
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation =  GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}
