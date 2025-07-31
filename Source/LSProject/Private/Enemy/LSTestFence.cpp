#include "Enemy/LSTestFence.h"

ALSTestFence::ALSTestFence()
{
	Scene=CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = Scene;
	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(Scene);
	Health=100.0f;
}

void ALSTestFence::BeginPlay()
{
	Super::BeginPlay();
	
}


