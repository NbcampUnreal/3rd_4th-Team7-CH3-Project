#include "Character/LSPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/LSCharacterStateComp.h"
#include "Component/LSInventoryComp.h"
#include "Controller/LSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/LSShopComp.h"
#include "Game/LSGameState.h"
#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"


ALSPlayerCharacter::ALSPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	WalkSpeed = 600.0f;
	MaxHealth = 500.0f;
	CurrentHealth = MaxHealth;
	CurrentWeapon = ECurrentWeapon::None;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ShopComp = CreateDefaultSubobject<ULSShopComp>(TEXT("ShopComponent"));
	InvenComp = CreateDefaultSubobject<ULSInventoryComp>(TEXT("InventoryComponent"));
	CharacterStateComp = CreateDefaultSubobject<ULSCharacterStateComp>(TEXT("CharacterStateComponent"));
	
	// Weapon 
	WeaponSystemComp = CreateDefaultSubobject<ULSPlayerWeaponSystemComp>(TEXT("WeaponSystemComponent")); 
}

ECurrentWeapon ALSPlayerCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

void ALSPlayerCharacter::SetCurrentWeapon(ECurrentWeapon Weapon)
{
	CurrentWeapon = Weapon;
}

float ALSPlayerCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

float ALSPlayerCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ALSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ALSPlayerController* PC = Cast<ALSPlayerController>(GetController()))
	{
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->ViewPitchMin = -70.0f;
			PC->PlayerCameraManager->ViewPitchMax = 70.0f;
		}
	}
}

void ALSPlayerCharacter::Death()
{
	Super::Death();

	PlayAnimMontage(DieMontage);
	CharacterStateComp->SetState(ECharacterState::Die);

	if (!bShowGameOver)
	{
		bShowGameOver = true;

		if (ALSPlayerController* PC = Cast<ALSPlayerController>(GetController()))
		{
			if (UUserWidget* HUD = PC->GetHUDWidget())
			{
				if (HUD->IsInViewport())
					HUD->RemoveFromParent();
			}

			if (!GameOverWidgetInstance && GameOverWidgetClass)
			{
				GameOverWidgetInstance = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
			}
			if (GameOverWidgetInstance && !GameOverWidgetInstance->IsInViewport())
			{
				GameOverWidgetInstance->AddToViewport(100);
			}

			FInputModeUIOnly Mode;
			if (GameOverWidgetInstance) Mode.SetWidgetToFocus(GameOverWidgetInstance->TakeWidget());
			PC->SetInputMode(Mode);
			PC->bShowMouseCursor = true;
			PC->SetPause(true);
		}
	}
}

void ALSPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ALSPlayerController* PlayerController = Cast<ALSPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered,
				                          this, &ALSPlayerCharacter::Move);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered,
				                          this, &ALSPlayerCharacter::Look);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered,
				                          this, &ALSPlayerCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed,
				                          this, &ALSPlayerCharacter::StopJump);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered,
				                          this, &ALSPlayerCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed,
				                          this, &ALSPlayerCharacter::StopSprint);
			}
			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Started,
				                          this, &ALSPlayerCharacter::Attack);
			}
			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Started,
										  this, &ALSPlayerCharacter::Reload);
			}
			if (PlayerController->OpenShopAction) 
			{ 
				EnhancedInput->BindAction(PlayerController->OpenShopAction, ETriggerEvent::Started, 
											this, &ALSPlayerCharacter::OpenShopUI); 
			} 

			// Weapon
			if (PlayerController->EquipPistol) 
			{
				UE_LOG(LogTemp, Warning, TEXT("피스톨 바인딩"));
				EnhancedInput->BindAction(PlayerController->EquipPistol, ETriggerEvent::Triggered, 
											this, &ALSPlayerCharacter::EquipPistol); 
			} 
			if (PlayerController->EquipShotgun) 
			{ 
				EnhancedInput->BindAction(PlayerController->EquipShotgun, ETriggerEvent::Triggered, 
											this, &ALSPlayerCharacter::EquipShotgun); 
			} 
			if (PlayerController->EquipRifle) 
			{ 
				EnhancedInput->BindAction(PlayerController->EquipRifle, ETriggerEvent::Triggered, 
											this, &ALSPlayerCharacter::EquipRifle); 
			}
			if (PlayerController->FireWeapon) 
			{
				UE_LOG(LogTemp, Warning, TEXT("binding"));
				EnhancedInput->BindAction(PlayerController->FireWeapon, ETriggerEvent::Triggered, 
											this, &ALSPlayerCharacter::FireWeapon); 
			} 
	
		} 
	}
}

float ALSPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	UpdateHealthBar(static_cast<int32>(CurrentHealth),static_cast<int32>(MaxHealth));
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		Death();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ALSPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (CharacterStateComp->CanMove())
	{
		const FVector2D MoveInput = Value.Get<FVector2D>();
		FRotator Rotation = GetControlRotation();

		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Rotation.Yaw, 0.0f));
		FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Rotation.Yaw, 0.0f));

		if (!FMath::IsNearlyZero(MoveInput.X))
		{
			AddMovementInput(ForwardVector, MoveInput.X);
		}
		if (!FMath::IsNearlyZero(MoveInput.Y))
		{
			AddMovementInput(RightVector, MoveInput.Y);
		}
	}
}

void ALSPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ALSPlayerCharacter::StartJump(const FInputActionValue& Value)
{
	if (CharacterStateComp->CanJump())
	{
		if (Value.Get<bool>())
		{
			Jump();
		}
	}
}

void ALSPlayerCharacter::StopJump(const FInputActionValue& Value)
{
	if (CharacterStateComp->CanJump())
	{
		if (!Value.Get<bool>())
		{
			StopJumping();
		}
	}
}

void ALSPlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 1.5f;
	}
}

void ALSPlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}


void ALSPlayerCharacter::Attack()
{
	Super::Attack();

	if (FireMontageCollection.IsEmpty()) return;
	if (CurrentWeapon == ECurrentWeapon::None) return;
	if (!CharacterStateComp->CanFire()) return;

	const int32 Index = static_cast<int32>(CurrentWeapon) - 1;

	if (FireMontageCollection.IsValidIndex(Index))
	{
		FireMontage = FireMontageCollection[Index];
		PlayAnimMontage(FireMontage);
		CharacterStateComp->SetState(ECharacterState::Fire);
		UE_LOG(LogTemp, Warning, TEXT("Fire attempt - State: %s"),
		       *UEnum::GetValueAsString(CharacterStateComp->GetCurrentState()));
	}
}

void ALSPlayerCharacter::Reload(const FInputActionValue& Value)
{
	if (ReloadMontageCollection.IsEmpty()) return;
	if (CurrentWeapon == ECurrentWeapon::None) return;
	if (!CharacterStateComp->CanReload()) return;

	const int32 Index = static_cast<int32>(CurrentWeapon) - 1;

	if (ReloadMontageCollection.IsValidIndex(Index))
	{
		ReloadMontage = ReloadMontageCollection[Index];
		PlayAnimMontage(ReloadMontage);
		CharacterStateComp->SetState(ECharacterState::Reload);
		UE_LOG(LogTemp, Warning, TEXT("Reload attempt - State: %s"),
		       *UEnum::GetValueAsString(CharacterStateComp->GetCurrentState()));
	}
}

void ALSPlayerCharacter::Equip()
{
	if (ReloadMontageCollection.IsEmpty()) return;
	if (CharacterStateComp->CanEquip()) return;

	const int32 Index = static_cast<int32>(CurrentWeapon) - 1;
	
	if (ReloadMontageCollection.IsValidIndex(Index))
	{
		EquipMontage = ReloadMontageCollection[Index];
		PlayAnimMontage(EquipMontage);
		CharacterStateComp->SetState(ECharacterState::Equip);
		
	}
}

void ALSPlayerCharacter::OpenShopUI()
{
	ALSGameState* GS=GetWorld()->GetGameState<ALSGameState>();
	if (!GS)	return;
	
	if (ALSPlayerController* PC=Cast<ALSPlayerController>(GetController()))
	{
		if (GS->bGetCanOpenShopUI())
		{
			PC->ShowShopWidget();	
		}
	}

}

