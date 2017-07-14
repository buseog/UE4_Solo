// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyHUD.h"
#include "GameFramework/Character.h"
#include "MyPlayer.generated.h"



class AMyItem;
class AMyWeapon;
class AMySpell;
class AMyEffect;
class AMyBullet;

UCLASS()
class ESSUE_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents(void) override;
	virtual float TakeDamage(float _Damage, struct FDamageEvent const& _DamageEvent, class AController* _EventInstigator, class AActor* _DamageCauser) override;

private:
	enum AnimationID { IDLE, RUN, JUMP, SLASH, SHOOT };
	enum WeaponID { SWORD, GUN };

private:
	TArray<QUEST>	QuestList;
	TMap<FString, AMyItem*> Inventory;
	TArray<AMyWeapon*> WeaponList;
	AMyWeapon*	CurrentWeapon;

	AMyHUD* Hud;
	FVector KnockBackPower;
	bool	ShowInventory;
	int		CurrentState;
	float	MotionDelay;
	float	StepEffectDelay;

public: // ������ ������� public..
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PLAYER)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PLAYER)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PLAYER)
	float MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PLAYER)
	float MaxMP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PLAYER)
	float MouseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PLAYER)
	TSubclassOf<AMyBullet> BPBullet;

	UPROPERTY(EditDefaultsOnly, Category = PLAYER)
	TArray<TSubclassOf<AMyWeapon>> BPWeaponList;

	UPROPERTY(EditDefaultsOnly, Category = PLAYER)
	TArray<TSubclassOf<AMySpell>> BPSpellList;

	UPROPERTY(EditDefaultsOnly, Category = PLAYER)
	TArray<TSubclassOf<AMyEffect>> BPEffectList;

	UPROPERTY(EditDefaultsOnly, Category = PLAYER)
	TArray<UAnimSequence*> Animation;

private:
	void KnockBack(void);

	void EquipSword(void);
	void EquipGun(void);

	void OnSword(void);
	void OnShot(void);
	void OnCastSpell(void);

public: // �� & ��
	inline float GetHP(void) { return HP; }
	inline float GetMaxHP(void) { return MaxHP; }
	inline float GetMP(void) { return MP; }
	inline float GetMaxMP(void) { return MaxMP; }
	inline bool	GetToggleInventory(void) { return ShowInventory;	}
	inline USkeletalMeshComponent* GetSkeletalMeshComponent(void) const { return GetMesh(); }
	inline TArray<QUEST>* GetQuestList(void) { return &QuestList; }

public: // �ܺο��� �Ҹ� �Լ�
	// �̵��Լ�
	void MoveForward(float _Amount);
	void MoveSide(float _Amount);

	// ���콺 ������ �Լ�
	void Yaw(float _Amount);
	void Pitch(float _Amount);

	// ���콺 Ŭ��
	void MouseLClick(void);
	void MouseRClick(void);
	void InventoryPicking(void);
	void StartAttack(void);

	// ���� �Լ�
	void StartJump(void);
	void StopJump(void);
	
	// �κ��丮 �Լ�
	void ToggleInventory(void);
	void RefreshInventory(void);
	void AddInventory(AMyItem* _Item);
	void AddWeapon(AMyWeapon* _Weapon);

	// ����
	void DrinkPotion(FString _ItemName);

	// ����Ʈ
	void AddQuest(QUEST _Quest);
	void RenewalQuest(FString _Name);
};
