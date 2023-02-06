// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Movement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(Movement->Velocity);
		bIsInAir = Movement->IsFalling();
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
	}
}