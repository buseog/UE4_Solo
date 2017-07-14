// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "MyItem.h"
#include "MyWeapon.h"
#include "Mybullet.h"
#include "MySpell.h"
#include "MyEffect.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimSequence.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = IDLE;
	MotionDelay = 0.f;
	StepEffectDelay = 0.f;
	MouseSpeed = 50.f;
	CurrentWeapon = nullptr;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	Hud = Cast<AMyHUD>(pController->GetHUD());
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KnockBack();

	StepEffectDelay -= DeltaTime;

	MotionDelay -= DeltaTime;

	if (MotionDelay <= 0.f && IDLE != CurrentState)
	{
		CurrentState = IDLE;
		GetMesh()->PlayAnimation(Animation[CurrentState], true);

		if (nullptr != CurrentWeapon)
			CurrentWeapon->Rest();
	}
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ¿Ãµø
	PlayerInputComponent->BindAxis("Forward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Side", this, &AMyPlayer::MoveSide);

	// »≠∏È»∏¿¸
	PlayerInputComponent->BindAxis("Yaw", this, &AMyPlayer::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AMyPlayer::Pitch);

	// L≈¨∏Ø
	PlayerInputComponent->BindAction("MouseLB", IE_Pressed, this, &AMyPlayer::MouseLClick);
	
	// R≈¨∏Ø
	PlayerInputComponent->BindAction("MouseRB", IE_Pressed, this, &AMyPlayer::MouseRClick);

	// ¡°«¡
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyPlayer::StopJump);

	// ¿Œ∫•≈‰∏Æ
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyPlayer::ToggleInventory);

	// ¿Â∫Ò
	PlayerInputComponent->BindAction("EquipSword", IE_Pressed, this, &AMyPlayer::EquipSword);
	PlayerInputComponent->BindAction("EquipGun", IE_Pressed, this, &AMyPlayer::EquipGun);
}

void AMyPlayer::PostInitializeComponents(void)
{
	Super::PostInitializeComponents();

	int32 WeaponListCount = BPWeaponList.Num();

	for (int i = 0; i < WeaponListCount; ++i)
	{
		AMyWeapon* Weapon = GetWorld()->SpawnActor<AMyWeapon>(BPWeaponList[i]);

		if (Weapon->Type == FString(TEXT("Sword")))
			Weapon->SetOwningPawn(this, TEXT("BackAttach"));

		else
			Weapon->SetOwningPawn(this, TEXT("WaistAttach"));

		Weapon->OnEquip();
		AddWeapon(Weapon);
	}
}

float AMyPlayer::TakeDamage(float _Damage, struct FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	Super::TakeDamage(_Damage, _DamageEvent, _EventInstigator, _DamageCauser);

	KnockBackPower = GetActorLocation() - _DamageCauser->GetActorLocation();
	KnockBackPower.Normalize();
	KnockBackPower *= _Damage * 500.f;

	HP -= _Damage;

	if (HP <= 0.f)
		HP = MaxHP;

	return HP;
}

void AMyPlayer::KnockBack(void)
{
	AddMovementInput(KnockBackPower, 1.f);
	KnockBackPower *= 0.5f;
}

void AMyPlayer::EquipSword(void)
{
	if (CurrentWeapon == WeaponList[SWORD])
		return;

	if (nullptr == CurrentWeapon)
	{
		CurrentWeapon = WeaponList[SWORD];
		CurrentWeapon->SetAttachPoint(FString(TEXT("RightHandAttach")));
		CurrentWeapon->OnEquip();
	}
	else
	{
		// ≈ª¬¯
		CurrentWeapon->SetAttachPoint(FString(TEXT("WaistAttach")));
		CurrentWeapon->OnEquip();
		// ¿Â¬¯
		CurrentWeapon = WeaponList[SWORD];
		CurrentWeapon->SetAttachPoint(FString(TEXT("RightHandAttach")));
		CurrentWeapon->OnEquip();
	}
}

void AMyPlayer::EquipGun(void)
{
	if (CurrentWeapon == WeaponList[GUN])
		return;

	if (nullptr == CurrentWeapon)
	{
		CurrentWeapon = WeaponList[GUN];
		CurrentWeapon->SetAttachPoint(FString(TEXT("RightHandAttach2")));
		CurrentWeapon->OnEquip();
	}
	else
	{
		// ≈ª¬¯
		CurrentWeapon->SetAttachPoint(FString(TEXT("BackAttach")));
		CurrentWeapon->OnEquip();
		// ¿Â¬¯
		CurrentWeapon = WeaponList[GUN];
		CurrentWeapon->SetAttachPoint(FString(TEXT("RightHandAttach2")));
		CurrentWeapon->OnEquip();
	}
}

void AMyPlayer::OnSword(void)
{
	if (CurrentState != RUN && MotionDelay >= 0.f)
		return;

	CurrentState = SLASH;
	GetMesh()->PlayAnimation(Animation[CurrentState], false);
	GetMesh()->SetPosition(0.f);
	MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();

	CurrentWeapon->Attack();
}

