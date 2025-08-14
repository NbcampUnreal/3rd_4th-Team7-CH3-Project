#include "Weapon/ReloadWeapon.h"
#include "Character/LSPlayerCharacter.h"
#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Weapon/LSWeaponBase.h"

void UReloadWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ALSPlayerCharacter* Character = Cast<ALSPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->WeaponSystemComp->CurrentWeapon->Reload();
	}
}
