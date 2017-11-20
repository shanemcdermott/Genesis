// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Maps/PointGenerators/PointGenerator.h"
#include "PoissonPointGenerator.generated.h"

/**
 * 
 */
UCLASS()
class GENESIS_API UPoissonPointGenerator : public UPointGenerator
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
		TArray<int32> OpenPoints;

	UPROPERTY()
		TArray<FVector2D> PointArray;

	TArray<TArray<int32>> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
		int32 SampleLimit = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
		float MinimumDistance = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Points")
		float MaximumDistance = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Points")
		float Radius2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
		float CellSize;

	UPoissonPointGenerator() { bNeedsMoreRandomness = false; };
	~UPoissonPointGenerator() {};

protected:
		virtual TArray<FVector2D> GeneratePoints_Implementation(int32 numberOfPoints) override;
		
		void Initialize(int32 numberOfPoints);

		bool FindSamplePoint(const FVector2D& InOrigin, FVector2D& OutResult);
		void FindSamplePoints(const FVector2D& InOrigin);

		bool IsPointValid(const FVector2D& InPoint);

		FIntPoint PointToGrid(const FVector2D& InPoint);
};
