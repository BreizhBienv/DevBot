// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Paths.h"
#include "CSVDataSaverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BERYL_API UCSVDataSaverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCSVDataSaverComponent();

	UFUNCTION(BlueprintCallable)
	void SaveStringAsCSV(FString HeaderRow, FString DataStr);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ResultsFileName = "RunsResults";

	UFUNCTION(BlueprintCallable)
	FString GetCSVFilePath();
};