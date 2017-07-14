// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBullet.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "MyPlayer.h"
#include "MyMonster.h"


// Sets default values
AMyBullet::AMyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProxSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProxSphere"));
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyBullet::OnProxOverlapBegin);
	ProxSphere->InitSphereRadius(15.f);
	RootComponent = ProxSphere;

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovement->SetUpdatedComponent(ProxSphere);
	BulletMovement->InitialSpeed = 2000.f;
	BulletMovement->MaxSpeed = 2000.f;
	BulletMovement->bRotationFollowsVelocity = true;
	BulletMovement->bShouldBounce = true;
	BulletMovement->Bounciness = 0.01f;
	InitialLifeSpan = 1.f;
	OwnerType = OwnerID(-1);

	Damage = 1.f;
	Speed = 1.f;
}

// Called when the game starts or when spawned
void AMyBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBullet::PostInitializeComponents(void)
{
	Super::PostInitializeComponents();

	BulletMovement->InitialSpeed = Speed;
	BulletMovement->MaxSpeed = Speed;
	InitialLifeSpan = LifeTime;
}

void AMyBullet::SetOwnerType(OwnerID _OwnerID)
{
	OwnerType = _OwnerID;
}

void AMyBullet::OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (OwnerType < 0)
		return;

	if (_OtherComp != _OtherActor->GetRootComponent() || _OtherActor == this)
		return;

	if (OwnerType == PLAYER)
	{
		if (0.f >= _OtherActor->TakeDamage(Damage, FDamageEvent(), NULL, this))
		{
			AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			Player->RenewalQuest(Cast<AMyMonster>(_OtherActor)->MonsterName);
			_OtherActor->Destroy();
		}
	}
	else
		_OtherActor->TakeDamage(Damage, FDamageEvent(), NULL, this);

	Destroy();
}