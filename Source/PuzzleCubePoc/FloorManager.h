// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.h"
#include "Cube.h"
#include "FloorManager.generated.h"

UCLASS()
class PUZZLECUBEPOC_API AFloorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
	int width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
	int length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
	int cubeLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	class AFloorTile* ActiveBombTile;

	FORCEINLINE void SetActiveBombTile(AFloorTile* Tile) { ActiveBombTile = Tile; }

	void SetBombOnTile(AFloorTile* OverlappedTile);
	void SetOffBomb();

	void ActivateAdv();

	void RemoveRows(int RowsToRemove);

	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	TSubclassOf<class AFloorTile> FloorTileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	TSubclassOf<class ACube> CubeClass;

private:
	TArray<AFloorTile*> FloorTiles;
	void SetAdvBombs(AFloorTile* advTile);
};
