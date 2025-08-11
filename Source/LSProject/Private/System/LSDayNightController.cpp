#include "System/LSDayNightController.h"
#include "Components/StaticMeshComponent.h"

ALSDayNightController::ALSDayNightController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALSDayNightController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Day %d START"), CurrentDay);
	if (!SunLightActor || !SkySphereActor)
	{
		SetActorTickEnabled(false);
		return;
	}
	if (auto* MeshComp = SkySphereActor->FindComponentByClass<UStaticMeshComponent>())
	{
		SkyMID = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	}

	if (!SkyMID)
	{
		SetActorTickEnabled(false);
		return;
	}

	SunLightActor->SetActorRotation(FRotator(0.f, SunYaw, 0.f));
	SkyMID->SetScalarParameterValue(TEXT("SunHeight"), -1.f);
}

void ALSDayNightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!SunLightActor || !SkyMID) return;

	ElapsedPhaseTime += DeltaTime;

	const float PhaseDuration = bIsDayPhase ? DayDuration : NightDuration;
	const float TargetAngleChange = 180.f;

	float PhaseProgress = FMath::Clamp(ElapsedPhaseTime / PhaseDuration, 0.f, 1.f);
    
	float StartPitch = bIsDayPhase ? -180.f : 0.f;
	float CurrentPitch = StartPitch + (TargetAngleChange * PhaseProgress);

	if (PhaseProgress >= 1.0f)
	{
		bIsDayPhase = !bIsDayPhase;
		ElapsedPhaseTime = 0.f;

		if (!bIsDayPhase)
		{
			UE_LOG(LogTemp, Warning, TEXT("Day %d Night START"), CurrentDay);
			CurrentPitch = 0.f;
		}
		else
		{
			if (CurrentDay < MaxDays)
			{
				CurrentDay++;
				UE_LOG(LogTemp, Warning, TEXT("Day %d Day START"), CurrentDay); 
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GAME OVER : 5 days OVER")); 
				SetActorTickEnabled(false);
				return; 
			}
			CurrentPitch = -180.f;
		}
	}
    
	SunLightActor->SetActorRotation(FRotator(CurrentPitch, 0.f, 0.f));
    
	float SunHeight = FMath::Sin(FMath::DegreesToRadians(CurrentPitch + 180.f));
	SkyMID->SetScalarParameterValue(TEXT("SunHeight"), SunHeight);
}

int32 ALSDayNightController::GetRemainSecond() const
{
	const float PhaseDuration = bIsDayPhase ? DayDuration : NightDuration;
	const float Remain = FMath::Max(0.f,PhaseDuration - ElapsedPhaseTime);
	return FMath::CeilToInt(Remain);
} 