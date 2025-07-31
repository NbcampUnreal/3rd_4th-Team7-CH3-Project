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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SceneComp")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComp")
	UStaticMeshComponent* StaticMesh;

protected:
	virtual void BeginPlay() override;

private:
	float Health;
};
