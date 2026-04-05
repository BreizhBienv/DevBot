// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CommsComponent.h"

#include "JackalRobot.h"

// Sets default values for this component's properties
UCommsComponent::UCommsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCommsComponent::UpdatePingScore(FSpatialMemoryData NewPingScoreData)
{
    PingScore = 0;
    
    // Add contributions
    PingScore += (NewPingScoreData.SpatialGridCells.Num() - LastPingScoreData.SpatialGridCells.Num()) * PingWeights.W_NewCell;
    PingScore += (NewPingScoreData.SpatialMemoryElement.Num() - LastPingScoreData.SpatialMemoryElement.Num()) * PingWeights.W_NewMapItem;
    
    int rewardUpdateCount = 0;
    for (auto element : LastPingScoreData.SpatialMemoryElement)
    {
        const FSpatialMemoryElement* newElem = NewPingScoreData.SpatialMemoryElement.Find(element.Key);
        if (newElem && element.Value.ElementReward != newElem->ElementReward)
            rewardUpdateCount++;
    }
    
    PingScore += rewardUpdateCount * PingWeights.W_TaskUpdate;
}

bool UCommsComponent::ShouldSendPing()
{
    if (GetWorld()->GetTimeSeconds() - LastPingTimeStamp < PingWeights.PingCooldown)
        return false;

    if (PingScore < PingWeights.PingThreshold)
        return false;
    
    return true;
}

void UCommsComponent::BroadcastMessage(UCommsMessage* Message)
{
    if (!CommsArea)
        return;

    LastPingTimeStamp = GetWorld()->GetTimeSeconds();
    OnMessageEmitted.Broadcast(Message);

    TArray<AActor*> OverlappingActors;
    CommsArea->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (!Actor || Actor == GetOwner())
            continue;

        UCommsComponent* OtherComms =
            Actor->FindComponentByClass<UCommsComponent>();

        SendMessageToTarget(OtherComms, Message);
    }
}

void UCommsComponent::SendMessageToTarget(UCommsComponent* Target, UCommsMessage* Message)
{    
    // Send the message 
    SendDelayedMessage(
        Target, 
        Message, 
        FMath::FRandRange(MinLatency, MaxLatency)
    );
}

void UCommsComponent::SendMessageToTargetWithTimeModifier(UCommsComponent* Target, UCommsMessage* Message,
    float AddedLatency)
{
    SendDelayedMessage(
        Target, 
        Message, 
        FMath::FRandRange(MinLatency + AddedLatency, MaxLatency + AddedLatency)
    );
}

void UCommsComponent::AnswerToSender(UCommsComponent* Sender, UCommsMessage* Message)
{
    CommsTarget.Add(Sender);
    OnMessageEmitted.Broadcast(Message);
    SendMessageToTarget(Sender, Message);
}

void UCommsComponent::SendDelayedMessage(
    UCommsComponent* Target,
    UCommsMessage* Message,
    float Delay)
{

    TWeakObjectPtr<UCommsComponent> WeakTarget = Target;

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([WeakTarget, Message]()
        {
            if (WeakTarget.IsValid())
            {
                WeakTarget->OnMessageReceived.Broadcast(Message);
            }
        });

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        TimerDelegate,
        Delay,
        false
    );
}

void UCommsComponent::ExchangeData(AJackalRobot* r1, AJackalRobot* r2)
{
    UCommsComponent* c1 = r1->GetCommsComponent();
    UCommsComponent* c2 = r1->GetCommsComponent();
    
    USpatialMemoryComponent* s1 = r1->GetSpatialMemoryComponent();
    USpatialMemoryComponent* s2 = r2->GetSpatialMemoryComponent();
    
    FSpatialMemoryData d1 = s1->GetSpatialMemoryData();
    FSpatialMemoryData d2 = s2->GetSpatialMemoryData();

    s1->MergeSpatialGrid(d2.SpatialGridCells);
    s1->MergeSpatialMemoryMaps(d2.SpatialMemoryElement);
    
    s2->MergeSpatialGrid(d1.SpatialGridCells);
    s2->MergeSpatialMemoryMaps(d1.SpatialMemoryElement);
    
    c1->LastPingScoreData = d1;
    c2->LastPingScoreData = d2;
    
    //return 2.0f; 
    //0.5 per merge. 
    //Which data to be merged should be chosen and not send everything. 
    //0.5 should depend on the number of element retrieved per jackal.
}

UCommsMessage* UCommsComponent::CreateCommsMessageExchangeData(UObject* Outer, int NewSenderID,
    const FVector& NewSenderLocation, AJackalRobot* Sender, int NewTargetReceiverID, float NewTimestamp)
{
    {
        CurrentMessage = NewObject<UCommsMessage>(Outer);
        if (CurrentMessage)
        {
            CurrentMessage->SenderComponent = Sender;
            CurrentMessage->SenderID = NewSenderID;
            CurrentMessage->SenderLocation = NewSenderLocation;
            CurrentMessage->TargetReceiverID = NewTargetReceiverID;
            CurrentMessage->Timestamp = NewTimestamp;
            CurrentMessage->ToExecute.AddDynamic(this, &UCommsComponent::ExchangeData);
        }
        return CurrentMessage;
    }
}

float UCommsComponent::ExecuteMessage(UCommsMessage* message, AJackalRobot* sender, AJackalRobot* other)
{
    float result = 2.0f;
    if (message->ToExecute.IsBound())
        message->ToExecute.Broadcast(sender, other);
    
    return result;
}
