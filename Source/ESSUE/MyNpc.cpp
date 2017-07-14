// Fill out your copyright notice in the Description page of Project Settings.

#include "MyNpc.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "MyHUD.h"


// Sets default values
AMyNpc::AMyNpc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProxSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProxSphere"));
	ProxSphere->AttachTo(RootComponent);
	ProxSphere->SetSphereRadius(100.f);
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyNpc::OnProxOverlapBegin);
	NpcMessage = TEXT("어서오세요. 플레이어!");
	
	GiveQuest = false;
}

// Called when the game starts or when spawned
void AMyNpc::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyNpc::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);
}

void AMyNpc::OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (_OtherComp != _OtherActor->GetRootComponent() || _OtherActor != Player)
		return;

	APlayerController* pController = GetWorld()->GetFirstPlayerController();

	AMyHUD* hud = Cast<AMyHUD>(pController->GetHUD());
	hud->AddMessageBox(Message(NpcName + TEXT(" : ") + NpcMessage, 5.f, FColor::White));

	if (true == GiveQuest)
		return;

	if (NpcName == TEXT("Jung"))
	{
		FString QuestName = TEXT("ADAM 처치!");
		FString TargetName = TEXT("ADAM");

		Player->AddQuest(QUEST(QuestName, TargetName, 5, 0));
		GiveQuest = true;
	}

	else
	{
		FString QuestName = TEXT("SWAT 처치!");
		FString TargetName = TEXT("SWAT");

		Player->AddQuest(QUEST(QuestName, TargetName, 5, 0));
		GiveQuest = true;
	}
}