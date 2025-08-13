
#include "Component/LSInventoryComp.h"

ULSInventoryComp::ULSInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void ULSInventoryComp::BeginPlay()
{
	Super::BeginPlay();
}

void ULSInventoryComp::AddToInven(const FName& Input,int32 Amount)
{
	int32& NewValue=MyItems.FindOrAdd(Input);
	NewValue=NewValue+Amount;
	
	ShowInven();
}

int32 ULSInventoryComp::CountItem(const FName& Input)
{
	if (MyItems.Contains(Input))
	{
		return MyItems[Input];
	}

	return 0;
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
