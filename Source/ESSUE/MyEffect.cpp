// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEffect.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "MyPlayer.h"

// Sets default values
AMyEffect::AMyEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	RootComponent = Particle;

	Duration = 1.f;
	AliveTime = 0.f;
}

// Called when the game starts or when spawned
void AMyEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveTime += DeltaTime;

	if (AliveTime >= Duration)
		Destroy();
}

void AMyEffect::SetLocation(FVector _Location)
{
	SetActorLocation(_Location);
}