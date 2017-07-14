// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMonster.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMeshSocket.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Animation/AnimSequence.h"
#include "MyPlayer.h"
#include "MyWeapon.h"
#include "MyBullet.h"

// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = nullptr;

	MonsterName = TEXT("Unknown Monster");
	HP = 100.f;
	Exp = 0.f;
	Speed = 20.f;
	Damage = 20.f;
	AttackDelay = 1.5f;
	BPDropItem = nullptr;
	LastAttackTime = 0.f;

	SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SightSphere"));
	SightSphere->AttachTo(RootComponent);
	SightSphere->SetSphereRadius(300.f);
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->AttachTo(RootComponent);
	AttackRangeSphere->SetSphereRadius(60.f);

	CurrentState = IDLE;
	MotionDelay = 0.f;
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Pattern(DeltaTime);
	KnockBack();

	MotionDelay -= DeltaTime;

	if (MotionDelay <= 0.f && IDLE != CurrentState)
	{
		CurrentState = IDLE;
		GetMesh()->PlayAnimation(Animation[CurrentState], true);
		Weapon->Rest();
	}
}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyMonster::PostInitializeComponents(void)
{
	Super::PostInitializeComponents();

	if (nullptr == BPWeapon)
		return;

	Weapon = GetWorld()->SpawnActor<AMyWeapon>(BPWeapon);

	if (nullptr == Weapon)
		return;

	Weapon->SetOwningPawn(this);
	Weapon->OnEquip();
}

float AMyMonster::TakeDamage(float _Damage, struct FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	Super::TakeDamage(_Damage, _DamageEvent, _EventInstigator, _DamageCauser);

	KnockBackPower = GetActorLocation() - _DamageCauser->GetActorLocation();
	KnockBackPower.Normalize();
	KnockBackPower *= _Damage * 500.f;

	HP -= _Damage;

	if (HP <= 0.f)
		return 0.f;

	return HP;
}

void AMyMonster::KnockBack(void)
{
	AddMovementInput(KnockBackPower, 1.f);
	KnockBackPower *= 0.5f;
}

void AMyMonster::Pattern(float _DeltaTime)
{
	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (nullptr == Player)
		return;

	FVector DestDirection = Player->GetActorLocation() - GetActorLocation();
	float DestDistance = DestDirection.Size();

	if (false == IsInSightRange(DestDistance))
		return;

	DestDirection.Normalize();

	FRotator DestRotation = DestDirection.Rotation();
	DestRotation.Pitch = 0.f;
	RootComponent->SetWorldRotation(DestRotation);

	if (false == IsInAttackRange(DestDistance))
	{
		AddMovementInput(DestDirection, Speed * _DeltaTime);

		if (CurrentState != RUN && CurrentState != JUMP)
		{
			CurrentState = RUN;
			GetMesh()->PlayAnimation(Animation[CurrentState], true);
			GetMesh()->SetPosition(0.f);
			MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();
		}
	}

	else
	{
		LastAttackTime += _DeltaTime;

		if (LastAttackTime >= AttackDelay)
		{
			OnAttack(Player);
			LastAttackTime = 0.f;
		}
	}
}

void AMyMonster::OnAttack(AActor* _EnemyActor)
{
	if (Weapon->Type == FString(TEXT("Sword")))
		OnSword();

	else
		OnShot();
}

void AMyMonster::OnSword(void)
{
	if (CurrentState != RUN && MotionDelay >= 0.f)
		return;

	CurrentState = SLASH;
	GetMesh()->PlayAnimation(Animation[CurrentState], false);
	GetMesh()->SetPosition(0.f);
	MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();

	Weapon->Attack();
}

void AMyMonster::OnShot(void)
{
	if (CurrentState != RUN && MotionDelay >= 0.f)
		return;

	CurrentState = SHOOT;
	GetMesh()->PlayAnimation(Animation[CurrentState], false);
	GetMesh()->SetPosition(0.f);
	MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();

	FRotator Rotation = GetActorRotation();
	FVector Location = GetActorLocation() + Rotation.RotateVector(FVector(100.f, 30.f, 10.f));

	AMyBullet* Bullet = GetWorld()->SpawnActor<AMyBullet>(BPBullet, Location, Rotation);
	Bullet->SetOwnerType(AMyBullet::OwnerID::MONSTER);
}