// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyNpc.generated.h"

UCLASS()
class ESSUE_API AMyNpc : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNpc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool	GiveQuest;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FString NpcName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FString NpcMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC)
	class USphereComponent* ProxSphere;

public:
	UFUNCTION(BlueprintNativeEvent, Category = Collosion)
	void OnProxOverlapBegin(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	virtual void OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
