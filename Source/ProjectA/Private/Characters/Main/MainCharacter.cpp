// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Main/MainCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapon/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Characters/Main/MainPlayerController.h"

AMainCharacter::AMainCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 270.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 6.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1500.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	LockOnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LockOnSphere"));
	LockOnSphere->SetupAttachment(GetRootComponent());

	AttackEndComboState();

	MaxHealth = 100;
	Health = 100;
	MaxStamina = 100;
	Stamina = 100;

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	LockOnSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnLockOnOverlap);
	LockOnSphere->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnLockOnOverlapEnd);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MainState == ECharacterArmedState::EAS_LockOn)
	{
		const FRotator CharacterRotation = GetActorRotation();
		const FRotator Direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedOnEnemy->GetActorLocation());
		SetActorRotation(FRotator(CharacterRotation.Pitch, Direction.Yaw, CharacterRotation.Roll));
		GetController()->SetControlRotation(FRotator(CharacterRotation.Pitch, Direction.Yaw, CharacterRotation.Roll));
	}
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
	PlayerInputComponent->BindAction(FName("LockOn"), EInputEvent::IE_Pressed, this, &AMainCharacter::LockOn);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Roll"), EInputEvent::IE_Pressed, this, &AMainCharacter::Roll);
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

	/** Roll Direction */
	if (value > 0.f)
	{
		CharacterRollState.bFront = true;
		CharacterRollState.bBack = false;
	}
	else if (value < 0.f)
	{
		CharacterRollState.bFront = false;
		CharacterRollState.bBack = true;
	}
	else
	{
		CharacterRollState.bFront = false;
		CharacterRollState.bBack = false;
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

	/** Roll Direction */
	if (value > 0.f)
	{
		CharacterRollState.bRight = true;
		CharacterRollState.bLeft = false;
	}
	else if (value < 0.f)
	{
		CharacterRollState.bRight = false;
		CharacterRollState.bLeft = true;
	}
	else
	{
		CharacterRollState.bRight = false;
		CharacterRollState.bLeft = false;
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
	if (MainState == ECharacterArmedState::EAS_Armed)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(FName("CombatToIdle"), EquipMontage);
		}
	}
	else if (MainState == ECharacterArmedState::EAS_Unarmed)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(FName("IdleToCombat"), EquipMontage);
		}
	}
}

void AMainCharacter::ChangeArmDsiarm()
{
	if (MainState == ECharacterArmedState::EAS_Armed)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("SwordHolderSocket"));
		MainState = ECharacterArmedState::EAS_Unarmed;
	}
	else if (MainState == ECharacterArmedState::EAS_Unarmed)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		MainState = ECharacterArmedState::EAS_Armed;
	}
	else if (MainState == ECharacterArmedState::EAS_UnarmedToLockOn)
	{
		Weapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		MainState = ECharacterArmedState::EAS_LockOn;
	}
}

void AMainCharacter::EPress()
{
	AWeapon* TempWeapon = Cast<AWeapon>(OverlappedItem);
	if (TempWeapon)
	{
		Weapon = TempWeapon;
		OverlappedItem = nullptr;
		Weapon->Equip(GetMesh(), FName("SwordHolderSocket"), this);
	}
}

void AMainCharacter::LockOn()
{
	if (!Weapon) return;
	if (!CanLockedOnEnemy) return;
	if (MainState == ECharacterArmedState::EAS_Unarmed)
	{
		MainState = ECharacterArmedState::EAS_UnarmedToLockOn;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(FName("IdleToCombat"), EquipMontage);
		}
	}
	else
	{
		MainState = ECharacterArmedState::EAS_LockOn;
	}
	LockedOnEnemy = CanLockedOnEnemy;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AMainCharacter::UnLockOn()
{
	MainState = ECharacterArmedState::EAS_Armed;
	LockedOnEnemy = nullptr;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainCharacter::Attack()
{
	if (CombatState == ECharacterCombatState::ECS_Rolling) return;
	if (CombatState==ECharacterCombatState::ECS_Attacking && !CanNextCombo) return;
	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1))
	{
		CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	}
	CanNextCombo = false;
	PlayAttackMontage(FName(*FString::Printf(TEXT("Attack%d"), CurrentCombo)));
}

void AMainCharacter::PlayAttackMontage(FName SectionName)
{
	//UE_LOG(LogTemp, Warning, TEXT("AttackMontage"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Animnotify_PlayAttackMontage Func %s"), *SectionName.ToString());
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		CombatState = ECharacterCombatState::ECS_Attacking;
	}
}

void AMainCharacter::AttackStartComboState()
{
	CanNextCombo = true;
}

void AMainCharacter::AttackEndComboState()
{
	CombatState = ECharacterCombatState::ECS_Default;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMainCharacter::Roll()
{
	if (CombatState == ECharacterCombatState::ECS_Rolling) return;
	if (CombatState == ECharacterCombatState::ECS_Attacking) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && RollMontage)
	{
		AnimInstance->Montage_Play(RollMontage);
		if (MainState == ECharacterArmedState::EAS_LockOn)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d"), CharacterRollState.bFront, CharacterRollState.bBack, CharacterRollState.bRight, CharacterRollState.bLeft);
			FName Way = GetRollWay();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *Way.ToString());
			if (Way.IsValid())
			{
				AnimInstance->Montage_JumpToSection(Way, RollMontage);
			}
		}
		CombatState = ECharacterCombatState::ECS_Rolling;
	}

	Stamina -= 10.f;
	if (MainPlayerController)
	{
		MainPlayerController->SetStaminaPercent(MaxStamina, Stamina);
	}
}

FName AMainCharacter::GetRollWay()
{
	FName Way;
	if (CharacterRollState.bFront)
	{
		Way = FName("F");
		if (CharacterRollState.bRight)
		{
			Way = FName("FR");
		}
		else if(CharacterRollState.bLeft)
		{
			Way = FName("FL");
		}
	}
	else if (CharacterRollState.bBack)
	{
		Way = FName("B");
		if (CharacterRollState.bRight)
		{
			Way = FName("BR");
		}
		else if (CharacterRollState.bLeft)
		{
			Way = FName("BL");
		}
	}
	else
	{
		if (CharacterRollState.bRight)
		{
			Way = FName("R");
		}
		else if (CharacterRollState.bLeft)
		{
			Way = FName("L");
		}
		else
		{
			Way = FName("F");
		}
	}
	return Way;
}

void AMainCharacter::RollEnd()
{
	CombatState = ECharacterCombatState::ECS_Default;
}

void AMainCharacter::OnLockOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* TempCharacter = Cast<APawn>(OtherActor);
	if (TempCharacter && TempCharacter != this)
	{
		CanLockedOnEnemy = TempCharacter;
	}
}

void AMainCharacter::OnLockOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* TempCharacter = Cast<APawn>(OtherActor);
	if (TempCharacter && TempCharacter != this)
	{
		CanLockedOnEnemy = nullptr;
	}
	if (MainState == ECharacterArmedState::EAS_LockOn) UnLockOn();
}

