#include "Props/LSNullFence.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

ALSNullFence::ALSNullFence()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(Root);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALSNullFence::OnOverlapStartEvent);
	MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ALSNullFence::OnOverlapEndEvent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(Root);

	IsOverlaped=false;
	
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] ALSNullFence"));
}

void ALSNullFence::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetComponent)
	{
		UserWidget=WidgetComponent->GetUserWidgetObject();
		if(UserWidget)
		{
			UserWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ALSNullFence::OnOverlapStartEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (OtherActor==Player && WidgetComponent)
	{
		UserWidget=WidgetComponent->GetUserWidgetObject();
		if(UserWidget)
		{
			UserWidget->SetVisibility(ESlateVisibility::Visible);
			IsOverlaped=true;
		}
	}
}

void ALSNullFence::OnOverlapEndEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (OtherActor==Player && WidgetComponent)
	{
		if (WidgetComponent)
		{
			UserWidget=WidgetComponent->GetUserWidgetObject();
			if(UserWidget)
			{
				UserWidget->SetVisibility(ESlateVisibility::Hidden);
				IsOverlaped=false;
			}
		}
	}
}

//EnemyTodo : 수정요망
void ALSNullFence::RestoreFence()
{
	if (!IsOverlaped || !BPFenceClass) return;
	GetWorld()->SpawnActor<AActor>(
		BPFenceClass,
		GetActorLocation(),
		GetActorRotation()
	);
	
}

void ALSNullFence::RestoreOverlappedFence(UWorld* World)
{
	if (!World) return;
    
	TArray<AActor*> FoundFences;
	UGameplayStatics::GetAllActorsOfClass(World, ALSNullFence::StaticClass(), FoundFences);
    
	for (AActor* Actor : FoundFences)
	{
		if (ALSNullFence* Fence = Cast<ALSNullFence>(Actor))
		{
			if (Fence->IsOverlaped)
			{
				Fence->RestoreFence();
				return;
			}
		}
	}
}
