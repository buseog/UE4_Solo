// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ESSUE_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void StartPlay(void) override;
	
	
};
