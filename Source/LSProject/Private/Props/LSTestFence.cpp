#include "Props/LSTestFence.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Props/LSNullFence.h"
#include "UniversalObjectLocators/UniversalObjectLocatorUtils.h"


ALSTestFence::ALSTestFence()
{
	Scene=CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;
	
	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMesh);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALSTestFence::OnFenceOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALSTestFence::OnFenceEndOverlap);
	
	HealthBar=CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(StaticMesh);
	
	MaxHealth=100.0f;
	CurrentHealth = 100.0f;
}

void ALSTestFence::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateCurrentHealth();
	if (HealthBar)
	{
		HealthBar->SetVisibility(false);
	}
}

float ALSTestFence::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                               class AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	UpdateCurrentHealth();
	
	if (CurrentHealth<=0.0f)
	{
		if (BPNullFenceClass)
		{
			//ALSNullFence
			GetWorld()->SpawnActor<AActor>(
				BPNullFenceClass,
				GetActorLocation(),
				GetActorRotation()
			);
		}
		Destroy();
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ALSTestFence::UpdateCurrentHealth()
{
	if (!HealthBar) return;
	if (!HealthBarWidget)
	{
		HealthBarWidget = HealthBar->GetWidget();
	}
	if (HealthBarWidget)
	{
		if(UProgressBar* ProgressBar = Cast<UProgressBar>(HealthBarWidget->GetWidgetFromName(TEXT("FenceHealthBar"))))
		{
			float HealthPercent = CurrentHealth/MaxHealth;
			ProgressBar->SetPercent(HealthPercent);
		}
	}
}

void ALSTestFence::OnFenceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!OtherActor || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Overlap Is Failed"))
		return;
	}
	if (OtherActor==Player)
	{
		HealthBar->SetVisibility(true);
	}
}

void ALSTestFence::OnFenceEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!OtherActor || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Overlap Is Failed"))
	}
	if (OtherActor==Player)
	{
		HealthBar->SetVisibility(false);
	}
}
