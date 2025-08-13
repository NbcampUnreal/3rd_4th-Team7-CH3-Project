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

	LayoutInUI();
}

void ULSInventoryWidget::LayoutInUI()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn)	return;
	
	ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn);
	if (!Player)	return;

	ULSInventoryComp* InvenComp = Player->FindComponentByClass<ULSInventoryComp>();
	if (!InvenComp)	return;

	if (!InventoryTable)	return;

	const FString Context = TEXT("ULSInventoryWidget::NativeConstruct");
	
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

			InvenSlot->SetType(Row->Category);
			
			InvenContainer->AddChild(InvenSlot);	
		}
	}
	
}
