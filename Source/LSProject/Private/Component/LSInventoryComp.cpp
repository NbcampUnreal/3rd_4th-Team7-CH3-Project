
#include "Component/LSInventoryComp.h"
#include "Character/LSPlayerCharacter.h"
#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Controller/LSPlayerController.h"
#include "Widget/LSInventoryWidget.h"
#include "Weapon/LSWeaponBase.h"

ULSInventoryComp::ULSInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	MyWeaponName=TEXT("None");
}

void ULSInventoryComp::BeginPlay()
{
	Super::BeginPlay();
}

void ULSInventoryComp::AddToInven(const FName& Input,int32 Amount)
{
	int32& NewValue=MyItems.FindOrAdd(Input);
	NewValue+=Amount;
}

int32 ULSInventoryComp::CountItem(const FName& Input)
{
	if (MyItems.Contains(Input))
	{
		return MyItems[Input];
	}

	return 0;
}

void ULSInventoryComp::Equip(const FName& Input)
{
	if (!MyItems.Contains(Input))	return;

	if (MyItems[Input]<=0)	return;
	MyItems[Input]--;

	MyWeaponName=Input;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	EquipWeapon();
}

void ULSInventoryComp::Unequip()
{
	if (int32* ExistingValue = MyItems.Find(MyWeaponName))
	{
		(*ExistingValue)++;
	}
	else
	{
		MyItems.Add(MyWeaponName, 1);
	}
	
	AActor* Owner = GetOwner();
	if (!Owner) return;

	ALSPlayerCharacter* Character=Cast<ALSPlayerCharacter>(Owner);
	if (!Character) return;

	ULSPlayerWeaponSystemComp* OtherComp = Character->FindComponentByClass<ULSPlayerWeaponSystemComp>();
	if (!OtherComp) return;
	
	//애니메이션
	EquipWeapon();

	//무기 삭제
	OtherComp->UnEquipWeapon();

	//무기 destroy 후 조정
	MyWeaponName=TEXT("None");
	
	//UI Update
	ALSPlayerController* PC=Cast<ALSPlayerController>(Character->GetController());
	if (!PC)	return;
	if (!PC->GetInvenWidget())	return;
	PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
	
}

void ULSInventoryComp::ChangeWeaponSlot(const FName& NewWeapon)
{
	Unequip();
	MyWeaponName=NewWeapon;
	Equip(MyWeaponName);
}

bool ULSInventoryComp::HasAmmo()
{
	FString AmmoStr=MyWeaponName.ToString();
	AmmoStr.Append("Ammo");
	FName Ammo=FName(*AmmoStr);

	if (MyItems.Find(Ammo) && MyItems[Ammo]>0)
	{
		return true;
	}

	return false;
}

int ULSInventoryComp::RequiredAmmo(int32 RequiredAmmo)
{
	FString AmmoStr=MyWeaponName.ToString();
	AmmoStr.Append("Ammo");
	FName AmmoName=FName(*AmmoStr);

	if (MyItems.Find(AmmoName))
	{
		if (MyItems[AmmoName]>=RequiredAmmo)
		{
			MyItems[AmmoName]-=RequiredAmmo;
			return	RequiredAmmo;
		}
		else
		{
			int32 Available = MyItems[AmmoName];
			MyItems[AmmoName]=0;
			return Available;
		}
	}

	return 0;
}

void ULSInventoryComp::AddAmmoToInven(int32 RequiredAmmo)
{
	FString AmmoStr=MyWeaponName.ToString();
	AmmoStr.Append("Ammo");
	FName AmmoName=FName(*AmmoStr);
	
	int32& NewValue=MyItems.FindOrAdd(AmmoName);
	NewValue+=RequiredAmmo;

	MyItems[AmmoName]=NewValue;
}

ECurrentWeapon ULSInventoryComp::ChangeWeaponNameToEnum(const FName& Input) 
{
	static const UEnum* EnumPtr = StaticEnum<ECurrentWeapon>();
	if (!EnumPtr)
	{
		return ECurrentWeapon::None;
	}

	int64 Value = EnumPtr->GetValueByName(Input);
	if (Value == INDEX_NONE)
	{
		return ECurrentWeapon::None;
	}

	return static_cast<ECurrentWeapon>(Value);
}

void ULSInventoryComp::EquipWeapon()
{
	AActor* Owner=GetOwner();
	if (!Owner)	return;

	ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Owner);
	if (!Player)	return;

	Player->SetCurrentWeapon(ChangeWeaponNameToEnum(MyWeaponName));
	Player->Equip();
	
}


