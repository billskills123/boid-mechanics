// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"
#include "Obstacle.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(transform);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Spawns in the default set number of boids
	for (int i = 0; i < SpawnCount; i++) {
		FVector SpawnLocation = (FMath::VRand() * FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		ABoid* newboid = GetWorld()->SpawnActor<ABoid>(SpawnLocation, SpawnRotation);
		newboid->Manager = this;

		MyBoids.Add(newboid);
	}
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ABoid* Boid : MyBoids) {
		Boid->UpdateBoid(DeltaTime); //Used to call the update function on every boid
	}

	if (DynamicObstacles == true ) {
		for (AObstacle* Obstacle : Obstacles) {
			Obstacle->UpdateObstacle(DeltaTime); //Only update obstacles position if needed
		}
	}
}

//Finds all of the boids neighbours
TArray<class ABoid*> ABoidManager::GetBoidNeighbourHood(ABoid* thisBoid)
{
	TArray<class ABoid*> ReturnBoids;

	for (ABoid* Boid : MyBoids)
	{
		if (Boid == thisBoid || !Boid) continue;

		float aDistance = (Boid->GetActorLocation() - thisBoid->GetActorLocation()).Size();

		if (aDistance < NeighbourRadius) {
			ReturnBoids.Add(Boid);
		}
	}

	return ReturnBoids;
}

//Reset all of the values to their default numbers
void ABoidManager::ResetToDefaults()
{
	SpawnBoids(50);
	SetAvoidBoids(true);
	SetAvoidObstacles(true);
	SetSpeed(750);
	SetSeparationWeight(1);
	SetCohesionWeight(1);
	SetAlignmentWeight(1);
	GenerateObstacles(0);
	SetBoundingBoxSize(1000);
	SetNeighbourHoodSize(1000);
	SetConstraintForce(2);
	SetDynamicObstacles(true);
	SetWanderWeight(1.5f);
	ChangeMesh();
	RemoveDebug();
}

//Spawn in new boids
void ABoidManager::SpawnBoids(int targetBoidsNum)
{
	SpawnCount = targetBoidsNum;

	if (targetBoidsNum > MyBoids.Num()) {
		int spawnBoids = targetBoidsNum - MyBoids.Num();

		//Spawns in new boids
		for (int i = 0; i < spawnBoids; i++) {
			FVector SpawnLocation = (FMath::VRand() * FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();

			ABoid* newboid = GetWorld()->SpawnActor<ABoid>(SpawnLocation, SpawnRotation);
			newboid->Manager = this;

			MyBoids.Add(newboid);
		}
	}
	else if (targetBoidsNum < MyBoids.Num()) {

		//Deletes all boids
		for (ABoid* Boid : MyBoids)
		{
			Boid->Destroy();
		}

		MyBoids.Empty();

		//Spawns in new boids
		for (int i = 0; i < SpawnCount; i++) {
			FVector SpawnLocation = (FMath::VRand() * FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();

			ABoid* newboid = GetWorld()->SpawnActor<ABoid>(SpawnLocation, SpawnRotation);
			newboid->Manager = this;

			MyBoids.Add(newboid);
		}
	}

	for (ABoid* Boid : MyBoids)
	{
		Boid->ChangeMesh(); //Updates the mesh on all new boids
	}
}

//Draws a debug box
void ABoidManager::DrawDebug()
{
	RemoveDebug();
	DrawDebugBox(GetWorld(), FVector::Zero(), FVector(BoundingSize, BoundingSize, BoundingSize), FColor::Green, true, -1, 0, 20);
}

//Removes the debug box
void ABoidManager::RemoveDebug()
{
	FlushPersistentDebugLines(GetWorld());
}

//Set bounding box size
void ABoidManager::SetBoundingBoxSize(int boxSize)
{
	BoundingSize = boxSize;
}

//Set whether boids should avoid each other
void ABoidManager::SetAvoidBoids(bool value)
{
	AvoidOtherBoids = value;
}

//Set whether boids should avoid obstacles
void ABoidManager::SetAvoidObstacles(bool value)
{
	AvoidObstacles = value;
}

//Set speed value
void ABoidManager::SetSpeed(int boidSpeed)
{
	Speed = boidSpeed;
}

//Set separation value
void ABoidManager::SetSeparationWeight(float separationValue)
{
	SeparationWeight = separationValue;
}

//Set cohesion value
void ABoidManager::SetCohesionWeight(float cohesionValue)
{
	CohesionWeight = cohesionValue;
}

//Set alignment value
void ABoidManager::SetAlignmentWeight(float alignmentValue)
{
	AlignmentWeight = alignmentValue;
}

//Generate new obstacles
void ABoidManager::GenerateObstacles(int obstacleCount)
{
	//Destroy all current obstacles
	for (int i = 0; i < Obstacles.Num(); i++) {
		Obstacles[i]->Destroy();
	}

	Obstacles.Empty();

	//Spawn new obstacles
	for (int i = 0; i < obstacleCount; i++)
	{
		FVector obstacleSpawnLocation = FVector(FMath::RandRange(BoundingSize, -BoundingSize), FMath::RandRange(BoundingSize, -BoundingSize), FMath::RandRange(BoundingSize, -BoundingSize));
		FRotator SpawnRotation = GetActorRotation();

		AObstacle* newObstacle = GetWorld()->SpawnActor<AObstacle>(obstacleSpawnLocation, SpawnRotation);

		Obstacles.Add(newObstacle);
	}
}

//Set boid mesh value
void ABoidManager::SetMeshValue(int meshValue)
{
	BoidMesh = meshValue;
}

//Change boids mesh
void ABoidManager::ChangeMesh()
{
	for (ABoid* Boid : MyBoids)
	{
		Boid->ChangeMesh();
	}
}

//Set neighbourhood size value
void ABoidManager::SetNeighbourHoodSize(int size)
{
	NeighbourRadius = size;
}

//Set constraint force
void ABoidManager::SetConstraintForce(int size)
{
	ConstraintForce = size;
}

//Set wander value
void ABoidManager::SetWanderWeight(float size)
{
	WanderWeight = size;
}

//Set whether obstacles are dynamic
void ABoidManager::SetDynamicObstacles(bool value)
{
	DynamicObstacles = value;
}
