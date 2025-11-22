#include "FortTimeOfDayManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFortTimeOfDayManager::AFortTimeOfDayManager()
{
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		FVector DesiredSize(99999.9f, 99999.9f, 99999.9f);
		FVector OriginalExtent = SphereMesh->GetStaticMesh()->GetBoundingBox().GetExtent();
		FVector OriginalSize = OriginalExtent * 2.0f;

		if (!OriginalSize.IsZero())
		{
			FVector ScaleFactor = DesiredSize / OriginalSize;
			SphereMesh->SetWorldScale3D(ScaleFactor);
		}
		SphereMesh->SetCastShadow(false);
	}

	// Configurar colisiones
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereMesh->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AFortTimeOfDayManager::BeginPlay()
{
	Super::BeginPlay();

	FVector FinalSize = SphereMesh->Bounds.BoxExtent * 2.0f;
	UE_LOG(LogTemp, Warning, TEXT("Final Sphere Size: %s"), *FinalSize.ToString());
}

// Called every frame
void AFortTimeOfDayManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
