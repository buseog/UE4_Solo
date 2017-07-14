// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySpell.generated.h"

UCLASS()
class ESSUE_API AMySpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AActor* Owner;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Spell)
	class UBoxComponent* ProxBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Spell)
	class UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
	float AliveTime;

public:
	void SetCaster(AActor* _Owner);
	void Collision(float _DeltaTime);
};
