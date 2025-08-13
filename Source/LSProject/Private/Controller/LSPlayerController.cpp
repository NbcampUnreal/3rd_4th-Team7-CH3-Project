#include "Controller/LSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Widget/LSShopWidget.h"
#include "Widget/LSInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"

ALSPlayerController::ALSPlayerController() :
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr),
	AttackAction(nullptr),
	ReloadAction(nullptr),
	OpenShopAction(nullptr),
	OpenInvenAction(nullptr),
	ShopWidgetClass(nullptr),
	ShopWidgetInstance(nullptr),
	InvenWidgetClass(nullptr),
	InvenWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	InGameHUDWidgetClass(nullptr),
	MainMenuWidget(nullptr),
	InGameHUDWidget(nullptr)
{
}


void ALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	if (LevelName == TEXT("LSStartMap"))
	{
		ShowMainMenuWidget();
		return; 
	}
	SetupMainMapPlay();

	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	
	if (IsLocalController() && MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>( this, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}

}

void ALSPlayerController::ShowShopWidget()
{
	if (!ShopWidgetInstance&& ShopWidgetClass)
	{
		ShopWidgetInstance = CreateWidget<ULSShopWidget>(this, ShopWidgetClass);
		if (ShopWidgetInstance)
		{
			ShopWidgetInstance->AddToViewport();
			FInputModeUIOnly InputMode;
			//FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;

			UE_LOG(LogTemp, Warning, TEXT("Show SHOP WIDGET"));
		}
	}
}

void ALSPlayerController::HideShopWidget()
{
	if (ShopWidgetInstance)
	{
		ShopWidgetInstance->RemoveFromParent();
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
		ShopWidgetInstance=nullptr;
		
		UE_LOG(LogTemp, Warning, TEXT("Hide SHOP WIDGET"));
	}
}

void ALSPlayerController::ShowInvenWidget()
{
	if (!InvenWidgetClass)	return;
	
	InvenWidgetInstance = CreateWidget<ULSInventoryWidget>( this, InvenWidgetClass);
	InvenWidgetInstance->AddToViewport();
	InvenWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void ALSPlayerController::HideInvenWidget()
{
	if (!InvenWidgetInstance)	return;
	
	InvenWidgetInstance->RemoveFromParent();
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	ShopWidgetInstance=nullptr;
}

void ALSPlayerController::GameStart()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	UGameplayStatics::OpenLevel(this, FName(TEXT("LSMainMap")));
	

}
void ALSPlayerController::GameQuit()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}

UUserWidget* ALSPlayerController::GetHUDWidget() const
{
	return InGameHUDWidget;
}

void ALSPlayerController::ShowMainMenuWidget()
{
	LockToStartMapCamera();

	if (IsLocalController() && MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}



void ALSPlayerController::LockToStartMapCamera() 
{
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		SetViewTargetWithBlend(*It, 0.f);
		break; 
	}
}

void ALSPlayerController::SetupMainMapPlay()
{
	if (InGameHUDWidgetClass && !InGameHUDWidget)
	{
		InGameHUDWidget = CreateWidget<UUserWidget>(this, InGameHUDWidgetClass);
		if (InGameHUDWidget)
		{
			InGameHUDWidget->AddToViewport();
		}
	}

	// 게임 입력 모드 전환
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}
