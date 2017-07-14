// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

typedef struct Quest
{
	FString QuestName;
	FString TargetName;
	int		Count;
	int		MaxCount;
	int		Type;

	Quest(void)
	{
		QuestName = TEXT("Unknown Quest");
		TargetName = TEXT("Unknown Name");
		Count = 0;
		MaxCount = 0;
		Type = 0;
	}

	Quest(FString _QuestName, FString _TargetName, int _MaxCount, int _Type)
	{
		QuestName = _QuestName;
		TargetName = _TargetName;
		Count = 0;
		MaxCount = _MaxCount;
		Type = _Type;
	}

}QUEST;

typedef struct Message
{
	FString String;
	float	Time;
	FColor	Color;

	Message(void)
	{
		Time = 5.f;
		Color = FColor::White;
	}
	Message(FString _String, float _Time, FColor _Color)
	{
		String = _String;
		Time = _Time;
		Color = _Color;
	}
}Message;

typedef struct Icon
{
	FString Name;
	int32 Count;
	UTexture2D* Texture;

	Icon(void)
	{
		Name = TEXT("Unknown Item");
		Texture = nullptr;
	}

	Icon(FString& _Name, int32 _Count, UTexture2D* _Texture)
	{
		Name = _Name;
		Count = _Count;
		Texture = _Texture;
	}
}ICON;

typedef struct Widget
{
	ICON Icon;
	FVector2D Pos;
	FVector2D Size;

	Widget(ICON _Icon)	{ Icon = _Icon;	}

	float Left(void) { return Pos.X; }
	float Right(void) { return Pos.X + Size.X; }
	float Top(void) { return Pos.Y; }
	float Bottom(void) { return Pos.Y + Size.Y; }

	bool Click(FVector2D Mouse)
	{
		return Mouse.X >= Left() && Mouse.X <= Right() && Mouse.Y >= Top() && Mouse.Y <= Bottom();
	}
}WIDGET;
/**
 * 
 */
UCLASS()
class ESSUE_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	TArray<Message> MessageBox;
	TArray<WIDGET>	WidgetBox;

	WIDGET*			PickedWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
	UFont* HUDFont;

private:
	void DrawMessageBox(void);
	void DrawWidgetBox(void);
	void DrawQuestBox(void);
	void DrawStatus(void);

public:
	// 원소 추가
	void AddMessageBox(Message _Message);
	void AddWidgetBox(WIDGET _Widget);

	// 클리어
	void ClearWidgetBox(void);

	// 이벤트
	void MouseLClick(void);
	void MouseRClick(void);
	void MouseDrag(void);

public:
	virtual void DrawHUD(void) override;
};