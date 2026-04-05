// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommsMessage.generated.h"

class AJackalRobot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FToExecute,
	AJackalRobot*, Sender,
	AJackalRobot*, Other
);

UCLASS(Blueprintable)
class BERYL_API UCommsMessage : public UObject
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector SenderLocation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int SenderID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AJackalRobot* SenderComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int TargetReceiverID = -1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Timestamp;

	//TArray<TFunction<float(AJackalRobot*, AJackalRobot*)>> ToExecute;
	UPROPERTY(BlueprintAssignable)
	FToExecute ToExecute;
	
	UFUNCTION(BlueprintCallable)
	static UCommsMessage* CreateCommsMessage(UObject* Outer, int NewSenderID, const FVector& NewSenderLocation, 
											AJackalRobot* Sender, int NewTargetReceiverID = -1, float NewTimestamp = 0.f)
	{
		UCommsMessage* NewMessage = NewObject<UCommsMessage>(Outer);
		if (NewMessage)
		{
            NewMessage->SenderComponent = Sender;
			NewMessage->SenderID = NewSenderID;
			NewMessage->SenderLocation = NewSenderLocation;
			NewMessage->TargetReceiverID = NewTargetReceiverID;
			NewMessage->Timestamp = NewTimestamp;
		}
		return NewMessage;
	}
};
