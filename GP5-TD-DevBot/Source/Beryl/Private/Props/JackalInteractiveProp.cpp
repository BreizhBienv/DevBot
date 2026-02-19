// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/JackalInteractiveProp.h"

// Sets default values
AJackalInteractiveProp::AJackalInteractiveProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AJackalInteractiveProp::FinishPropTask()
{
	OnTaskFinished.Broadcast(Reward);
	Reward = 0.f;
}

float AJackalInteractiveProp::GetReward()
{
	return Reward;
}

EInteractivePropType AJackalInteractiveProp::GetPropType()
{
	return PropType;
}


