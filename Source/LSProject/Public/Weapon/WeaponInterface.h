
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class LSPROJECT_API IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void Fire() = 0;
	virtual void Reload() = 0;
};
