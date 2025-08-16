#pragma once

#include "CoreMinimal.h"
#include "LSTestFence.h"
#include "GameFramework/Actor.h"
#include "LSNullFence.generated.h"

class UWidgetComponent;
class UBoxComponent;

UCLASS()
class LSPROJECT_API ALSNullFence : public AActor
{
	GENERATED_BODY()

public:	
	ALSNullFence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSNullFence")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSNullFence")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSNullFence")
	UUserWidget* UserWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSNullFence")
	UWidgetComponent* WidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSNullFence")
	USoundBase* RestoreSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LSNullFence")
	UBoxComponent* OverlapBoxComponent;

	void RestoreFence();

	UFUNCTION(BlueprintCallable, Category = "Fence")
	static void RestoreOverlappedFence(UWorld* World);

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Fence")
	void OnOverlapStartEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Fence")
	void OnOverlapEndEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ALSTestFence> BPFenceClass;
	
	UPROPERTY()
	AActor* Player=nullptr;

	UPROPERTY()
	int32 UseCoin;

	bool IsOverlaped;

};
