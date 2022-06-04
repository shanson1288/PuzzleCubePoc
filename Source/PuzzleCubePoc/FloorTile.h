// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

UCLASS()
class PUZZLECUBEPOC_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

	// overlap volume for trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Tile")
	class UBoxComponent* TriggerBox;

	//tile for character to step on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Tile")
	class UStaticMeshComponent* FloorTile;

	UPROPERTY(EditAnywhere)
	class UMaterial* BaseMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* BombMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* AdvantageMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bCharacterOnTile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bTileHasBomb;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bTileHasAdvantage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool bTileHighlighted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool ToggleBomb();

	bool SetBomb(bool bSetBomb);

	bool SetAdvantage(bool bSetAdvantage);

	void ToggleHighlight();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	class ACube* ActiveOverlappingCube;

	FORCEINLINE void SetActiveOverlappingCube(ACube* Cube) { ActiveOverlappingCube = Cube; }

	bool BlowUpCube();
};
