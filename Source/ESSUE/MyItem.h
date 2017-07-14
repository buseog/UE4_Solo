// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyItem.generated.h"

UCLASS()
class ESSUE_API AMyItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 Type;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
	class USphereComponent* ProxSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	class UTexture2D* Icon;

public:
	UFUNCTION(BlueprintNativeEvent, Category = Collosion)
	void OnProxOverlapBegin(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	virtual void OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	
	
};
