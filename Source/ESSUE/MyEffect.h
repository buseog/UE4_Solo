// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffect.generated.h"

class AMyPlayer;

UCLASS()
class ESSUE_API AMyEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	float AliveTime;

public:
	void SetLocation(FVector _Location);
	
};
