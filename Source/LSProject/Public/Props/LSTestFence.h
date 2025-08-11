#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSTestFence.generated.h"

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
	
private:
	float Health;
};
