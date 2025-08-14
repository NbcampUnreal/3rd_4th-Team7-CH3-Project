#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSTestFence.generated.h"

class UBoxComponent;
class UWidgetComponent;
class ALSNullFence;

UCLASS()
class LSPROJECT_API ALSTestFence : public AActor
{
	GENERATED_BODY()
	
public:
	ALSTestFence();

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SceneComp")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComp")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoxComp")
	UBoxComponent* BoxCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD_Health")
	UWidgetComponent* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD_Health")
	UUserWidget* HealthBarWidget;

	void UpdateCurrentHealth();
	
	UFUNCTION()
	virtual void OnFenceOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnFenceEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);

	UPROPERTY()
	AActor* Player=nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ALSNullFence> BPNullFenceClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SceneComp")
	float MaxHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SceneComp")
	float CurrentHealth;
};

