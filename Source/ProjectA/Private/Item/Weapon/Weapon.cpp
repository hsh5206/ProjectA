// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/Weapon.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
{
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	Mesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, const FName& InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}