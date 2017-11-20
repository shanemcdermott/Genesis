// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Maps/IslandMapGenerator.h"
#include "GenesisMapGenerator.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMapGen, Log, All);
/**
 * 
 */
UCLASS()
class GENESIS_API AGenesisMapGenerator : public AIslandMapGenerator
{
	GENERATED_BODY()
	
	
public:
	

	//Attempt at constructing map in editor.
	UFUNCTION(BlueprintCallable, Category = "World Generation|Continent Generation")
		void ConstructMap(const FIslandGeneratorDelegate onComplete);
	
	UFUNCTION(BlueprintCallable, Category = "World Generation|Continent Generation")
		void ExportPolygons(FString SaveDirectory, const FString& FileName);

};
