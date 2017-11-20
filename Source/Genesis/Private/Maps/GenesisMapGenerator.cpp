// Fill out your copyright notice in the Description page of Project Settings.

#include "GenesisMapGenerator.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogMapGen);

void AGenesisMapGenerator::ConstructMap(const FIslandGeneratorDelegate onComplete)
{
	CreateMap(onComplete);
}

void AGenesisMapGenerator::ExportPolygons(FString SaveDirectory, const FString & FileName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TArray<TSharedPtr<FJsonValue>> Centers;
	Centers.SetNum(MapGraph->GetCenterNum());

	for (int32 i = 0; i < MapGraph->GetCenterNum(); i++)
	{
		TSharedPtr<FJsonObject> JsonNode = MakeShareable(new FJsonObject);
		FMapCenter MapCenter = MapGraph->GetCenter(i);
		FVector2D CenterPoint = MapCenter.CenterData.Point;
		
		JsonNode->SetNumberField(TEXT("CenterX"), CenterPoint.X);
		JsonNode->SetNumberField(TEXT("CenterY"), CenterPoint.Y);
		JsonNode->SetNumberField(TEXT("CenterZ"), MapCenter.CenterData.Elevation);
		JsonNode->SetStringField(TEXT("Biome"), MapCenter.CenterData.Biome.GetTagName().ToString());
		JsonNode->SetNumberField(TEXT("Moisture"), MapCenter.CenterData.Moisture);
		TArray<TSharedPtr<FJsonValue>> Corners;
		Corners.SetNum(MapCenter.Corners.Num());
		for (int i = 0; i < Corners.Num(); i++)
		{
			Corners[i] = MakeShareable(new FJsonValueNumber(MapCenter.Corners[i]));
		}

		JsonNode->SetArrayField(TEXT("Corners"), Corners);
		
		Centers[i] = MakeShareable(new FJsonValueObject(JsonNode));
	}

	JsonObject->SetArrayField(TEXT("Centers"), Centers);
	
	TArray<TSharedPtr<FJsonValue>> Corners;
	Corners.SetNum(MapGraph->GetCornerNum());
	for (int32 i = 0; i < MapGraph->GetCornerNum(); i++)
	{
		TSharedPtr<FJsonObject> JsonNode = MakeShareable(new FJsonObject);
		FMapCorner MapCorner = MapGraph->GetCorner(i);
		FVector2D CenterPoint = MapCorner.CornerData.Point;

		JsonNode->SetNumberField(TEXT("CenterX"), CenterPoint.X);
		JsonNode->SetNumberField(TEXT("CenterY"), CenterPoint.Y);
		JsonNode->SetNumberField(TEXT("CenterZ"), MapCorner.CornerData.Elevation);
		JsonNode->SetStringField(TEXT("Biome"), MapCorner.CornerData.Biome.GetTagName().ToString());
		JsonNode->SetNumberField(TEXT("Moisture"), MapCorner.CornerData.Moisture);
		
		Corners[i] = MakeShareable(new FJsonValueObject(JsonNode));
	}

	JsonObject->SetArrayField(TEXT("Corners"), Corners);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	
	if (FFileHelper::SaveStringToFile(OutputString, *FileName))
	{
		UE_LOG(LogMapGen, Log, TEXT("File Saved."));
	}
	else
	{
		UE_LOG(LogMapGen, Error, TEXT("File Failed to Save to: %s"), *FileName);
	}
}
