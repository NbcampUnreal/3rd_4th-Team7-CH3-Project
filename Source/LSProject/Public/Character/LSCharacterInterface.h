
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LSCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULSCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class LSPROJECT_API ILSCharacterInterface
{
	GENERATED_BODY()

public:
	virtual void Attack() = 0;
	virtual void Death() = 0;

};
