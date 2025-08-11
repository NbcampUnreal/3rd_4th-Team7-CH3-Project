// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LSInventoryWidget.h"
#include "Character/LSPlayerCharacter.h"
#include "Component/LSInventoryComp.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Widget/LSInvenSlot.h"

void ULSInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LayoutInUI();
}

void ULSInventoryWidget::LayoutInUI()
{
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn))
		{
			if (ULSInventoryComp* InvenComp = Player->FindComponentByClass<ULSInventoryComp>())
			{
				if (InvenSlotClass)
				{
					//ULSInvenSlot* InvenSlot = CreateWidget<ULSInvenSlot>(this, InvenSlotClass);

					//Slot->Name->SetText()
				
					//InvenContainer->AddChild(InvenSlot);	
				}
				
			}
		}
		
	}
}
