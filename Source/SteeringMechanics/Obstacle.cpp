// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "BoidManager.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	Mesh->SetStaticMesh(sphereMesh);
	this->SetRootComponent(Mesh);

	FVector obstacleSize = FVector(FMath::RandRange(1.0f, 2.0f), FMath::RandRange(1.0f, 2.0f), FMath::RandRange(1.0f, 2.0f));
	this->SetActorScale3D(obstacleSize);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

//Seek function
FVector AObstacle::Seek(FVector position)
{
	FVector newVelocity = position - GetActorLocation();
	newVelocity.Normalize();

	return newVelocity;
}

//Wander function
void AObstacle::Wander()
{
	FVector myLocation = GetActorLocation();

	if (FVector::Dist(myLocation, WanderDestination) < 500.0f || WanderDestination == FVector::ZeroVector) {
		WanderDestination = FVector(FMath::RandRange(-5000.0f, 5000.0f), FMath::RandRange(-5000.0f, 5000.0f), FMath::RandRange(-5000.0f, 5000.0f)); //Should be using the current bounding box size but unreal cant use the boid manager reference :(
	}
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Update the obstacle - picks a random location and travels towards it
void AObstacle::UpdateObstacle(float DeltaTime)
{
	FVector targetVelocity = FVector::ZeroVector;
	FVector location = GetActorLocation();

	Wander();
	targetVelocity += Seek(WanderDestination);

	FVector newForce = targetVelocity - currentVelocity;
	currentVelocity += newForce * DeltaTime;
	location += (currentVelocity * 500.0f * DeltaTime);

	SetActorLocation(location);
}