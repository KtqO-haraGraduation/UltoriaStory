// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MyNetworkPlayerController.generated.h"


UCLASS()
class ULTORIAPROJECT_API AMyNetworkPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    // UIƒ{ƒ^ƒ“‚©‚çŒÄ‚Ô
    UFUNCTION(BlueprintCallable)
    void FindSession();

    UFUNCTION(BlueprintCallable)
    void CreateRoom(const FString& roomId);

    UFUNCTION(BlueprintCallable)
    void JoinRoom(const FString& roomId);

    UFUNCTION(BlueprintCallable)
    void Input1();

    UFUNCTION(BlueprintCallable)
    void SessionLeave();

    UFUNCTION(BlueprintCallable)
    void IsReady();

private:
    UFUNCTION(Server, Reliable)
    void Server_Input1();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Input1();

    UFUNCTION(Client, Reliable)
    void Client_ShowInput1();

    UFUNCTION(Server, Reliable)
    void Server_LeaveSession();

    UFUNCTION(Client, Reliable)
    void Client_LeaveSession();

    void CreateWidgetsOnce();
    void UpdateUI();

    void ShowMainUI();
    void HideMainUI();

    void ShowReadyUI();
    void CheckPlayerStateReady();

    void HideReadyUI();

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> m_MainWidgetClass;

    UPROPERTY()
    UUserWidget* m_MainWidget;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> m_ReadyWidgetClass;

    UPROPERTY()
    UUserWidget* m_ReadyWidget;
};
