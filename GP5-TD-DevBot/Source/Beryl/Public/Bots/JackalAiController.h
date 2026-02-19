// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "JackalAiController.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnTargetReached
);

UCLASS()
class BERYL_API AJackalAiController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "JackalBot|Events")
	FOnTargetReached OnTargetReached;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JackalBot|Path")
	TArray<FVector> PathPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JackalBot|Path")
	int32 CurrentPathPointIndex = 0;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};