void AMyPlayer::OnShot(void)
{
	if (CurrentState != RUN && MotionDelay >= 0.f)
		return;

	CurrentState = SHOOT;
	GetMesh()->PlayAnimation(Animation[CurrentState], false);
	GetMesh()->SetPosition(0.f);
	MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();

	FRotator Rotation = GetActorRotation();
	FVector Location = GetActorLocation() + Rotation.RotateVector(FVector(150.f, 10.f, 50.f));

	AMyBullet* Bullet = GetWorld()->SpawnActor<AMyBullet>(BPBullet, Location, Rotation);
	Bullet->SetOwnerType(AMyBullet::OwnerID::PLAYER);

	// ¿Ã∆Â∆Æ
	if (nullptr == BPEffectList[1])
		return;

	AMyEffect* Effect = GetWorld()->SpawnActor<AMyEffect>(BPEffectList[1], Location, Rotation);
}

void AMyPlayer::OnCastSpell(void)
{
	if (nullptr == CurrentWeapon)
		return;

	if (CurrentWeapon->Type == FString(TEXT("Sword")))
	{
		if (nullptr == BPSpellList[SWORD])
			return;

		if (CurrentState != RUN && MotionDelay >= 0.f)
			return;

		CurrentState = SLASH;
		GetMesh()->PlayAnimation(Animation[CurrentState], false);
		GetMesh()->SetPosition(0.f);
		MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();

		FVector Location = GetActorLocation() + FVector(0.f, 0.f, -70.f);

		AMySpell* Spell = GetWorld()->SpawnActor<AMySpell>(BPSpellList[SWORD], Location, FRotator(0));

		if (nullptr == Spell)
			return;

		Spell->SetCaster(this);
	}
	else
	{
		if (nullptr == BPSpellList[GUN])
			return;

		if (CurrentState != RUN && MotionDelay >= 0.f)
			return;

		CurrentState = SHOOT;
		GetMesh()->PlayAnimation(Animation[CurrentState], false);
		GetMesh()->SetPosition(0.f);
		MotionDelay = Animation[CurrentState]->GetMaxCurrentTime();

		FRotator Rotation = GetActorRotation();
		FVector Location = GetActorLocation() + Rotation.RotateVector(FVector(300.f, 30.f, 0.f));

		AMySpell* Spell = GetWorld()->SpawnActor<AMySpell>(BPSpellList[GUN], Location, GetActorRotation());

		if (nullptr == Spell)
			return;

		Spell->SetCaster(this);
	}
}

void AMyPlayer::MoveForward(float _Amount)
{
	if (CurrentState >= SLASH)
		return;

	if (Controller && _Amount)
	{
		if (CurrentState != RUN && CurrentState != JUMP)
		{
			CurrentState = RUN;
			GetMesh()->PlayAnimation(Animation[CurrentState], true);
			GetMesh()->SetPosition(0.f);
		}
		
		if (CurrentState != JUMP)
			MotionDelay = 0.1f;

		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, _Amount);

		// ¿Ã∆Â∆Æ
		if (nullptr == BPEffectList[0])
			return;
		
		if (StepEffectDelay <= 0.f && JUMP != CurrentState)
		{
			FVector Location = GetActorLocation();
			Location.Z -= 70.f;

			AMyEffect* Effect = GetWorld()->SpawnActor<AMyEffect>(BPEffectList[0], Location, FRotator(0));
			StepEffectDelay = 0.1f;
		}
	}
}

void AMyPlayer::MoveSide(float _Amount)
{
	if (CurrentState >= SLASH)
		return;

	if (Controller && _Amount)
	{
		if (CurrentState != RUN && CurrentState != JUMP)
		{
			CurrentState = RUN;
			GetMesh()->PlayAnimation(Animation[CurrentState], true);
			GetMesh()->SetPosition(0.f);
		}

		if (CurrentState != JUMP)
			MotionDelay = 0.1f;

		FVector right = GetActorRightVector();
		AddMovementInput(right, _Amount);

		// ¿Ã∆Â∆Æ
		if (nullptr == BPEffectList[0])
			return;

		if (StepEffectDelay <= 0.f && JUMP != CurrentState)
		{
			FVector Location = GetActorLocation();
			Location.Z -= 70.f;

			AMyEffect* Effect = GetWorld()->SpawnActor<AMyEffect>(BPEffectList[0], Location, FRotator(0));
			StepEffectDelay = 0.1f;
		}
	}
}

void AMyPlayer::Yaw(float _Amount)
{
	// µÂ∑°±◊ ±‚¥… ¡¶∞≈
	if (true == ShowInventory)
		return;// Hud->MouseDrag();

	else
		AddControllerYawInput(MouseSpeed * _Amount * GetWorld()->GetDeltaSeconds());
}

void AMyPlayer::Pitch(float _Amount)
{
	if (true == ShowInventory)
		return;//Hud->MouseDrag();

	else
		AddControllerPitchInput(MouseSpeed * _Amount * GetWorld()->GetDeltaSeconds());
}

