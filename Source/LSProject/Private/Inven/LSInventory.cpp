

#include "Inven/LSInventory.h"

ALSInventory::ALSInventory()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ALSInventory::AddToInven(const FName& Input)
{
	MyItems.FindOrAdd(Input)++;
}

void ALSInventory::ShowInven()
{
	if (MyItems.Num() ==0)	return;

	//check
	for (const auto& Pair : MyItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name:%s, Count: %d"),*Pair.Key.ToString(),Pair.Value);
		
	}
}

void ALSInventory::BeginPlay()
{
	Super::BeginPlay();
	
}


