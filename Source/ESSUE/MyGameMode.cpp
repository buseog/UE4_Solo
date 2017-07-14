// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"

void AMyGameMode::StartPlay(void)
{
	Super::StartPlay();

	if (GEngine)
	{
		// 디버그 메시지 표시
		// 키값을 -1로 주면 메시지를 업데이트하거나 새로고침 하지 않음.
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("AA"), true, FVector2D(5.f, 5.f));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%d"), 30), true, FVector2D(5.f, 5.f));
	}
}


