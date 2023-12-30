// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class STEERINGMECHANICS_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		int BoidMesh = 1;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
		int SpawnCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float SpawnRadius = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float NeighbourRadius = 900;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float SeparationWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float CohesionWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float AlignmentWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float Speed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float BoundingSize = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float ConstraintForce = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float WanderWeight = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		TArray<class AObstacle*> Obstacles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool AvoidOtherBoids = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool AvoidObstacles = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		bool DynamicObstacles = true;

	UFUNCTION(BlueprintCallable)
		void ResetToDefaults();

	UFUNCTION(BlueprintCallable)
		void SpawnBoids(int targetBoidsNum);

	UFUNCTION(BlueprintCallable)
		void DrawDebug();

	UFUNCTION(BlueprintCallable)
		void RemoveDebug();

	UFUNCTION(BlueprintCallable)
		void SetBoundingBoxSize(int boxSize);

	UFUNCTION(BlueprintCallable)
		void SetAvoidBoids(bool value);

	UFUNCTION(BlueprintCallable)
		void SetAvoidObstacles(bool value);

	UFUNCTION(BlueprintCallable)
		void SetSpeed(int boidSpeed);

	UFUNCTION(BlueprintCallable)
		void SetSeparationWeight(float separationValue);

	UFUNCTION(BlueprintCallable)
		void SetCohesionWeight(float cohesionValue);

	UFUNCTION(BlueprintCallable)
		void SetAlignmentWeight(float alignmentValue);

	UFUNCTION(BlueprintCallable)
		void GenerateObstacles(int obstacleCount);

	UFUNCTION(BlueprintCallable)
		void SetMeshValue(int meshValue);

	UFUNCTION(BlueprintCallable)
		void ChangeMesh();

	UFUNCTION(BlueprintCallable)
		void SetNeighbourHoodSize(int size);

	UFUNCTION(BlueprintCallable)
		void SetConstraintForce(int size);

	UFUNCTION(BlueprintCallable)
		void SetWanderWeight(float size);

	UFUNCTION(BlueprintCallable)
		void SetDynamicObstacles(bool value);

	USceneComponent* transform;
	TArray<class ABoid*> MyBoids;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<class ABoid*> GetBoidNeighbourHood(ABoid* thisBoid);
};