// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNetworkGameInstance.h"
#include "Engine/Engine.h"

void UMyNetworkGameInstance::Init()
{
    Super::Init();

    if (GEngine)
    {
        GEngine->OnTravelFailure().AddUObject(
            this,
            &UMyNetworkGameInstance::HandleTravelFailure
        );
    }
}

void UMyNetworkGameInstance::HandleTravelFailure(
    UWorld* World,
    ETravelFailure::Type FailureType,
    const FString& ErrorString)
{
    FString Message = FString::Printf(
        TEXT("Travel Failure: %d\n%s"),
        static_cast<int32>(FailureType),
        *ErrorString
    );

    UE_LOG(LogTemp, Error, TEXT("%s"), *Message);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 15.0f, FColor::Red, Message
        );
    }
}
