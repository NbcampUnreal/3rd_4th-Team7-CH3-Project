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
#include "Weapon/LSWeaponBase.h"
#include "Props/LSNullFence.h" //Static함수로 만들어서 가져올까 그냥?
#include "Components/CapsuleComponent.h"


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

	WalkSpeed = 300.0f;
	MaxHealth = 500.0f;
	CurrentHealth = MaxHealth;
	CurrentWeapon = ECurrentWeapon::None;
	bCanFire = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ShopComp = CreateDefaultSubobject<ULSShopComp>(TEXT("ShopComponent"));
	InventoryComp = CreateDefaultSubobject<ULSInventoryComp>(TEXT("InventoryComponent"));
	CharacterStateComp = CreateDefaultSubobject<ULSCharacterStateComp>(TEXT("CharacterStateComponent"));
	WeaponSystemComp = CreateDefaultSubobject<ULSPlayerWeaponSystemComp>(TEXT("WeaponSystemComponent"));
	Tags.AddUnique("Player");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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

	if (CharacterStateComp->CanDie())
	{
		CharacterStateComp->SetState(ECharacterState::Die);

		if (ALSPlayerController* PC = Cast<ALSPlayerController>(GetController()))
		{
			PC->ShowGameOverWidget();
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
				EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Triggered,
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
			if (PlayerController->OpenInvenAction)
			{
				EnhancedInput->BindAction(PlayerController->OpenInvenAction, ETriggerEvent::Started,
				                          this, &ALSPlayerCharacter::StartInvenUI);
			}
			if (PlayerController->OpenInvenAction)
			{
				EnhancedInput->BindAction(PlayerController->OpenInvenAction, ETriggerEvent::Completed,
				                          this, &ALSPlayerCharacter::EndInvenUI);
			}
			if (PlayerController->RestoreFenceAction)
			{
				EnhancedInput->BindAction(PlayerController->RestoreFenceAction, ETriggerEvent::Triggered,
				                          this, &ALSPlayerCharacter::RestoreFence);
			}
		}
	}
}

float ALSPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		Death();
	}
	UE_LOG(LogTemp, Warning, TEXT("Damage: curr=%.1f"), CurrentHealth);
	UpdateHealthBar(CurrentHealth, MaxHealth);
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
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 2.0f;
	}
}

void ALSPlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ALSPlayerCharacter::ResetFireTimer()
{
	bCanFire = true;
}

void ALSPlayerCharacter::Attack()
{
	Super::Attack();

	if (!bCanFire) return;
	if (FireMontageCollection.IsEmpty()) return;
	if (CurrentWeapon == ECurrentWeapon::None) return;
	if (!CharacterStateComp->CanFire()) return;

	if (WeaponSystemComp->CurrentWeapon->GetCurrentAmmo() > 0)
	{
		const int32 Index = static_cast<int32>(CurrentWeapon) - 1;

		if (FireMontageCollection.IsValidIndex(Index))
		{
			FireMontage = FireMontageCollection[Index];
			PlayAnimMontage(FireMontage);
			CharacterStateComp->SetState(ECharacterState::Fire);

			WeaponSystemComp->CurrentWeapon->Fire();
		}
	}
	else
	{
			WeaponSystemComp->CurrentWeapon->PlayNoneFireSound();
	}
	
	bCanFire = false;
	float FireRate = WeaponSystemComp->CurrentWeapon->GetFireRate();

	GetWorld()->GetTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ALSPlayerCharacter::ResetFireTimer,
		FireRate,
		false);
}

void ALSPlayerCharacter::Reload(const FInputActionValue& Value)
{
	if (ReloadMontageCollection.IsEmpty()) return;
	if (CurrentWeapon == ECurrentWeapon::None) return;
	if (!WeaponSystemComp->CurrentWeapon->IsCanReload()) return;
	if (!CharacterStateComp->CanReload()) return;
	if (!InventoryComp->HasAmmo())	return;
	
	const int32 Index = static_cast<int32>(CurrentWeapon) - 1;

	if (ReloadMontageCollection.IsValidIndex(Index))
	{
		ReloadMontage = ReloadMontageCollection[Index];
		PlayAnimMontage(ReloadMontage);
		CharacterStateComp->SetState(ECharacterState::Reload);
	}
}

void ALSPlayerCharacter::Equip()
{
	if (EquipMontageCollection.IsEmpty()) return;
	if (!CharacterStateComp->CanEquip()) return;

	const int32 Index = static_cast<int32>(CurrentWeapon) - 1;

	if (EquipMontageCollection.IsValidIndex(Index))
	{
		EquipMontage = EquipMontageCollection[Index];
		PlayAnimMontage(EquipMontage);
		CharacterStateComp->SetState(ECharacterState::Equip);
	}
}

void ALSPlayerCharacter::OpenShopUI()
{
	ALSGameState* GS = GetWorld()->GetGameState<ALSGameState>();
	if (!GS) return;

	if (ALSPlayerController* PC = Cast<ALSPlayerController>(GetController()))
	{
		if (GS->bGetCanOpenShopUI())
		{
			PC->ShowShopWidget();
		}
	}
}

void ALSPlayerCharacter::StartInvenUI()
{
	ALSPlayerController* PC = Cast<ALSPlayerController>(GetController());
	if (!PC) return;

	PC->ShowInvenWidget();
}

void ALSPlayerCharacter::EndInvenUI()
{
	ALSPlayerController* PC = Cast<ALSPlayerController>(GetController());
	if (!PC) return;

	PC->HideInvenWidget();
}

void ALSPlayerCharacter::RestoreFence()
{
	ALSNullFence::RestoreOverlappedFence(GetWorld());
}

void ALSPlayerCharacter::UpdateHealthBar(float Current, float Max)
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
			float pct = (Max > 0) ? Current / Max : 0.f;
			HUDHealthBar->SetPercent(pct);
		}
	}
}
