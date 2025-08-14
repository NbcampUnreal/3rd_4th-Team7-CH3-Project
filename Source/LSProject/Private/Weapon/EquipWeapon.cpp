#include "Weapon/EquipWeapon.h"
#include "Character/LSPlayerCharacter.h"
#include "Weapon/LSPlayerWeaponSystemComp.h"

void UEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ALSPlayerCharacter* Character = Cast<ALSPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->WeaponSystemComp->EquipWeapon();
	}
	
	
}
