// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSInventory.generated.h"

UCLASS()
class LSPROJECT_API ALSInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSInventory();

	//Actor->Weapon 변경하기
	void AddToInven(const FName& Input);
	//void AddItemToInven(ALSItem* Item);
	//void AddAmmoToInven(ALSAmmo* Ammo);
	//void AddAttachmentToInven(ALSAttachment* Attachment);

	//추후 삭제
	void ShowInven();
	
protected:
	virtual void BeginPlay() override;


private:
	//Actor->Weapon 변경하기
	TMap<FName,int32> MyItems;
	//TMap<ALSItem*,int32> MyItems;
	//TMap<ALSAttachment*,int32> MyAttachments;


	
	//추후 삭제
};
