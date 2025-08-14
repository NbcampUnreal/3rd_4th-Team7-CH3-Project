
#include "Component/LSInventoryComp.h"
#include "Character/LSPlayerCharacter.h"

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

void ULSInventoryComp::equip(const FName& Input)
{
	if (!MyItems.Contains(Input))	return;

	if (MyItems[Input]<=0)	return;
	MyItems[Input]--;

	MyWeaponName=Input;

	ChangeWeapon();
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
	
	MyWeaponName=TEXT("None");

	ChangeWeapon();
}

void ULSInventoryComp::ChangeWeaponSlot(const FName& NewWeapon)
{
	Unequip();
	
	MyWeaponName=NewWeapon;

	equip(MyWeaponName);
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

void ULSInventoryComp::ChangeWeapon()
{
	AActor* Owner=GetOwner();
	if (!Owner)	return;

	ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Owner);
	if (!Player)	return;

	Player->SetCurrentWeapon(ChangeWeaponNameToEnum(MyWeaponName));
	Player->Equip();
}


