
#include "Inven/LSInventoryComp.h"

ULSInventoryComp::ULSInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void ULSInventoryComp::BeginPlay()
{
	Super::BeginPlay();
}

void ULSInventoryComp::AddToInven(const FName& Input)
{
	MyItems.FindOrAdd(Input)++;
}

void ULSInventoryComp::ShowInven()
{
	if (MyItems.Num() ==0)	return;

	//check
	for (const auto& Pair : MyItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name:%s, Count: %d"),*Pair.Key.ToString(),Pair.Value);
		
	}
}
