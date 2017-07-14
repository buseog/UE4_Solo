// Fill out your copyright notice in the Description page of Project Settings.

#include "MyItem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyPlayer.h"
#include "MyHud.h"


// Sets default values
AMyItem::AMyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Name = TEXT("Unknown Item");
	Count = 0;
	Type = 0;
	ProxSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProxSphere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyItem::OnProxOverlapBegin);
	ProxSphere->AttachTo(Mesh);

}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyItem::OnProxOverlapBegin_Implementation(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (nullptr == Cast<AMyPlayer>(_OtherActor))
		return;

	AMyPlayer* MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	MyPlayer->AddInventory(this);
	APlayerController* pController = GetWorld()->GetFirstPlayerController();

	AMyHUD* hud = Cast<AMyHUD>(pController->GetHUD());
	hud->AddMessageBox(Message(Name + TEXT(" 을 얻었습니다."), 3.f, FColor::White));
	Destroy();
}