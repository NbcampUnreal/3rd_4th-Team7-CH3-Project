#include "Weapon/LSWeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Character/LSPlayerCharacter.h"
#include "Enemy/LSEnemy.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Component/LSInventoryComp.h"
#include "Controller/LSPlayerController.h"
#include "Widget/LSInventoryWidget.h"

ALSWeaponBase::ALSWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponScene"));
	SetRootComponent(Scene);
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponSkeletalMesh->SetupAttachment(Scene);

	MaxAmmo = 10;
	CurrentAmmo = 10;

	FireRange = 100000.0f;
	Damage = 20.0f;
	FireRate = 0.1f;
	FireEffect = nullptr;
	FireSound = nullptr;
	FireSoundVolume = 1.0f;
	ReloadSound = nullptr;
	ReloadSoundVolume = 1.0f;
}

void ALSWeaponBase::Fire()
{
	if (ALSPlayerCharacter* Character = Cast<ALSPlayerCharacter>(GetOwner()))
	{
		CurrentAmmo--;
		if (CurrentAmmo < 0)
		{
			CurrentAmmo = 0;
		}

		FVector CameraLocation = Character->Camera->GetComponentLocation();
		FVector CameraDirection = Character->Camera->GetForwardVector();

		FVector EndLocation = CameraLocation + (CameraDirection * FireRange);
		FVector StartLocation = CameraLocation + (CameraDirection * 200.0f);;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(Character);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Pawn,
			QueryParams);

		ALSEnemy* HitEnemy = Cast<ALSEnemy>(HitResult.GetActor());

		if (bHit && HitEnemy)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Zombie"));
			UGameplayStatics::ApplyDamage(
				HitEnemy,
				Damage,
				nullptr,
				this,
				UDamageType::StaticClass());
		}

		if (FireEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				FireEffect,
				WeaponSkeletalMesh,
				TEXT("Muzzle"),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true);
		}

		DrawDebugLine(
			GetWorld(),
			StartLocation,
			EndLocation,
			FColor::Red,
			false,
			2.0f);
	}
}

void ALSWeaponBase::Reload()
{
	//우진->
	ALSPlayerCharacter* Player = Cast<ALSPlayerCharacter>(GetOwner());
	if (!Player)	return;

	ULSInventoryComp* Inven=Player->FindComponentByClass<ULSInventoryComp>();
	if (!Inven)	return;

	CurrentAmmo+=Inven->RequiredAmmo(MaxAmmo-CurrentAmmo);

	//UI Update
	ALSPlayerController* PC=Cast<ALSPlayerController>(Player->GetController());
	if (!PC)	return ;
	if (!PC->GetInvenWidget())	return ;
	ULSInventoryWidget* InvenUI=Cast<ULSInventoryWidget>(PC->GetInvenWidget());
	InvenUI->OnUpdateInvenUI.Broadcast();
	
	//CurrentAmmo = MaxAmmo;
}

float ALSWeaponBase::GetFireRate() const
{
	return FireRate;
}

int32 ALSWeaponBase::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 ALSWeaponBase::GetMaxAmmo() const
{
	return MaxAmmo;
}

bool ALSWeaponBase::IsCanReload() const
{
	return CurrentAmmo < MaxAmmo;
}

bool ALSWeaponBase::IsCanFire() const
{
	return CurrentAmmo > 0;
}