// Fill out your copyright notice in the Description page of Project Settings.

#include "MySpell.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MyMonster.h"
#include "MyPlayer.h"

// Sets default values
AMySpell::AMySpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProxBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxProx"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	
	RootComponent = Particle;
	ProxBox->AttachTo(RootComponent);

	Owner = nullptr;
	Duration = 1.f;
	Damage = 100.f;
	AliveTime = 0.f;
}

// Called when the game starts or when spawned
void AMySpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Collision(DeltaTime);

	AliveTime += DeltaTime;
	
	if (AliveTime >= Duration)
		Destroy();
}

void AMySpell::SetCaster(AActor* _Owner)
{
	Owner = _Owner;
}

void AMySpell::Collision(float _DeltaTime)
{
	TArray<AActor*> Actors;
	ProxBox->GetOverlappingActors(Actors);

	for (int i = 0; i < Actors.Num(); ++i)
	{
		if (Owner == Actors[i])
			continue;

		AMyMonster* Monster = Cast<AMyMonster>(Actors[i]);

		if (nullptr != Monster && ProxBox->IsOverlappingComponent(Monster->GetRootPrimitiveComponent()))
		{
			if (0.f >= Monster->TakeDamage(Damage, FDamageEvent(), NULL, this))
			{
				AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				Player->RenewalQuest(Monster->MonsterName);
				Monster->Destroy();
			}
		}
	}
}