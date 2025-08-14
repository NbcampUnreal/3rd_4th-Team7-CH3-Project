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
class UWBP_GameOver;
class UWBP_GameClear;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") 
	UInputAction* RestoreFenceAction;
	
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
	FORCEINLINE ULSInventoryWidget* GetInvenWidget() {return InvenWidgetInstance;};
	void ShowInvenWidget();
	void HideInvenWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") 
	TSubclassOf<UUserWidget> MainMenuWidgetClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") 
	TSubclassOf<UUserWidget> InGameHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameClearWidgetClass;
	UPROPERTY() 
	UUserWidget* MainMenuWidget; 
	UPROPERTY() 
	UUserWidget* InGameHUDWidget;
	UPROPERTY()
	UUserWidget* GameOverWidget;
	UPROPERTY()
	UUserWidget* GameClearWidget;

	UFUNCTION(BlueprintPure, Category="HUD")
	UUserWidget* GetHUDWidget() const;
	
	UFUNCTION(BlueprintCallable) 
	void GameStart(); 
	UFUNCTION(BlueprintCallable) 
	void GameQuit();
	UFUNCTION(BlueprintCallable)
	void ShowGameOverWidget();
	UFUNCTION(BlueprintCallable)
	void ShowGameClearWidget();
	UFUNCTION(BlueprintCallable)
	void HideHUDWidget();

	UPROPERTY(VisibleAnywhere, Category="UI")
	bool bGameOverShown = false;
	UPROPERTY(VisibleAnywhere, Category="UI")
	bool bGameClearShown = false;

protected: 
	virtual void BeginPlay() override;

private:
	void ShowMainMenuWidget();
	void LockToStartMapCamera();
	void SetupMainMapPlay();
	void SetUIOnlyInput(UUserWidget* Focus = nullptr);
	void SetGameOnlyInput();

};  
