// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorManager.h"
#include "Engine/World.h"

// Sets default values
AFloorManager::AFloorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	width = 5;
	length = 10;
	cubeLength = 5;
}

// Called when the game starts or when spawned
void AFloorManager::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	
	if (FloorTileClass) {
		float posy = 0.f;
		float posx = 0.f;

		int size = width * length;
		for (int pos = 0; pos < size; pos++) {

			
			AFloorTile* SpawnedFloorTile = World->SpawnActor<AFloorTile>(FloorTileClass, FVector(posx, posy, 0.f), FRotator(0.f));
			FloorTiles.Add(SpawnedFloorTile);

			// on last row
			if (pos / width == length - 1) {
				//ACube* SpawnedCube = World->SpawnActor<ACube>(MyCube, FVector(posx, posy, 130.f), FRotator(0.f));
				
				//TODO remove this is for testing
				//if (pos % 3 == 0) {
				//	SpawnedCube->SetIsAdvantage(true);
				//}
				//if (pos % 5 == 0) {
				//	SpawnedCube->SetIsDestruction(true);
				//}
			}
			
			if ((pos +1) % width == 0) {
				//end of column, new row
				posy = 0;
				posx += 200;
			}
			else {
				posy += 200;
			}
		}

		if (CubeClass) {
			int cubeAmt = width * cubeLength;
			for (int pos = 0; pos < cubeAmt; pos++) {
				ACube* SpawnedCube = World->SpawnActor<ACube>(CubeClass, FVector(posx, posy, 130.f), FRotator(0.f));

				int32 randNum = FMath::RandRange(1, 10);

				if (randNum <= 3) {
					SpawnedCube->SetIsDestruction(true);
				}
				else if (randNum > 3 && randNum <= 8) {

				}
				else if (randNum > 8) {
					SpawnedCube->SetIsAdvantage(true);
				}

				if ((pos + 1) % width == 0) {
					//end of column, new row
					posy = 0;
					posx += 200;
				}
				else {
					posy += 200;
				}
			}
		}
	}
}

// Called every frame
void AFloorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorManager::SetBombOnTile(AFloorTile* OverlappedTile) {
	if (OverlappedTile) {
		bool overalappingCurrentBomb = false;
		if (OverlappedTile == ActiveBombTile) {
			overalappingCurrentBomb = true;
		}
		//disable current bomb tile
		if (ActiveBombTile) {
			bool isTileActive = ActiveBombTile->ToggleBomb();
			if (!isTileActive) {
				SetActiveBombTile(nullptr);
			}
		}

		if (!overalappingCurrentBomb) {
			//enable overlapping tile
			AFloorTile* floorTile = OverlappedTile;
			bool isTileActive = floorTile->ToggleBomb();
			if (isTileActive) {
				SetActiveBombTile(floorTile);
			}
		}
	}
}

void AFloorManager::SetOffBomb() {
	int rowsToRemove = 0;
	for (AFloorTile* FloorTile : FloorTiles) {
		if (FloorTile->bTileHasBomb) {
			bool destructionCubeDestroyed = FloorTile->BlowUpCube();
			if (destructionCubeDestroyed) {
				rowsToRemove++;
			}
		}
	}
	if (ActiveBombTile) {
		SetActiveBombTile(nullptr);
	}
	if (rowsToRemove != 0) {
		RemoveRows(rowsToRemove);
	}
}

void AFloorManager::ActivateAdv() {
	for (AFloorTile* FloorTile : FloorTiles) {
		if (FloorTile->bTileHasAdvantage) {
			SetAdvBombs(FloorTile);
			FloorTile->SetAdvantage(false);
		}
	}
}

void AFloorManager::SetAdvBombs(AFloorTile* advTile) {
	int posOfAdv = FloorTiles.Find(advTile);
	advTile->SetBomb(true);

	int xpos = posOfAdv % width;
	int ypos = posOfAdv / width;

	struct pair{
		int x;
		int y;
	};

	pair directions[8] = { {-1,-1}, {-1,0}, {-1,1},  {0,1}, {1,1},  {1,0},  {1,-1},  {0, -1} };
	for (pair direction : directions) {
		int cx = xpos + direction.x;
		int cy = ypos + direction.y;
		if (cy >= 0 && cy < length) {
			if (cx >= 0 && cx < width) {
				int posToFInd = cx + (cy) * width;
				FloorTiles[posToFInd]->SetBomb(true);
			}
		}
	}
}

void AFloorManager::RemoveRows(int RowsToRemove) {
	for (int i = 0; i < RowsToRemove; i++) {
		for (int pos = 0; pos < width; pos++) {
			AFloorTile* FloorTile = FloorTiles[0];
			FloorTile->Destroy();
			FloorTiles.RemoveAt(0);
		}
		length -= 1;
	}
}

