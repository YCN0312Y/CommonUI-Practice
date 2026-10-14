// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Gameplay/YCNPlayerController.h"
#include "YCNUIDemo/Character/YCNCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

void AYCNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AYCNPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AYCNPlayerController::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AYCNPlayerController::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AYCNPlayerController::Jump);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AYCNPlayerController::StopJumping);
	}
}

void AYCNPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	YCNCharacter = Cast<AYCNCharacter>(aPawn);
}

void AYCNPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	YCNCharacter = nullptr;
}

void AYCNPlayerController::Move(const FInputActionValue& Value)
{
	if (YCNCharacter)
	{
		YCNCharacter->Move(Value);
	}
}

void AYCNPlayerController::Look(const FInputActionValue& Value)
{
	if (YCNCharacter)
	{
		YCNCharacter->Look(Value);
	}
}

void AYCNPlayerController::Jump()
{
	if (YCNCharacter)
	{
		YCNCharacter->Jump();
	}
}

void AYCNPlayerController::StopJumping()
{
	if (YCNCharacter)
	{
		YCNCharacter->StopJumping();
	}
}