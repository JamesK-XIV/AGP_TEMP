// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterHUD.h"

void UPlayerCharacterHUD::SetHealthBar(float HealthPercent)
{
	HealthBar->SetPercent(HealthPercent);
}

void UPlayerCharacterHUD::SetAmmo(float CurrentAmmo, float MaxAmmo)
{
	Ammo->SetText(FText::FromString(FString::FromInt(CurrentAmmo) + "/" + FString::FromInt(MaxAmmo)));
}
