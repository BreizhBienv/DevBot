// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/Messages/CommsMessage.h"
#include "CommsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnMessageReceived,
    UCommsMessage*, Message
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnMessageEmitted,
    UCommsMessage*, Message
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BERYL_API UCommsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCommsComponent();

    UPROPERTY(BlueprintAssignable, Category = "Communication")
    FOnMessageReceived OnMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Communication")
    FOnMessageEmitted OnMessageEmitted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Communication")
    USphereComponent* CommsArea;

protected:

    UFUNCTION()
    void SendDelayedMessage(UCommsComponent* Target, UCommsMessage* Message, float Delay);

public:	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float MinLatency = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float MaxLatency = 0.4f;

    UFUNCTION(BlueprintCallable, Category = "Communication")
    void BroadcastMessage(UCommsMessage* Message);


};
