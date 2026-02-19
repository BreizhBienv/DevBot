// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CSVDataSaverComponent.h"

// Sets default values for this component's properties
UCSVDataSaverComponent::UCSVDataSaverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCSVDataSaverComponent::SaveStringAsCSV(FString HeaderRow, FString DataStr)
{
	FString FilePath = GetCSVFilePath();

	FFileHelper::SaveStringToFile(HeaderRow + "\n" + DataStr, *FilePath);

	UE_LOG(LogTemp, Warning, TEXT("Saved file at: %s\n========= END OF TESTS =========\n"), *FilePath);
}

FString UCSVDataSaverComponent::GetCSVFilePath()
{
	FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
	return FPaths::ProjectSavedDir() + ResultsFileName + Timestamp + ".csv";
}
