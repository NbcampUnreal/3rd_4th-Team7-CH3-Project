#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "LSDayNightController.generated.h"

UCLASS()
class LSPROJECT_API ALSDayNightController : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSDayNightController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DayNight")
	int32 GetCurrentDay () const { return CurrentDay;}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	ADirectionalLight* SunLightActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	AActor* SkySphereActor = nullptr;
    
	UPROPERTY(EditAnywhere, Category="DayNight")
	float DayDuration = 120.f;

	UPROPERTY(EditAnywhere, Category="DayNight")
	float NightDuration = 180.f;

	UPROPERTY(EditAnywhere, Category = "DayNight")
	int32 MaxDays = 5;

private:
	int32 CurrentDay = 1;
	float ElapsedPhaseTime = 0.f; //
	bool bIsDayPhase = true;
	float SunYaw =  -180.f;

	UMaterialInstanceDynamic* SkyMID = nullptr;
	
};
