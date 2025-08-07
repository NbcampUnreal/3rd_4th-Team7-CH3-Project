#include "Controller/LSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Widget/LSShopWidget.h"

ALSPlayerController::ALSPlayerController() :
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr),
	AttackAction(nullptr),
	ReloadAction(nullptr),
	ShopWidgetClass(nullptr),
	ShopWidgetInstance(nullptr)
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

	CreateShopWidget();
}

void ALSPlayerController::CreateShopWidget()
{
	if (ShopWidgetClass)
	{
		ShopWidgetInstance = CreateWidget<ULSShopWidget>(this, ShopWidgetClass);
		if (ShopWidgetInstance)
		{
			ShopWidgetInstance->AddToViewport();
			ShopWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