void ALSPlayerCharacter::CheckForDoorHover()
{
	/*
	FVector CamLoc;
	FRotator CamRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CamLoc, CamRot);

	const float TraceDistance = 1000.f; // 10미터 이내만 검사
	FVector TraceEnd = CamLoc + CamRot.Vector() * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, CamLoc, TraceEnd, ECC_Visibility, Params
	);

	
	DrawDebugLine(
		GetWorld(),
		CamLoc,
		TraceEnd,
		bHit ? FColor::Green : FColor::Red,
		false,       // 지속 여부
		1.0f,        // 표시 시간 (초)
		0,           // 깊이 우선순위
		2.0f         // 두께
	);
	
	
	if (bHit && Hit.GetComponent() && Hit.GetComponent()->ComponentHasTag("Door"))
	{
		UE_LOG(LogTemp,Warning,TEXT("Door : Linetrace Hit"));
		
		FVector PlayerLoc = GetActorLocation();
		//FVector HitLoc = Hit.ImpactPoint;
		FVector DoorLoc = Hit.GetComponent()->GetComponentLocation();
		
		float Distance = FVector::Dist(PlayerLoc, DoorLoc);
		if (Distance <= MaxInteractWithDoorDistance)
		{
			UE_LOG(LogTemp,Warning,TEXT("Door : In Interaction Distance"));
			
			// 추가: 캐릭터의 정면 방향과 문 방향이 일치하는지 검사
			FVector ToDoor = (DoorLoc - PlayerLoc).GetSafeNormal();
			FVector Forward = GetActorForwardVector();

			float Dot = FVector::DotProduct(Forward, ToDoor); // -1 ~ 1
			// 1에 가까울수록 캐릭터가 정면으로 보고 있음 (예: 0.7 이상이면 정면)

			ALSGameState* GS=Cast<ALSGameState>(GetWorld()->GetGameState());
			if (!GS)	return;
			
			if (Dot > 0.5f)
			{
				UE_LOG(LogTemp,Warning,TEXT("Door : Right Direction"));
				GS->SetDoorOverlapped(true);
			}
		}
	}

	*/
}
// Weapon 
void ALSPlayerCharacter :: EquipPistol(const FInputActionValue& Value) 
{
	UE_LOG(LogTemp, Warning, TEXT("Input 1 에 입장."));	
	if (WeaponSystemComp) 
	{ 
		WeaponSystemComp->EquipPistol();
		UE_LOG(LogTemp, Warning, TEXT("Input 1 & EquipPistol 눌러잇!"));
	} 
} 
void ALSPlayerCharacter :: EquipShotgun(const FInputActionValue& Value) 
{ 
	if (WeaponSystemComp) 
	{ 
		WeaponSystemComp->EquipShotgun(); 
	} 
} 
void ALSPlayerCharacter :: EquipRifle(const FInputActionValue& Value) 
{ 
	if (WeaponSystemComp) 
	{ 
		WeaponSystemComp->EquipRifle(); 
	} 
} 

void ALSPlayerCharacter::FireWeapon(const FInputActionValue& Value)
{
	if (WeaponSystemComp)
	{
		WeaponSystemComp->FireWeapon();
	}
}

void ALSPlayerCharacter::UpdateHealthBar(int32 Current, int32 Max)
{
	if (ALSPlayerController* PC = Cast<ALSPlayerController>(GetController()))
	{
		if (!HUDHealthBar)
		{
			if (UUserWidget* HUD = PC->GetHUDWidget())
			{
				HUDHealthBar = Cast<UProgressBar>(HUD->GetWidgetFromName(TEXT("HealthBar")));
			}
		}

		if (HUDHealthBar)
		{
			const float pct = (Max > 0) ? static_cast<float>(Current) / static_cast<float>(Max) : 0.f;
			HUDHealthBar->SetPercent(FMath::Clamp(pct, 0.f, 1.f));
		}
	}
}
