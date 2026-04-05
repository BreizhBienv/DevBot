// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/Messages/CommsMessage.h"
#include "SpatialMemoryComponent.h"
#include "CommsComponent.generated.h"

class AJackalRobot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnMessageReceived,
    UCommsMessage*, Message
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnMessageEmitted,
    UCommsMessage*, Message
);


USTRUCT(BlueprintType, Blueprintable)
struct FPingWeights
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float W_NewCell = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float W_NewMapItem = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float W_TaskUpdate = 5;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float PingThreshold = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float PingCooldown = 10.f;
};

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
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Communication")
    TArray<UCommsComponent*> CommsTarget;
    
    UPROPERTY(BlueprintReadWrite, Category = "Communication")
    UCommsMessage* CurrentMessage;

protected:

    UFUNCTION()
    void SendDelayedMessage(UCommsComponent* Target, UCommsMessage* Message, float Delay);

public:	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    FPingWeights PingWeights;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comms")
    float PingScore = 0.f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comms")
    float LastPingTimeStamp = 0.f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comms")
    FSpatialMemoryData LastPingScoreData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float MinLatency = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comms")
    float MaxLatency = 0.4f;

    UFUNCTION(BlueprintCallable, Category = "Comms")
    void UpdatePingScore(FSpatialMemoryData NewPingScoreData);
    
    UFUNCTION(BlueprintCallable, Category = "Comms")
    bool ShouldSendPing();
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    void BroadcastMessage(UCommsMessage* Message);
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    void SendMessageToTarget(UCommsComponent* Target, UCommsMessage* Message);
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    void SendMessageToTargetWithTimeModifier(UCommsComponent* Target, UCommsMessage* Message, float AddedLatency);
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    void AnswerToSender(UCommsComponent* Sender, UCommsMessage* Message);
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    void ExchangeData(AJackalRobot* r1, AJackalRobot* r2);
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    UCommsMessage* CreateCommsMessageExchangeData(UObject* Outer, int NewSenderID, const FVector& NewSenderLocation,  
                                            AJackalRobot* Sender, int NewTargetReceiverID = -1, float NewTimestamp = 0.f);
    
    UFUNCTION(BlueprintCallable, Category = "Communication")
    float ExecuteMessage(UCommsMessage* message, AJackalRobot* sender, AJackalRobot* other);
};
