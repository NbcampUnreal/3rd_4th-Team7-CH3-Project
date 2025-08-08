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
#include "Weapon/LSPlayerWeaponSystemComp.h" 

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
	Health = 300.0f;
	CurrentWeapon = ECurrentWeapon::None;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ShopComp = CreateDefaultSubobject<ULSShopComp>(TEXT("ShopComponent"));
	InvenComp = CreateDefaultSubobject<ULSInventoryComp>(TEXT("InventoryComponent"));
	CharacterStateComp = CreateDefaultSubobject<ULSCharacterStateComp>(TEXT("CharacterStateComponent"));

	// Weapon 
	WeaponSystem = CreateDefaultSubobject<ULSPlayerWeaponSystemComp>(TEXT("WeaponSystem")); 
}

ECurrentWeapon ALSPlayerCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

void ALSPlayerCharacter::SetCurrentWeapon(ECurrentWeapon Weapon)
{
	CurrentWeapon = Weapon;
}

void ALSPlayerCharacter::Death()
{
	Super::Death();

	PlayAnimMontage(DieMontage);
	CharacterStateComp->SetState(ECharacterState::Die);
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

			// Weapon 
			if (PlayerController->EquipPistol) 
			{ 
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
		} 
	}
}

float ALSPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     AController* EventInstigator, AActor* DamageCauser)
{
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

// Weapon 
void ALSPlayerCharacter :: EquipPistol(const FInputActionValue& Value) 
{ 
	if (WeaponSystem) 
	{ 
		WeaponSystem->EquipPistol(); 
	} 
} 
void ALSPlayerCharacter :: EquipShotgun(const FInputActionValue& Value) 
{ 
	if (WeaponSystem) 
	{ 
		WeaponSystem->EquipShotgun(); 
	} 
} 
void ALSPlayerCharacter :: EquipRifle(const FInputActionValue& Value) 
{ 
	if (WeaponSystem) 
	{ 
		WeaponSystem->EquipRifle(); 
	} 
} 

