// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"

void AMyGameMode::StartPlay(void)
{
	Super::StartPlay();

	if (GEngine)
	{
		// ����� �޽��� ǥ��
		// Ű���� -1�� �ָ� �޽����� ������Ʈ�ϰų� ���ΰ�ħ ���� ����.
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("AA"), true, FVector2D(5.f, 5.f));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%d"), 30), true, FVector2D(5.f, 5.f));
	}
}


