// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class STEERINGMECHANICS_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();
	FVector currentVelocity = FVector::ZeroVector;

	class ABoidManager* BoidManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		FVector WanderDestination;

	void Wander();
	FVector Seek(FVector position);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateObstacle(float DeltaTime);
};
