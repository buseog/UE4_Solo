// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWeapon.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyMonster.h"
#include "MyPlayer.h"


// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Name = TEXT("Unknown Weapon");
	Damage = 1.f;
	Attacking = false;
	Owner = nullptr;
	OwnerType = PLAYER;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

	ProxBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProxBox"));
	ProxBox->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnProxOverlapBegin);
	ProxBox->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
			
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (false == Attacking || Type == FString(TEXT("Gun")))
		return;

	if (_OtherComp != _OtherActor->GetRootComponent() || _OtherActor == this || _OtherActor == Owner)
		return;

	if (true == HitActor.Contains(_OtherActor))
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

	HitActor.Add(_OtherActor);
}

void AMyWeapon::SetOwningPawn(AMyMonster* _Owner)
{
	if (Owner != _Owner)
		Owner = _Owner;

	AttachPoint = TEXT("RightHandAttach");
	OwnerType = MONSTER;
}

void AMyWeapon::SetOwningPawn(AMyPlayer* _Owner, FString _AttachPoint)
{
	if (Owner != _Owner)
		Owner = _Owner;

	AttachPoint = _AttachPoint;
	OwnerType = PLAYER;
}

void AMyWeapon::SetAttachPoint(FString _AttachPoint)
{
	AttachPoint = _AttachPoint;
}

void AMyWeapon::AttachToPawn(void)
{
	if (nullptr == Owner)
		return;

	USkeletalMeshComponent* PawnMesh = nullptr;

	if (OwnerType == PLAYER)
		PawnMesh = Cast<AMyPlayer>(Owner)->GetSkeletalMeshComponent();

	else if (OwnerType == MONSTER)
		PawnMesh = Cast<AMyMonster>(Owner)->GetSkeletalMeshComponent();

	Mesh->AttachTo(PawnMesh, FName(*AttachPoint));
}

void AMyWeapon::OnEquip(void)
{
	AttachToPawn();
}

void AMyWeapon::UnEquip(void)
{
	Mesh->DetachFromParent();
}

void AMyWeapon::Attack(void)
{
	HitActor.Empty();
	Attacking = true;
}

void AMyWeapon::Rest(void)
{
	HitActor.Empty();
	Attacking = false;
}