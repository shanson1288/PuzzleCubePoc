// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PuzzleCubePocCharacter.h"
#include "Cube.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	
	
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //only overlap no physics
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //default ignore everything
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //set specific channels, only interact with pawns on overlap
	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));
	
	FloorTile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorTile"));
	FloorTile->SetupAttachment(GetRootComponent());

	BaseMaterial = CreateDefaultSubobject<UMaterial>(TEXT("BaseMaterial"));
	BombMaterial = CreateDefaultSubobject<UMaterial>(TEXT("BombMaterial"));
	AdvantageMaterial = CreateDefaultSubobject<UMaterial>(TEXT("AdvantageMaterial"));

	bCharacterOnTile = false;
	bTileHighlighted = false;
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorTile::OnOverlapEnd);
}

// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorTile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		APuzzleCubePocCharacter* PuzzleCubePocCharacter = Cast<APuzzleCubePocCharacter>(OtherActor);
		ACube* OverlappingCube = Cast<ACube>(OtherActor);
		if (PuzzleCubePocCharacter) {
			PuzzleCubePocCharacter->SetActiveOverlappingTile(this);
			bCharacterOnTile = true;

			if (!bTileHasBomb) {
				ToggleHighlight();
			}
		} else if (OverlappingCube) {
			SetActiveOverlappingCube(OverlappingCube);
		}
	}
}

void AFloorTile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		APuzzleCubePocCharacter* PuzzleCubePocCharacter = Cast<APuzzleCubePocCharacter>(OtherActor);
		ACube* OverlappingCube = Cast<ACube>(OtherActor);
		if (PuzzleCubePocCharacter) {
			if (bTileHighlighted) {
				ToggleHighlight();
			}
			bCharacterOnTile = false;
		} else if (OverlappingCube == ActiveOverlappingCube) {
			SetActiveOverlappingCube(nullptr);
		}
	}
}

void AFloorTile::ToggleHighlight() {

		bTileHighlighted = !bTileHighlighted;
		FloorTile->SetRenderCustomDepth(bTileHighlighted);

}

bool AFloorTile::ToggleBomb() {
	bTileHasBomb = !bTileHasBomb;
	if (bTileHasBomb) {
		FloorTile->SetMaterial(0, BombMaterial);

	}
	else {
		FloorTile->SetMaterial(0, BaseMaterial);

	}
	return bTileHasBomb;
}

bool AFloorTile::SetBomb(bool bSetBomb) {
	bTileHasBomb = bSetBomb;
	if (bTileHasBomb) {
		FloorTile->SetMaterial(0, BombMaterial);

	}
	else {
		FloorTile->SetMaterial(0, BaseMaterial);

	}
	return bTileHasBomb;
}

bool AFloorTile::SetAdvantage(bool bSetAdvantage) {
	bTileHasAdvantage = bSetAdvantage;
	if (bTileHasAdvantage) {
		FloorTile->SetMaterial(0, AdvantageMaterial);
	}
	else if (bTileHasBomb) {
		FloorTile->SetMaterial(0, BombMaterial);
	}else {
		FloorTile->SetMaterial(0, BaseMaterial);
	}
	return bTileHasAdvantage;
}

bool AFloorTile::BlowUpCube() {
	bool destructionCubeDestroyed = false;
	if (bTileHasBomb) {
		bool advCubeDestroyed = false;

		if (ActiveOverlappingCube) {
			if (ActiveOverlappingCube->IsAdvantage()) {
				advCubeDestroyed = true;
			}	
			if (ActiveOverlappingCube->IsDestruction()) {
				destructionCubeDestroyed = true;
			}
			ActiveOverlappingCube->Destroy();
		}
		ToggleBomb();
		if (advCubeDestroyed) {
			SetAdvantage(true);
		}
	}
	return destructionCubeDestroyed;
}

