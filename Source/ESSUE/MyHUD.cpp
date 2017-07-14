// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "MyPlayer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

void AMyHUD::DrawHUD(void)
{
	Super::DrawHUD();

	DrawMessageBox();
	DrawWidgetBox();
	DrawQuestBox();
	DrawStatus();
}

void AMyHUD::AddMessageBox(Message _Message)
{
	MessageBox.Add(_Message);
}

void AMyHUD::DrawMessageBox(void)
{
	for (int i = 0; i < MessageBox.Num(); ++i)
	{
		float Width, Height = 20.f;
		float Pad = 50.f;
		GetTextSize(MessageBox[i].String, Width, Height, HUDFont, 1.f);

		float x = 0.f;
		float y = 40.f * i;

		DrawText(MessageBox[i].String, MessageBox[i].Color, x + Pad, y + Pad, HUDFont);

		MessageBox[i].Time -= GetWorld()->GetDeltaSeconds();

		if (MessageBox[i].Time <= 0.f)
			MessageBox.RemoveAt(i);
	}
}

void AMyHUD::DrawWidgetBox(void)
{
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (false == MyPlayer->GetToggleInventory())
		return;

	DrawRect(FLinearColor(0.5f, 0.5f, 0.5, 1.f), 690.f, 90.f, 410.f, 210.f);

	for (int i = 0; i < WidgetBox.Num(); ++i)
	{
		FString String = WidgetBox[i].Icon.Name + FString::Printf(TEXT(" x %d"), WidgetBox[i].Icon.Count);
		DrawTexture(WidgetBox[i].Icon.Texture, WidgetBox[i].Pos.X, WidgetBox[i].Pos.Y, WidgetBox[i].Size.X, WidgetBox[i].Size.Y, 0.f, 0.f, 1.f, 1.f);
		DrawText(String, FLinearColor::Yellow, WidgetBox[i].Pos.X, WidgetBox[i].Pos.Y, HUDFont, 0.6f, false);
	}
}

void AMyHUD::DrawQuestBox(void)
{
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	TArray<Quest>* QuestList = MyPlayer->GetQuestList();


	float PosX = 950.f;
	float PosY = 120.f;
	float Margin = 40.f;

	for (int i = 0; i < QuestList->Num(); ++i)
	{
		DrawText((*QuestList)[i].QuestName, FLinearColor::Green, PosX, PosY * (i + 1), HUDFont, 1.f, false);
		DrawText((*QuestList)[i].TargetName + FString(TEXT(" óġ")), FLinearColor::White, PosX, PosY * (i + 1) + (Margin * 1), HUDFont, 0.7f, false);
		DrawText(FString::Printf(TEXT("%d / "), (*QuestList)[i].Count) + FString::Printf(TEXT("%d"), (*QuestList)[i].MaxCount), FLinearColor::White, PosX, PosY * (i + 1) + (Margin * 2), HUDFont, 0.7f, false);
	}
}

void AMyHUD::DrawStatus(void)
{
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	float Width = 200.f;
	float Height = 40.f;

	float Pad = 3.f;
	float Margin = 50.f;
	float HpPercent = FMath::Max(MyPlayer->GetHP() / MyPlayer->GetMaxHP(), 0.f);
	float MpPercent = FMath::Max(MyPlayer->GetMP() / MyPlayer->GetMaxMP(), 0.f);

	DrawRect(FLinearColor(0.5f, 0.5f, 0.5f, 1.f), Margin - Pad, Canvas->SizeY - Height - Pad - Margin * 2.f, Width + 2 * Pad, Height + 2 * Pad);
	DrawRect(FLinearColor(1.f - HpPercent, HpPercent, 0.f, 1.f), Margin, Canvas->SizeY - Height - Margin * 2.f, Width * HpPercent, Height);

	DrawRect(FLinearColor(0.5f, 0.5f, 0.5f, 1.f), Margin - Pad, Canvas->SizeY - Height - Pad - Margin, Width + 2 * Pad, Height + 2 * Pad);
	DrawRect(FLinearColor(0.f, 0.f, MpPercent, 1.f), Margin, Canvas->SizeY - Height - Margin, Width * MpPercent, Height);
}

void AMyHUD::AddWidgetBox(WIDGET _Widget)
{
	_Widget.Pos = FVector2D(700.f, 120.f);
	_Widget.Size = FVector2D(100.f, 100.f);
	float Pad = 20.f;

	int32 iX = WidgetBox.Num() % 4;
	int32 iY = WidgetBox.Num() / 4;

	_Widget.Pos.X += _Widget.Size.X * iX + Pad * iX;
	_Widget.Pos.Y += _Widget.Size.Y * iY + Pad * iY;

	WidgetBox.Add(_Widget);
}

void AMyHUD::ClearWidgetBox(void)
{
	WidgetBox.Empty();
}

void AMyHUD::MouseLClick(void)
{
	FVector2D Mouse;
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	pController->GetMousePosition(Mouse.X, Mouse.Y);
	PickedWidget = nullptr;
	
	for (int i = 0; i < WidgetBox.Num(); ++i)
	{
		if (true == WidgetBox[i].Click(Mouse))
		{
			PickedWidget = &WidgetBox[i];
			return;
		}
	}
}

void AMyHUD::MouseRClick(void)
{
	FVector2D Mouse;
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	pController->GetMousePosition(Mouse.X, Mouse.Y);

	for (int i = 0; i < WidgetBox.Num(); ++i)
	{
		if (true == WidgetBox[i].Click(Mouse))
		{
			AMyPlayer* MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			MyPlayer->DrinkPotion(WidgetBox[i].Icon.Name);
		}
	}
}

void AMyHUD::MouseDrag(void)
{
	static FVector2D LastMouse;
	FVector2D CurnnetMouse, DiffMouse;

	APlayerController* pController = GetWorld()->GetFirstPlayerController();

	pController->GetMousePosition(CurnnetMouse.X, CurnnetMouse.Y);
	DiffMouse = CurnnetMouse - LastMouse;
	
	float Time = pController->GetInputKeyTimeDown(EKeys::LeftMouseButton);
	if (Time > 0.f && nullptr != PickedWidget)
	{
		PickedWidget->Pos += DiffMouse;
	}

	LastMouse = CurnnetMouse;
}