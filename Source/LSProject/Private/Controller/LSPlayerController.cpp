#include "Controller/LSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"


ALSPlayerController::ALSPlayerController() :
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr),
	AttackAction(nullptr),
	ReloadAction(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidget(nullptr)
{
}

void ALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

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
void ALSPlayerController::GameStart()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}
	if (InGameHUDWidgetClass && !InGameHUDWidget)
	{
		InGameHUDWidget = CreateWidget<UUserWidget>(this,InGameHUDWidgetClass);
		if (InGameHUDWidget)
		{
			InGameHUDWidget->AddToViewport();
		}
	}
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}
void ALSPlayerController::GameQuit()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}