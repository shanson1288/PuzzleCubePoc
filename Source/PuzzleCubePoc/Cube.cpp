// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetupAttachment(GetRootComponent());

	BaseMaterial = CreateDefaultSubobject<UMaterial>(TEXT("BaseMaterial"));
	AdvantageMaterial = CreateDefaultSubobject<UMaterial>(TEXT("AdvantageMaterial"));
	DestructionMaterial = CreateDefaultSubobject<UMaterial>(TEXT("DestructionMaterial"));

	StartLocation = FVector(0.f);
	EndLocation = FVector(0.f);

	StartRotation = FRotator(0.0f, 0.0f, 0.0f);
	EndRotation = FRotator(0.0f, 0.0f, 0.0f);

	InterpSpeed = 3.0f;
	InterpTime = 2.f;
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	EndLocation += StartLocation + FVector(-200.0f, 0.0f, 0.0f);

	StartRotation = GetActorRotation();
	EndRotation += StartRotation + FRotator(90.0f, 0.0f, 0.0f);

	bInterping = false;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &ACube::ToggleInterping, InterpTime);

	Distance = (EndLocation - StartLocation).Size();

	if (bAdvantage) {
		Cube->SetMaterial(0, AdvantageMaterial);
	}
	else if (bDestruction) {
		Cube->SetMaterial(0, DestructionMaterial);
	}
	else {
		Cube->SetMaterial(0, BaseMaterial);
	}
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bInterping) {
		//FRotator CurrentRotation = GetActorRotation();
		//FRotator InterpR = FMath::RInterpTo(CurrentRotation, EndRotation, DeltaTime, InterpSpeed);
		//SetActorRotation(InterpR);
		

		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndLocation, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StartLocation).Size();
		if (Distance - DistanceTraveled <= 1.f) {
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &ACube::ToggleInterping, InterpTime);
			GetNewLocation(StartLocation, EndLocation);
			//EndRotation.Add(90.0f, 0.0f, 0.0f);
		}
	}
}

void ACube::ToggleInterping() {
	bInterping = !bInterping;
}

void ACube::GetNewLocation(FVector& start, FVector& end) {
	start = FVector(end.X,end.Y,end.Z);
	end += FVector(-200.0f, 0.0f, 0.0f);
}

void ACube::SetIsAdvantage(bool bSetAdvtantage) {
	bAdvantage = bSetAdvtantage;
	
	if (bAdvantage) {
		Cube->SetMaterial(0, AdvantageMaterial);
	}
	else {
		Cube->SetMaterial(0, BaseMaterial);
	}
}

void ACube::SetIsDestruction(bool bSetDestruction) {
	bDestruction = bSetDestruction;

	if (bDestruction) {
		Cube->SetMaterial(0, DestructionMaterial);
	}
	else {
		Cube->SetMaterial(0, BaseMaterial);
	}
}