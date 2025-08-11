#include "Weapon/LSPlayerWeaponSystemComp.h" 
#include "Engine/World.h" 
#include "GameFramework/Actor.h" 
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h" 
#include "Weapon/LSWeaponBase.h"
#include "Kismet/GameplayStatics.h"

ULSPlayerWeaponSystemComp::ULSPlayerWeaponSystemComp() 
{ 
	PrimaryComponentTick.bCanEverTick = false; 
	 
	CurrentWeapon = nullptr; 
	WeaponToSpawn = nullptr; 
} 

void ULSPlayerWeaponSystemComp::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(1);
}

 
void ULSPlayerWeaponSystemComp::EquipWeapon(int WeaponType) 
{
	UE_LOG(LogTemp, Error, TEXT("위치 체크"));
	// 기존 무기 제거 
	if (CurrentWeapon) 
	{ 
		CurrentWeapon->Destroy(); 
		CurrentWeapon = nullptr; 
	} 
 
	switch (WeaponType) 
	{ 
	case 1: 
		WeaponToSpawn = PistolClass; 
		break; 
	case 2: 
		WeaponToSpawn = ShotgunClass; 
		break; 
	case 3: 
		WeaponToSpawn = RifleClass; 
		break; 
	default: 
		return; 
	} 

	if (WeaponToSpawn) 
	{ 
		UWorld* World = GetWorld(); 
		if (!World) return; 
 
		AActor* OwnerActor = GetOwner(); 
		if (!OwnerActor) return; 
		 
		CurrentWeapon = World->SpawnActor<ALSWeaponBase>(WeaponToSpawn);
		if (!CurrentWeapon)
		{
			return;
		}
		if (CurrentWeapon) 
		{ 
			CurrentWeapon->SetOwner(OwnerActor); 
 
			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()); 
			if (OwnerCharacter && CurrentWeapon) 
			{ 
				USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh(); 
				if (MeshComp) 
				{ 
					CurrentWeapon->AttachToComponent( 
					MeshComp, 
					FAttachmentTransformRules::SnapToTargetIncludingScale, 
					FName("RightWeapon") 
					); 
				} 
			} 
		} 
	} 
} 

void ULSPlayerWeaponSystemComp::EquipPistol() 
{ 
	EquipWeapon(1);

	UE_LOG(LogTemp, Warning, TEXT("Input 1 & EquipPistol UpLoading!"));
} 

void ULSPlayerWeaponSystemComp::EquipShotgun() 
{ 
	EquipWeapon(2); 
} 

void ULSPlayerWeaponSystemComp::EquipRifle() 
{ 
	EquipWeapon(3); 
} 

void ULSPlayerWeaponSystemComp::FireWeapon()
{
	if (!CurrentWeapon) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return;

	FVector CameraLoc;
	FRotator CameraRot;
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	FVector TraceStart = CameraLoc;
	FVector TraceEnd = CameraLoc + (CameraRot.Vector() * 10000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	Params.AddIgnoredActor(CurrentWeapon);
	Params.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 5.0f);

	if (bHit && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			20.0f,
			PC,
			OwnerCharacter,
			nullptr
		);
	}
	
}