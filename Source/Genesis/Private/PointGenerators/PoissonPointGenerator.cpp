// Fill out your copyright notice in the Description page of Project Settings.

#include "PoissonPointGenerator.h"


// Random Point Selector
TArray<FVector2D> UPoissonPointGenerator::GeneratePoints_Implementation(int32 numberOfPoints)
{
	
	Initialize(numberOfPoints);


	float randomX = RandomGenerator.FRandRange(Border, MaxPoint());
	float randomY = RandomGenerator.FRandRange(Border, MaxPoint());
	OpenPoints.Add(PointArray.Add(FVector2D(randomX, randomY)));

	while(OpenPoints.Num() > 0 && PointArray.Num() < numberOfPoints)
	{
		int32 Index = OpenPoints.Pop();
		FindSamplePoints(PointArray[Index]);
	}
	return PointArray;
}

void UPoissonPointGenerator::Initialize(int32 numberOfPoints)
{
	
	MaximumDistance = MinimumDistance * 2.f;
	Radius2 = MinimumDistance * MinimumDistance;
	CellSize = MinimumDistance / FMath::Sqrt(2);

	OpenPoints.Empty();
	PointArray.Empty();
	Grid.SetNum(MapSize / CellSize);
	for (int32 i = 0; i < Grid.Num(); i++)
	{
		Grid[i].SetNumZeroed(Grid.Num());
	}
	//background.SetNum(numberOfPoints);
}

bool UPoissonPointGenerator::FindSamplePoint(const FVector2D & InOrigin, FVector2D & OutResult)
{

	float angle = 2 * RandomGenerator.FRand() * 3.14159f;
	float r = FMath::Sqrt(RandomGenerator.FRand() * 3 * Radius2 + Radius2);

	OutResult.X = InOrigin.X + 2 * FMath::Cos(angle);
	OutResult.Y = InOrigin.Y + 2 * FMath::Sin(angle);
		
	if (IsPointValid(OutResult))
		return true;

	return false;
}

void UPoissonPointGenerator::FindSamplePoints(const FVector2D& InOrigin)
{
	
	for (int k = 0; k < SampleLimit; k++)
	{
		FVector2D NewPoint;
		if (FindSamplePoint(InOrigin, NewPoint))
		{
			int32 id = PointArray.Add(NewPoint);
			OpenPoints.Emplace(id);
			FIntPoint GCoord = PointToGrid(NewPoint);
			Grid[GCoord.X][GCoord.Y] = id;
		}
	}
	
}

bool UPoissonPointGenerator::IsPointValid(const FVector2D & InPoint)
{
	FIntPoint GridCoord = PointToGrid(InPoint);

	int GridX = GridCoord.X;
	int GridY = GridCoord.Y;
	if (GridX < 0 || GridY < 0 || GridX >= Grid.Num() || GridY >= Grid.Num()) return false;

	int xmin = FMath::Max(GridX - 2, 0);
	int ymin = FMath::Max(GridY - 2, 0);
	int xmax = FMath::Min(GridX + 2, MaxPoint());
	int ymax = FMath::Min(GridY + 2, MaxPoint());

	for (int y = ymin; y <= ymax; y++)
	{
		for (int x = xmin; x <= xmax; x++)
		{
			FVector2D s = PointArray[Grid[x][y]];
			if (s != FVector2D::ZeroVector)
			{
				FVector2D d = s - InPoint;
				if (d.X * d.X + d.Y * d.Y < Radius2) return false;
			}
		}
	}
	return true;
}

FIntPoint UPoissonPointGenerator::PointToGrid(const FVector2D& InPoint)
{
	FIntPoint result;
	result.X = (int)InPoint.X / CellSize;
	result.Y = (int)InPoint.Y / CellSize;
	return result;
}
