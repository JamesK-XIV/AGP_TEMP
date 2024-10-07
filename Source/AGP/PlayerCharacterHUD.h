// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerCharacterHUD.generated.h"


/**
 * 
 */
UCLASS()
class AGP_API UPlayerCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
    UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* Ammo;

public:
	void SetHealthBar(float HealthPercent);
	void SetAmmo(float CurrentAmmo, float MaxAmmo);
};
