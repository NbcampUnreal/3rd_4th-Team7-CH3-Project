// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h" 
#include "GameFramework/PlayerController.h"
#include "LSPlayerController.generated.h" 

class UInputMappingContext; 
class UInputAction; 
class ULSShopWidget;
class ULSInventoryWidget;

class UUserWidget; 
class UWBP_MainMenu; 
class UWBP_InGameHUD; 

class UTextBlock;
class UProgressBar;

UCLASS() 
class LSPROJECT_API ALSPlayerController : public APlayerController 
{ 
	GENERATED_BODY() 

public: 
	ALSPlayerController(); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputMappingContext* InputMappingContext;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* MoveAction; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* LookAction; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* JumpAction; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* SprintAction; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* AttackAction; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* OpenShopAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* OpenInvenAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI") 
	TSubclassOf<ULSShopWidget> ShopWidgetClass; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI") 
	TObjectPtr<ULSShopWidget> ShopWidgetInstance; 
	void ShowShopWidget();
	void HideShopWidget();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI") 
	TSubclassOf<ULSInventoryWidget> InvenWidgetClass; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI") 
	TObjectPtr<ULSInventoryWidget> InvenWidgetInstance; 
	void ShowInvenWidget();
	void HideInvenWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") 
	TSubclassOf<UUserWidget> MainMenuWidgetClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") 
	TSubclassOf<UUserWidget> InGameHUDWidgetClass; 
	UPROPERTY() 
	UUserWidget* MainMenuWidget; 
	UPROPERTY() 
	UUserWidget* InGameHUDWidget;

	UFUNCTION(BlueprintPure, Category="HUD")
	UUserWidget* GetHUDWidget() const;
	
	UFUNCTION(BlueprintCallable) 
	void GameStart(); 
	UFUNCTION(BlueprintCallable) 
	void GameQuit(); 

protected: 
	virtual void BeginPlay() override;

private:
	//시작맵에서 카메라 고정도하고 메뉴도 UI온리로 표시할거
	void ShowMainMenuWidget();
	void LockToStartMapCamera();
	
	void SetupMainMapPlay(); //메인맵표시 허드랑 입력까지 GameOnly로

};  
