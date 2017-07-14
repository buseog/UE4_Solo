// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Coreminimal.h"
#include "GameFramework/Actor.h"
#include "MyBullet.generated.h"

UCLASS()
class ESSUE_API AMyBullet : public AActor
{
	GENERATED_BODY()

public:
	enum OwnerID { PLAYER, MONSTER };

public:
	// Sets default values for this actor's properties
	AMyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents(void) override;

private:
	OwnerID OwnerType;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BULLET)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BULLET)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BULLET)
	float LifeTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BULLET)
	class USphereComponent* ProxSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BULLET)
	class UProjectileMovementComponent* BulletMovement;

public:
	void SetOwnerType(OwnerID _OwnerID);

public:
	UFUNCTION(BlueprintNativeEvent, Category = Collosion)
	void OnProxOverlapBegin(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	virtual void OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};