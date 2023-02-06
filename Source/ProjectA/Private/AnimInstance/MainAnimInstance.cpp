// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/MainAnimInstance.h"
#include "Characters/Main/MainCharacter.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (Character)
	{
		MainCharacter = Cast<AMainCharacter>(Character);
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		CharacterState = MainCharacter->MainState;
		CombatState = MainCharacter->CombatState;
	}
}
