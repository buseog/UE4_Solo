// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class AMyMonster;
class AMyPlayer;

UCLASS()
class ESSUE_API AMyWeapon : public AActor
{
	GENERATED_BODY()

public:
	enum OwnerID { PLAYER, MONSTER };
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AActor* Owner;
	FString AttachPoint;

	OwnerID OwnerType;
	bool Attacking;
	
	TArray<AActor*> HitActor;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString Type;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	class UBoxComponent* ProxBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	class USkeletalMeshComponent* Mesh;

private:
	void AttachToPawn(void);

public:
	void SetOwningPawn(AMyMonster* _Owner);
	void SetOwningPawn(AMyPlayer* _Owner, FString _AttachPoint);
	void SetAttachPoint(FString _AttachPoint);

public:
	void Attack(void);
	void Rest(void);
	void OnEquip(void);
	void UnEquip(void);

public:
	UFUNCTION(BlueprintNativeEvent, Category = Collosion)
	void OnProxOverlapBegin(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	virtual void OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	
};
