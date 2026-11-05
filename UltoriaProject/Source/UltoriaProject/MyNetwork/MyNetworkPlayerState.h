// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Delegates/DelegateCombinations.h"
#include "MyNetworkPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChanged);

UCLASS()
class ULTORIAPROJECT_API AMyNetworkPlayerState : public APlayerState
{
    GENERATED_BODY()

private:
    AMyNetworkPlayerState();

    UPROPERTY(ReplicatedUsing = OnRep_SlotIndex)
    int32 m_SlotIndex;

    UPROPERTY(ReplicatedUsing = OnRep_IsReady)
    bool m_IsReady;

public:
    UFUNCTION(BlueprintCallable)
    int32 GetSlotIndex() const { return m_SlotIndex; }
    UFUNCTION(BlueprintCallable)
    bool GetIsReady() const { return m_IsReady; }

    UFUNCTION(BlueprintCallable)
    void SetSlotIndex(int32 NewIndex);
    void SetIsReady(bool NewValue);

    UFUNCTION()
    void OnRep_SlotIndex();

    UPROPERTY(BlueprintAssignable)
    FOnStateChanged OnStateChanged;

    UFUNCTION(BlueprintCallable)
    void MyStateChanged();

    UFUNCTION(BlueprintImplementableEvent, Category = "Ready")
    void OnReadyStateChanged();

    UFUNCTION()
    void OnRep_IsReady();

    UFUNCTION(Server, Reliable)
    void Server_SetIsReady(bool NewValue);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
