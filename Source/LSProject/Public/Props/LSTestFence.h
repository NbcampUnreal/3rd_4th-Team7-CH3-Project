#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSTestFence.generated.h"

class UBoxComponent;

UCLASS()
class LSPROJECT_API ALSTestFence : public AActor
{
	GENERATED_BODY()
	
public:
	ALSTestFence();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SceneComp")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComp")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoxComp")
	UBoxComponent* BoxCollision;

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
	
private:

	float Health;
};

