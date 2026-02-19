// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CommsComponent.h"

// Sets default values for this component's properties
UCommsComponent::UCommsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCommsComponent::BroadcastMessage(UCommsMessage* Message)
{
    if (!CommsArea)
        return;

    OnMessageEmitted.Broadcast(Message);

    TArray<AActor*> OverlappingActors;
    CommsArea->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (!Actor || Actor == GetOwner())
            continue;

        UCommsComponent* OtherComms =
            Actor->FindComponentByClass<UCommsComponent>();

        if (OtherComms)
        {
            // Send the message 
            SendDelayedMessage(
                OtherComms, 
                Message, 
                FMath::FRandRange(MinLatency, MaxLatency)
            );
        }
    }
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
