// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "BoidManager.h"
#include "Obstacle.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	boidMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Models/Fish.Fish'")).Object; //Default mesh for the boids

	Mesh->SetStaticMesh(boidMesh);
	this->SetRootComponent(Mesh);

	boidSize = FVector(5.0f, 5.0f, 5.0f); //Default size for the boids
	this->SetActorScale3D(boidSize);

	WanderDestination = GetActorLocation();
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
}

//Used to change the mesh of the boid
void ABoid::ChangeMesh()
{
	switch (Manager->BoidMesh) {
	case 1:
		boidMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Models/Fish.Fish'"));

		if (boidMesh)
		{
			Mesh->SetStaticMesh(boidMesh);
		}

		boidSize = FVector(5.0f, 5.0f, 5.0f);
		this->SetActorScale3D(boidSize);
		break;

	case 2:
		boidMesh = LoadObject<UStaticMesh>(nullptr, TEXT("Skele'/Game/Models/Bird_Mesh.Bird_Mesh'"));

		if (boidMesh)
		{
			Mesh->SetStaticMesh(boidMesh);
		}

		boidSize = FVector(FMath::RandRange(10.0f, 20.0f), FMath::RandRange(10.0f, 20.0f), FMath::RandRange(10.0f, 20.0f));
		this->SetActorScale3D(boidSize);
		break;

	case 3:
		boidMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

		if (boidMesh)
		{
			Mesh->SetStaticMesh(boidMesh);
		}

		boidSize = FVector(1.0f, 1.0f, 1.0f);
		this->SetActorScale3D(boidSize);
		break;

	case 4:
		boidMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));

		if (boidMesh)
		{
			Mesh->SetStaticMesh(boidMesh);
		}

		boidSize = FVector(1.0f, 1.0f, 1.0f);
		this->SetActorScale3D(boidSize);
		break;

	case 5:
		boidMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));

		if (boidMesh)
		{
			Mesh->SetStaticMesh(boidMesh);
		}

		boidSize = FVector(1.0f, 1.0f, 1.0f);
		this->SetActorScale3D(boidSize);
		break;
	}
}

//Seeking function
FVector ABoid::Seek(FVector position)
{
	FVector newVelocity = position - GetActorLocation();
	newVelocity.Normalize();

	return newVelocity;
}

//Fleeing function
FVector ABoid::Flee(FVector position)
{
	FVector newVelocity = GetActorLocation() - position;
	newVelocity.Normalize();

	return newVelocity;
}

//Wander function
void ABoid::Wander()
{
	FVector myLocation = GetActorLocation();

	if (FVector::Dist(myLocation, WanderDestination) < 500.0f || WanderDestination == FVector::ZeroVector) {
		WanderDestination = FVector(FMath::RandRange(-Manager->BoundingSize, Manager->BoundingSize), FMath::RandRange(-Manager->BoundingSize, Manager->BoundingSize), FMath::RandRange(-Manager->BoundingSize, Manager->BoundingSize)); //Picks a random wander destination inside the bounding box
	}
}

//Alignment function
FVector ABoid::Alignment(TArray<ABoid*> Neighbours)
{
	if (Neighbours.Num() == 0) return FVector::ZeroVector;

	FVector newVelocity;

	for (ABoid* Boid : Neighbours) {
		newVelocity += Boid->currentVelocity;
	}

	newVelocity /= Neighbours.Num();
	newVelocity.Normalize();

	return newVelocity;
}

//Cohesion function
FVector ABoid::Cohesion(TArray<ABoid*> Neighbours)
{
	if (Neighbours.Num() == 0) return FVector::ZeroVector;

	FVector avgLocation;

	float inverseVal = 1 / Neighbours.Num();

	for (ABoid* Boid : Neighbours) {
		avgLocation += Boid->GetActorLocation() * inverseVal;
	}

	return Seek(avgLocation);
}

//Separation function
FVector ABoid::Seperation(TArray<ABoid*> Neighbours)
{
	if (Neighbours.Num() == 0) return FVector::ZeroVector;

	FVector avgFlee;

	for (ABoid* Boid : Neighbours) {
		avgFlee += Flee(Boid->GetActorLocation());
	}

	avgFlee.Normalize();

	return avgFlee;
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Update boid
void ABoid::UpdateBoid(float DeltaTime) {

	FVector location = GetActorLocation();
	FVector targetVelocity = FVector::ZeroVector;
	TArray<ABoid*> closestBoids = Manager->GetBoidNeighbourHood(this);

	//Bounding Box
	if (location.X > Manager->BoundingSize || location.X < -Manager->BoundingSize || location.Y > Manager->BoundingSize || location.Y < -Manager->BoundingSize || location.Z > Manager->BoundingSize || location.Z < -Manager->BoundingSize) {
		targetVelocity += Seek(FVector(FMath::RandRange(-Manager->BoundingSize, Manager->BoundingSize), FMath::RandRange(-Manager->BoundingSize, Manager->BoundingSize), FMath::RandRange(-Manager->BoundingSize, Manager->BoundingSize))) * Manager->ConstraintForce; //Seeks a random position inside the bounding box when outside
	}

	//Avoid Obstacles
	if (Manager->AvoidObstacles == true) {
		for (AActor* obstacle : Manager->Obstacles) {
			if (FVector::Dist(location, obstacle->GetActorLocation()) < 500.0f) {
				targetVelocity += Flee(obstacle->GetActorLocation()) * Manager->ConstraintForce; //Flees from the object
			}
		}
	}

	if (closestBoids.Num() == 0) {
		Wander();
		targetVelocity += Seek(WanderDestination); //If a boid has no neighbours it will wander around aimlessly
	}
	else if (closestBoids.Num() > 0) {
		if (Manager->AvoidOtherBoids == true) {
			for (ABoid* Boid : closestBoids) {
				if (FVector::Dist(location, Boid->GetActorLocation()) < 150.0f) {
					targetVelocity += Flee(Boid->GetActorLocation()) * Manager->ConstraintForce; //If a boid has neighbours it will attempt to avoid them
				}
			}
		}

		//Combine all of the force together
		Wander();
		targetVelocity += Seek(WanderDestination) * Manager->WanderWeight;
		targetVelocity += Alignment(closestBoids) * Manager->AlignmentWeight;
		targetVelocity += Cohesion(closestBoids) * Manager->CohesionWeight;
		targetVelocity += Seperation(closestBoids) * Manager->SeparationWeight;
	}

	//Moves the boid
	FVector newForce = targetVelocity - currentVelocity;
	currentVelocity += newForce * DeltaTime;
	location += (currentVelocity * Manager->Speed * DeltaTime);

	SetActorLocation(location);

	//Rotates the boid
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + currentVelocity);
	newRotation = FMath::RInterpTo(GetActorRotation(), newRotation, DeltaTime, 10.0f);
	SetActorRotation(newRotation);
}