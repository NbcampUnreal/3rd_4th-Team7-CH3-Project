#include "Weapon/LSPlayerWeaponSystemComp.h" 
#include "Engine/World.h" 
#include "GameFramework/Actor.h" 
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h" 
#include "Weapon/LSWeaponBase.h"
#include "Weapon/LSWeaponPistol.h"
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

	if (!WeaponToSpawn) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	UWorld* World = GetWorld();
	if (!World) return;

	CurrentWeapon = World->SpawnActor<ALSWeaponBase>(WeaponToSpawn);
	if (!CurrentWeapon) return;

	CurrentWeapon->SetOwner(OwnerCharacter);
	if (USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh())
	{
		CurrentWeapon->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightWeapon"));
	}

} 


void ULSPlayerWeaponSystemComp::EquipPistol() 
{ 
	EquipWeapon(1);
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
	CurrentWeapon->OnFire();
}

bool ULSPlayerWeaponSystemComp::PerformLineTrace(float Damage, float FireRange, FHitResult& OutHit)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return false;

	UStaticMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<UStaticMeshComponent>();
	if (!WeaponMesh) return false;

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return false;

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
	FRotator MuzzleRotation = WeaponMesh->GetSocketRotation("Muzzle");

	FVector TraceStart = MuzzleLocation;
	FVector TraceEnd = TraceStart + (MuzzleRotation.Vector() * FireRange);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	if (CurrentWeapon) Params.AddIgnoredActor(CurrentWeapon);
	Params.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);
	
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 5, 1.0f);
	FVector MuzzleForwardVector = WeaponMesh->GetSocketTransform("Muzzle").GetUnitAxis(EAxis::X);
	DrawDebugCoordinateSystem(GetWorld(), MuzzleLocation, WeaponMesh->GetSocketRotation("Muzzle"), 20.f, false, 5.f);
	DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Green, false, 5.f, 0, 2.f);
#endif

	if (bHit && OutHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("라인트레이스 명중: %s"), *OutHit.GetActor()->GetName());
		
		UGameplayStatics::ApplyDamage(
			OutHit.GetActor(),
			Damage,
			PC,
			OwnerCharacter,
			nullptr
		);
	}

	return bHit;
	
}
