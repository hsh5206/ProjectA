// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Main/MainCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapon/Weapon.h"
#include "Animation/AnimMontage.h"

AMainCharacter::AMainCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 250.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(0.f, 65.f, 0.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

}

void AMainCharacter::ChangeArmDsiarm()
{
	if (MainState == ECharacterState::EAS_Armed)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("SwordHolderSocket"));
		MainState = ECharacterState::EAS_Unarmed;
	}
	else if (MainState == ECharacterState::EAS_Unarmed)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		MainState = ECharacterState::EAS_Armed;
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainCharacter::Turn);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("WalkRun"), EInputEvent::IE_Pressed, this, &AMainCharacter::WalkRun);
	PlayerInputComponent->BindAction(FName("WalkRun"), EInputEvent::IE_Released, this, &AMainCharacter::WalkRun);
	PlayerInputComponent->BindAction(FName("ArmDisarm"), EInputEvent::IE_Pressed, this, &AMainCharacter::ArmDisarm);
	PlayerInputComponent->BindAction(FName("EPress"), EInputEvent::IE_Pressed, this, &AMainCharacter::EPress);
}

void AMainCharacter::MoveForward(float value)
{
	if (Controller && (value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, value);
	}
}

void AMainCharacter::MoveRight(float value)
{
	if (Controller && (value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, value);
	}
}

void AMainCharacter::LookUp(float value)
{
	if (Controller && (value != 0.f))
	{
		AddControllerPitchInput(value);
	}
}

void AMainCharacter::Turn(float value)
{
	if (Controller && (value != 0.f))
	{
		AddControllerYawInput(value);
	}
}

void AMainCharacter::WalkRun()
{
	UE_LOG(LogTemp, Warning, TEXT("WalkRun"));
	if (bWalk)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		bWalk = false;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		bWalk = true;
	}
}

void AMainCharacter::ArmDisarm()
{
	if (!Weapon) return;
	if (MainState == ECharacterState::EAS_Armed)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(FName("CombatToIdle"), EquipMontage);
		}
	}
	else if (MainState == ECharacterState::EAS_Unarmed)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(FName("IdleToCombat"), EquipMontage);
		}
	}
}

void AMainCharacter::EPress()
{
	AWeapon* TempWeapon = Cast<AWeapon>(OverlappedItem);
	if (TempWeapon)
	{
		Weapon = TempWeapon;
		OverlappedItem = nullptr;
		Weapon->Equip(GetMesh(), FName("SwordHolderSocket"));
	}
}
