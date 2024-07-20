// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SPMainCharacter.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SPHealthComponent.h"
#include "Core/SPPlayerController.h"
#include "Core/SPPlayerState.h"
#include "Items/SPItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UMG/SPManagerWidget.h"
#include "Weapon/SPGrenade.h"




ASPMainCharacter::ASPMainCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<USPHealthComponent>("HealthComponent");
	if(HealthComponent)
	{
		HealthComponent->SetIsReplicated(true);
		OnDamageSignature.AddUObject(HealthComponent, &USPHealthComponent::ChangeHealth);
		HealthComponent->OnDeathSignature.AddUObject(this, &ASPMainCharacter::DeathCharacter);
	}
	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,
																  &ASPMainCharacter::OnBeginOverlapCapsule);
	}
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	if(Weapon)
	{
		Weapon ->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "AkWeapon");
	}
	
	
	OnTakeRadialDamage.AddDynamic(this, &ASPMainCharacter::TakeRadialDamage);
}

void ASPMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateHUD();
}

void ASPMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(StartRifleFireAction, ETriggerEvent::Triggered, this, &ThisClass::StartRifleFire);
		EnhancedInputComponent->BindAction(GrenadeLaunchAction, ETriggerEvent::Triggered, this, &ThisClass::GrenadeLaunch);
	}
}

void ASPMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPMainCharacter, GrenadeInHand);
	DOREPLIFETIME(ASPMainCharacter, CurrentGrenades);
	DOREPLIFETIME(ASPMainCharacter, CurrentRifleAmmo);
}

void ASPMainCharacter::GrenadeLaunch()
{
	Server_GrenadeLaunch();
}

void ASPMainCharacter::Server_GrenadeLaunch_Implementation()
{
	if(CurrentGrenades > 0)
	{
		//I'm tried to make a grenade, but i don't understand why ApplyRadialDamage(im talking about damage, not a function)
		//  not replicated. I screwed up a little bit :(
		FActorSpawnParameters param;
		param.Owner							 = this;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ASPGrenade* Grenade				 = GetWorld()->SpawnActor<ASPGrenade>(
			  GrenadeInHand, GetMesh()->GetSocketLocation("hand_r"), GetControlRotation(), param);
		if(Grenade)
		{
			Grenade->StartFire();
			CurrentGrenades -= 1;
		}
	}
}

void ASPMainCharacter::StartRifleFire()
{
	Server_StartRifleFire();
}

void ASPMainCharacter::Server_StartRifleFire_Implementation()
{
	if(CurrentRifleAmmo > 0)
	{
		//Create LineTrace for Shooting
		FVector Start = Weapon->GetSocketLocation("Fire");
		FVector ForwardVector = Weapon->GetForwardVector();
		FVector End = Start + (ForwardVector * 1000.0f); 

		FHitResult HitResult;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

	
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,
			ECC_Visibility, CollisionParams);

	
		DrawDebugLine( GetWorld(), Start, End, FColor::Green,false,
				1,0,1);

		if (bHit)
		{
			// If we hit something, draw a red point
			DrawDebugPoint( GetWorld(), HitResult.Location,10, FColor::Red,false,1 );

			if(ASPMainCharacter* Enemy = Cast<ASPMainCharacter>(HitResult.GetActor()))
			{
				UGameplayStatics::ApplyRadialDamage(this, RifleInfo.Damage, HitResult.Location,
					RifleInfo.InnerRadius, RifleInfo.DamageType, {}, nullptr);
			
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());

				if(ASPPlayerController* EnemyPC = Cast<ASPPlayerController>(Enemy->GetController()))
				{
					if(EnemyPC->IsDead == true)
					{
						GetPlayerState<ASPPlayerState>()->AddKill();
						EnemyPC->GetPlayerState<ASPPlayerState>()->AddDead();
						EnemyPC->GetPlayerState<ASPPlayerState>()->IsMeDead = false;
					}
				}
			}
		}
		CurrentRifleAmmo -= 1;
	}
}

void ASPMainCharacter::DeathCharacter()
{
	if(ASPPlayerController* PC = Cast<ASPPlayerController>(GetController()))
	{
		PC->OnDeath();
	}

	Multi_Death();
}

void ASPMainCharacter::Multi_Death_Implementation()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(!HasAuthority())
	{
		GetMesh()->SetSimulatePhysics(true);
		if(GetController())
		{
			RemoveHUD();
		}
	}
	SetLifeSpan(5.f);
}

void ASPMainCharacter::RemoveHUD()
{
	if(MainHUD)
	{
		MainHUD->RemoveFromParent();
	}
}

void ASPMainCharacter::CreateHUD()
{
	if(HUDClassWidget)
	{
		if(APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			MainHUD = CreateWidget<USPManagerWidget>(PC, HUDClassWidget);
			if(!MainHUD)
			{
				return;
			}

			MainHUD->AddToViewport();
		}
	}
}

void ASPMainCharacter::OnBeginOverlapCapsule(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
													  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
													  bool bBFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		if(ASPItemBase* Item = Cast<ASPItemBase>(OtherActor))
		{
			if(Item->ItemStruct.WeaponType == "Grenade")
			{
				GrenadeInHand			= Item->ItemStruct.ItemClass;
				int32 NewGrenadesValue	= Item->ItemStruct.Amount + CurrentGrenades;
				CurrentGrenades			= UKismetMathLibrary::Clamp(NewGrenadesValue, 1, MaxGrenades);
			}
			if(Item->ItemStruct.WeaponType == "Rifle")
			{
				int32 NewAmmoValue		= Item->ItemStruct.Amount + CurrentRifleAmmo;
				CurrentRifleAmmo	= UKismetMathLibrary::Clamp(NewAmmoValue, 1, MaxRifleAmmo);
			}
		}
	}
}

void ASPMainCharacter::TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
												 FVector Origin, FHitResult HitInfo, AController* InstigatedBy,
												 AActor* DamageCauser)
{
	if(HasAuthority() && !GetHealthComponent()->GetIsDead())
	{
		if(OnDamageSignature.IsBound())
		{
			OnDamageSignature.Broadcast(Damage);
		}
	}
}


void ASPMainCharacter::OnRep_CurrentAmountGrenades()
{
	if(!HasAuthority())
	{
		if(OnFireGrenadeSignature.IsBound())
		{
			OnFireGrenadeSignature.Broadcast(CurrentGrenades);
		}
	}
}

void ASPMainCharacter::OnRep_CurrentAmountRifleAmmo()
{
	if(!HasAuthority())
	{
		if(OnFireRifleSignature.IsBound())
		{
			OnFireRifleSignature.Broadcast(CurrentRifleAmmo);
		}
	}
}
