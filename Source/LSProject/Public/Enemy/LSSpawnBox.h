#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "LSSpawnBox.generated.h"

UCLASS()
class LSPROJECT_API ULSSpawnBox : public UBoxComponent
{
	GENERATED_BODY()

public:
	ULSSpawnBox();

	void ZombieSqwan();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	int32 SpqwnZombieNum;
	
};
