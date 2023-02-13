// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"


class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECTA_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	void Equip(USceneComponent* InParent, const FName& InSocketName, APawn* NewInstigator);

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION(BlueprintCallable)
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(VisibleAnywhere)
	AActor* HitActor;

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceEnd;
};
