// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPItemBase.generated.h"

class ASPGrenade;

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText NameItem = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;

	// In the future, it is possible to expand to use the inventory system
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASPGrenade> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponType = "Grenade";
};

UCLASS()
class SP_API ASPItemBase : public AActor
{
	GENERATED_BODY()

public:
	ASPItemBase();
	
	UPROPERTY(EditAnywhere)
	FItemStruct ItemStruct;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep,
								 const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* ParticleSystem;

};
