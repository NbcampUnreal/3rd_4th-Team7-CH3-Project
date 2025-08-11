// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "LSDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class LSPROJECT_API ULSDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	ULSDragDropOperation();

	UPROPERTY()
	UObject* InvenPayload;
};
