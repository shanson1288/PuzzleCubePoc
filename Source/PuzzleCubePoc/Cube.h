// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class PUZZLECUBEPOC_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube | Collision")
	class UBoxComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube | Mesh")
	class UStaticMeshComponent* Cube;

	UPROPERTY(EditAnywhere)
	class UMaterial* BaseMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* AdvantageMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* DestructionMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube | CubeProperties")
	bool bMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube | CubeProperties")
	float MovementRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float RotateValue;
	FRotator CubeRotation;
	
	FVector StartLocation;
	FVector EndLocation;

	FRotator StartRotation;
	FRotator EndRotation;

	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CubeMovement")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CubeMovement")
	float InterpTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CubeMovement")
	bool bInterping;

	FTimerHandle InterpTimer;


	void ToggleInterping();
	void GetNewLocation(FVector& VecOne, FVector& VecTwo);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cube | CubeProperties")
	bool bAdvantage;

	FORCEINLINE bool IsAdvantage() { return bAdvantage; }

	void SetIsAdvantage(bool bSetAdvtantage);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cube | CubeProperties")
	bool bDestruction;

	FORCEINLINE bool IsDestruction() { return bDestruction; }

	void SetIsDestruction(bool bSetDestruction);
};
