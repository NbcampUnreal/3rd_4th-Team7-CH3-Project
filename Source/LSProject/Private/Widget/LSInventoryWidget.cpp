// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LSInventoryWidget.h"
#include "Character/LSPlayerCharacter.h"
#include "Component/LSInventoryComp.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "DataTable/LSInvenRow.h"
#include "Widget/LSInvenSlot.h"
#include "Components/Image.h"

void ULSInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LayOutEmptyInven();
	LayoutInInven();
	LayOutInWeapon();
	LayOutInUseItem();
	
	OnUpdateInvenUI.AddDynamic(this,&ULSInventoryWidget::UpdateSlot);
}

void ULSInventoryWidget::UpdateSlot()
{
	LayoutInInven();
	LayOutInWeapon();
}

void ULSInventoryWidget::LayOutEmptyInven()
{
	EmptyContainer->ClearChildren();

	//빈 인벤 창
	if (!EmptySlotClass)	return;
	EmptySlot=CreateWidget<ULSInvenSlot>(this, EmptySlotClass);
	EmptySlot->SlotName=FName(TEXT("Empty"));
	EmptyContainer->AddChild(EmptySlot);
}

void ULSInventoryWidget::LayoutInInven()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn)	return;
	
	ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn);
	if (!Player)	return;

	ULSInventoryComp* InvenComp = Player->FindComponentByClass<ULSInventoryComp>();
	if (!InvenComp)	return;

	if (!InventoryTable)	return;

	InvenContainer->ClearChildren();
	
	const FString Context = TEXT("ULSInventoryWidget::NativeConstruct");

	//인벤 정리
	const TArray<FName> RowNames = InventoryTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FLSInvenRow* Row = InventoryTable->FindRow<FLSInvenRow>(RowName, Context, true);
		if (!Row) continue;

		if (InvenComp->CountItem(Row->Name)<=0)	continue;
		
		if (InvenSlotClass)
		{
			ULSInvenSlot* InvenSlot = CreateWidget<ULSInvenSlot>(this, InvenSlotClass);

			InvenSlot->IconImage->SetBrushFromTexture(Row->Icon);
			
			FText NText=FText::FromName(Row->Name);
			InvenSlot->NameText->SetText(NText);

			FString CString=FString::Printf(TEXT("X %d"),InvenComp->CountItem(Row->Name));
			InvenSlot->CountText->SetText(FText::FromString(CString));

			InvenSlot->SlotName=FName(TEXT("Inventory"));
			
			InvenSlot->SetType(Row->Category);
			
			InvenContainer->AddChild(InvenSlot);
		}
	}
	
}

void ULSInventoryWidget::LayOutInWeapon()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn)	return;
	
	ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn);
	if (!Player)	return;

	ULSInventoryComp* InvenComp = Player->FindComponentByClass<ULSInventoryComp>();
	if (!InvenComp)	return;

	if (!InventoryTable)	return;
	
	WeaponContainer->ClearChildren();	
	
	const FString Context = TEXT("ULSInventoryWidget::NativeConstruct");

	//인벤 정리
	const TArray<FName> RowNames = InventoryTable->GetRowNames();
	
	//내무기
	if (!WeaponSlotClass)	return;
	WeaponSlot=CreateWidget<ULSInvenSlot>(this, WeaponSlotClass);
	WeaponSlot->SlotName=FName(TEXT("Weapon"));
	WeaponContainer->AddChild(WeaponSlot);

	for (const FName& RowName : RowNames)
	{
		const FLSInvenRow* Row = InventoryTable->FindRow<FLSInvenRow>(RowName, Context, true);
		if (!Row) continue;

		//UE_LOG(LogTemp, Warning, TEXT("MyWeapon %s"),*InvenComp->GetMyWeapon().ToString());
		
		if (InvenComp->GetMyWeapon()==Row->Name)																					
		{
			WeaponSlot->SetType(Row->Category);
			WeaponSlot->IconImage->SetBrushFromTexture(Row->Icon);
		}	
	}
}

void ULSInventoryWidget::LayOutInUseItem()
{
	UseItemContainer->ClearChildren();	
	if (!EmptySlotClass)	return;
	UseItemSlot=CreateWidget<ULSInvenSlot>(this, EmptySlotClass);
	UseItemSlot->SlotName=FName(TEXT("UseItem"));
	UseItemContainer->AddChild(UseItemSlot);
	
}
