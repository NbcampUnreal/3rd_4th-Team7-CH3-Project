
#pragma once

#include "CoreMinimal.h"
#include "LSCharacterInterface.h"
#include "GameFramework/Character.h"
#include "LSCharacterBase.generated.h"

UCLASS()
class LSPROJECT_API ALSCharacterBase : public ACharacter, public ILSCharacterInterface
{
	GENERATED_BODY()

public:
	ALSCharacterBase();

protected:
	float WalkSpeed;
	float Health;
	
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Death() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
