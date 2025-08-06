// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/LSWeaponPistol.h"
#include "Weapon/LSWeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

ALSWeaponPistol::ALSWeaponPistol()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshAsset(TEXT("/Game/Weapons/Pistol/Mesh/SM_Pistol.SM_Pistol"));
	if (StaticMeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshAsset.Object);

		UE_LOG(LogTemp, Warning, TEXT("물체 생성"));
	}

	Damage = 20.0f;
	FireRange = 100000.0f;
}

void ALSWeaponPistol::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(WeaponMappingContext, 1);
			}
		}

		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			EIC->BindAction(IA_EquipPistol, ETriggerEvent::Triggered, this, &ALSWeaponPistol::OnEquip);
			EIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ALSWeaponPistol::OnFire);
		}

		UE_LOG(LogTemp, Warning, TEXT("바인딩"));
	}
}

void ALSWeaponPistol::OnEquip(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Equip 부르기"));
	EquipWeapon();
}

void ALSWeaponPistol::OnFire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("사격"));
	PerformLineTrace();
}


void ALSWeaponPistol::EquipWeapon()
{
	if (ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner()))
	{
		AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightWeapon"));
		UE_LOG(LogTemp, Warning, TEXT("장착"));
	}
}

void ALSWeaponPistol::PerformLineTrace()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector ShotDirection = EyeRotation.Vector();
	FVector TraceEnd = EyeLocation + (ShotDirection * FireRange);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;

	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		EyeLocation,
		TraceEnd,
		ECC_Visibility,
		Params	
		)
	)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, nullptr);
		}

		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}

		TraceEnd = Hit.ImpactPoint;
	}

	
	PlayFireEffects(TraceEnd);
}

void ALSWeaponPistol::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, StaticMesh, TEXT("MuzzleSocket"));
	}
	DrawDebugPoint(GetWorld(), TraceEnd, 10.0f, FColor::Red, true); // test용이래!
}

	



