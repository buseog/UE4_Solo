// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "MyMonster.generated.h"

class AMyWeapon;
class AMyBullet;

UCLASS()
class ESSUE_API AMyMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents(void) override;
	virtual float TakeDamage(float _Damage, struct FDamageEvent const& _DamageEvent, class AController* _EventInstigator, class AActor* _DamageCauser) override;

private:
	enum AnimationID { IDLE, RUN, JUMP, SLASH, SHOOT };

private:
	AMyWeapon* Weapon;
	FVector KnockBackPower;
	float	MotionDelay;
	int		CurrentState;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	TSubclassOf<AMyWeapon> BPWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	TSubclassOf<AMyBullet> BPBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float BulletLaunchImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	FString MonsterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float AttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	class UClass* BPDropItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MONSTER)
	float LastAttackTime;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MONSTER)
	class USphereComponent* SightSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MONSTER)
	class USphereComponent* AttackRangeSphere;

	UPROPERTY(EditDefaultsOnly, Category = MONSTER)
	TArray<UAnimSequence*> Animation;

private:
	void KnockBack(void);
	void Pattern(float _DeltaTime);

	void OnAttack(AActor* _EnemyActor);
	void OnSword(void);
	void OnShot(void);

public:
	inline bool IsInSightRange(float _Distance) { return _Distance <= SightSphere->GetScaledSphereRadius(); }
	inline bool IsInAttackRange(float _Distance) { return _Distance <= AttackRangeSphere->GetScaledSphereRadius(); }

public:
	inline USkeletalMeshComponent* GetSkeletalMeshComponent(void) const { return GetMesh(); }
};
