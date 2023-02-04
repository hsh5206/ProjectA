// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/MainAnimInstance.h"
#include "Characters/Main/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		MainMovement = MainCharacter->GetCharacterMovement();
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MainMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MainMovement->Velocity);
		bIsInAir = MainMovement->IsFalling();
		CharacterState = MainCharacter->MainState;
		Direction = CalculateDirection(MainCharacter->GetVelocity(), MainCharacter->GetActorRotation());
	}
}
