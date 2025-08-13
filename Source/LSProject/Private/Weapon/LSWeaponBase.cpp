#include "Weapon/LSWeaponBase.h"
#include "GameFramework/Actor.h"

ALSWeaponBase::ALSWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponScene"));
	SetRootComponent(Scene);
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponSkeletalMesh->SetupAttachment(Scene);

	Damage = 20.0f;
	FireRange = 100000.0f;
	Niagara = nullptr;
	
}

void ALSWeaponBase::Fire()
{
    // 총구 위치를 가져옵니다
    FVector MuzzleLocation = WeaponSkeletalMesh->GetSocketLocation(TEXT("Muzzle"));
    // 총구 방향을 가져옵니다
    FRotator MuzzleRotation = WeaponSkeletalMesh->GetSocketRotation(TEXT("Muzzle"));
    FVector Direction = MuzzleRotation.Vector();
    
    // 라인트레이스의 끝점을 계산합니다
    FVector EndLocation = MuzzleLocation + (Direction * FireRange);
    
    // 라인트레이스에 사용할 파라미터를 설정합니다
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 자기 자신은 무시
    QueryParams.AddIgnoredActor(GetOwner()); // 무기를 들고 있는 캐릭터도 무시
    
    // 라인트레이스를 실행합니다
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        MuzzleLocation,
        EndLocation,
        ECollisionChannel::ECC_Visibility,
        QueryParams
    );

	DrawDebugLine(
		GetWorld(),
		MuzzleLocation,
		EndLocation,
		FColor::Red,
		false,
		2.0f);
	
}

FTransform ALSWeaponBase::GetLeftHandSocketTransform() const
{
	return WeaponSkeletalMesh->GetSocketTransform(TEXT("LeftHandGrip"));
}