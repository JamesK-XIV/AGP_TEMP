// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "../Characters/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

// NOTE: I am not setting bReplicates=true in the constructor of the WeaponPickup. I am setting it in the
// PickupBase as it will be the general rule to replicate all pickups. Setting it in a parent class will
// also mean that this actor is replicated.

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	// We are only generating the weapon rarity and the stats of the weapon pickup if it is on the server.
	if (GetLocalRole() == ROLE_Authority)
	{
		GenerateWeaponPickup();
	}
	// But we still want to update the material on the client based on the replicated values generated on the
	// server version of this weapon pickup.
	// NOTE: In a bad network, it is possible that the values will not have been replicated before the BeginPlay()
	// function is called. So you may need to set up ReplicatedUsing and also update the pickup material when the
	// weapon rarity value is changed. (For our purposes and for your assessments, you will be marked on perfect
	// network conditions but it is worth considering for future reference).
	UpdateWeaponPickupMaterial();
}

void AWeaponPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	//Super::OnPickupOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitInfo);
	// UE_LOG(LogTemp, Display, TEXT("Overlap event occurred on WeaponPickup"))

	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		Player->EquipWeapon(true, WeaponStats);
		Destroy();
	}
}

void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME is a macro defined by Unreal that takes the class name and the variable you want to replicate.
	// The variable you want to replicate also needs to be marked with UPROPERTY(Replicated).
	DOREPLIFETIME(AWeaponPickup, WeaponRarity);
	DOREPLIFETIME(AWeaponPickup, WeaponStats);
}

void AWeaponPickup::GenerateWeaponPickup()
{
	WeaponRarity = WeaponRarityPicker();
	TArray<bool> GoodStats;
	switch (WeaponRarity)
	{
	case EWeaponRarity::Legendary:
		GoodStats = WeaponStatPicker(4, 5);
		break;
	case EWeaponRarity::Master:
		GoodStats = WeaponStatPicker(3, 5);
		break;
	case EWeaponRarity::Rare:
		GoodStats = WeaponStatPicker(2, 5);
		break;
	default:
		GoodStats = WeaponStatPicker(0, 5);
		break;
	}

	WeaponStats.Accuracy = GoodStats[0] ? FMath::RandRange(0.98f, 1.0f) : FMath::RandRange(0.9f, 0.98f);
	WeaponStats.FireRate = GoodStats[1] ? FMath::RandRange(0.05f, 0.2f) : FMath::RandRange(0.2f, 1.0f);
	WeaponStats.BaseDamage = GoodStats[2] ? FMath::RandRange(15.0f, 30.0f) : FMath::RandRange(5.0f, 15.0f);
	WeaponStats.MagazineSize = GoodStats[3] ? FMath::RandRange(20, 100) : FMath::RandRange(1, 19);
	WeaponStats.ReloadTime = GoodStats[4] ? FMath::RandRange(0.1f, 1.0f) : FMath::RandRange(1.0f, 4.0f);
}

EWeaponRarity AWeaponPickup::WeaponRarityPicker()
{
	// Rules:
	// 50% chance of Common
	// 30% chance of Rare
	// 15% chance of Master
	// 5% chance of Legendary
	const float RandPercent = FMath::RandRange(0.0f, 1.0f);
	
	if (RandPercent <= 0.5f)
	{
		return EWeaponRarity::Common;
	}
	if (RandPercent <= 0.8f)
	{
		return EWeaponRarity::Rare;
	}
	if (RandPercent <= 0.95f)
	{
		return EWeaponRarity::Master;
	}
	
	return EWeaponRarity::Legendary;
}

TArray<bool> AWeaponPickup::WeaponStatPicker(int32 NumOfGood, int32 NumOfStats)
{
	// Fill the array with the correct number of good and bad stats.
	TArray<bool> GoodStats;
	for (int32 i = 0; i < NumOfStats; i++)
	{
		// Ternary condition: Will add true if I < NumOfGood otherwise add false.
		GoodStats.Add(i < NumOfGood ? true : false);
	}

	// Array shuffling algorithm.
	for (int32 i = 0; i < GoodStats.Num(); i++)
	{
		// Get a random index from the GoodStats array.
		const int32 RandIndex = FMath::RandRange(0, GoodStats.Num() - 1);
		// Then swap the item at that random index with the item in the i index.
		const bool Temp = GoodStats[i];
		GoodStats[i] = GoodStats[RandIndex];
		GoodStats[RandIndex] = Temp;
	}

	return GoodStats;
}