void AMyPlayer::MouseLClick(void)
{
	StartAttack();
}

void AMyPlayer::MouseRClick(void)
{
	if (true == ShowInventory)
		InventoryPicking();

	else
		OnCastSpell();
}

void AMyPlayer::InventoryPicking(void)
{
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	AMyHUD* hud = Cast<AMyHUD>(pController->GetHUD());
	hud->MouseRClick();
}

void AMyPlayer::StartAttack(void)
{
	if (nullptr == CurrentWeapon)
		return;

	if (CurrentWeapon->Type == FString(TEXT("Sword")))
		OnSword();

	else
		OnShot();
}

void AMyPlayer::StartJump(void)
{
	if (CurrentState >= SLASH)
		return;

	CurrentState = JUMP;
	GetMesh()->PlayAnimation(Animation[CurrentState], false);
	GetMesh()->SetPosition(0.f);
	MotionDelay = 0.8f;

	bPressedJump = true;
}

void AMyPlayer::StopJump(void)
{
	bPressedJump = false;
}

void AMyPlayer::ToggleInventory(void)
{
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	AMyHUD* hud = Cast<AMyHUD>(pController->GetHUD());

	if (true == ShowInventory)
	{
		hud->ClearWidgetBox();
		ShowInventory = false;
		pController->bShowMouseCursor = false;
	}
	else
	{
		ShowInventory = true;
		pController->bShowMouseCursor = true;

		for (TMap<FString, AMyItem*>::TIterator iter = Inventory.CreateIterator(); iter; ++iter)
		{
			UTexture2D* Texture = iter->Value->Icon;
			hud->AddWidgetBox(WIDGET(ICON(iter->Key, iter->Value->Count, Texture)));
		}
	}
}

void AMyPlayer::RefreshInventory(void)
{
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	AMyHUD* hud = Cast<AMyHUD>(pController->GetHUD());

	hud->ClearWidgetBox();

	for (TMap<FString, AMyItem*>::TIterator iter = Inventory.CreateIterator(); iter; ++iter)
	{
		UTexture2D* Texture = iter->Value->Icon;
		hud->AddWidgetBox(WIDGET(ICON(iter->Key, iter->Value->Count, Texture)));
	}
}

void AMyPlayer::AddInventory(AMyItem* _Item)
{
	if (Inventory.Find(_Item->Name))
		Inventory[_Item->Name]->Count += _Item->Count;

	else
		Inventory.Add(_Item->Name, _Item);
}

void AMyPlayer::AddWeapon(AMyWeapon* _Weapon)
{
	if (nullptr == _Weapon)
		return;

	WeaponList.AddUnique(_Weapon);
}

void AMyPlayer::DrinkPotion(FString _ItemName)
{
	if (Inventory[_ItemName]->Type == 0)
	{
		HP += 5000.f;

		if (HP >= MaxHP)
			HP = MaxHP;

		--Inventory[_ItemName]->Count;

		if (Inventory[_ItemName]->Count <= 0)
			Inventory.Remove(_ItemName);

		// ¿Ã∆Â∆Æ
		if (nullptr == BPEffectList[2])
			return;

		AMyEffect* Effect = GetWorld()->SpawnActor<AMyEffect>(BPEffectList[2], GetActorLocation(), GetActorRotation());
	}
	else
	{
		MP += 5000.f;

		if (MP >= MaxMP)
			MP = MaxMP;

		--Inventory[_ItemName]->Count;

		if (Inventory[_ItemName]->Count <= 0)
			Inventory.Remove(_ItemName);

		// ¿Ã∆Â∆Æ
		if (nullptr == BPEffectList[3])
			return;

		FVector Location = GetActorLocation() - FVector(0.f, 0.f, 100.f);

		AMyEffect* Effect = GetWorld()->SpawnActor<AMyEffect>(BPEffectList[3], Location, GetActorRotation());
	}

	RefreshInventory();
}

void AMyPlayer::AddQuest(QUEST _Quest)
{
	for (int i = 0; i < QuestList.Num(); ++i)
	{
		if (QuestList[i].QuestName == _Quest.QuestName)
			return;
	}

	QuestList.Add(_Quest);
}

void AMyPlayer::RenewalQuest(FString _Name)
{
	for (int i = 0; i < QuestList.Num(); ++i)
	{
		if (QuestList[i].TargetName == _Name)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ƒ˘Ω∫∆Æ ∞ªΩ≈!"), true, FVector2D(4.f, 4.f));
			++QuestList[i].Count;

			if (QuestList[i].Count >= QuestList[i].MaxCount)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ƒ˘Ω∫∆Æ øœ∑·!"), true, FVector2D(5.f, 5.f));
				QuestList.RemoveAt(i);

				// ¿Ã∆Â∆Æ
				if (nullptr == BPEffectList[4])
					return;

				FVector Location = GetActorLocation() + FVector(0.f, 0.f, -40.f);

				AMyEffect* Effect = GetWorld()->SpawnActor<AMyEffect>(BPEffectList[4], Location, FRotator(0));
			}
			break;
		}
	}
}