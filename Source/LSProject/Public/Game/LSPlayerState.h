#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LSPlayerState.generated.h"

UCLASS()
class LSPROJECT_API ALSPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ALSPlayerState();

	FORCEINLINE int32 GetCoin() const{return Coin;}
	FORCEINLINE void SetCoin(int32 NewCoin) {Coin=NewCoin;}

	FORCEINLINE int32 GetZombieNum() const { return ZombieNum; }
	UFUNCTION(BlueprintCallable) void AddZombieKill(int32 Delta = 1);


private:
	int32 Coin;
	int32 ZombieNum=0;
};
