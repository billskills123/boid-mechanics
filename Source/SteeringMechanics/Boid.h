// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class STEERINGMECHANICS_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

	FVector currentVelocity = FVector::ZeroVector;

	class ABoidManager* Manager;

	UFUNCTION(BlueprintCallable)
		void ChangeMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector Seek(FVector position);
	FVector Flee(FVector position);

	void Wander();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		FVector WanderDestination;

	FVector Alignment(TArray<ABoid*> Neighbours);
	FVector Cohesion(TArray<ABoid*> Neighbours);
	FVector Seperation(TArray<ABoid*> Neighbours);

	UStaticMeshComponent* Mesh;
	UStaticMesh* boidMesh;
	FVector boidSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float DeltaTime);
};
