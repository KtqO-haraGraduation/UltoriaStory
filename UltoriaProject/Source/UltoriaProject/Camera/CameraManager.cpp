// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "../Battle/BattleAreaManager.h"
#include "../Battle/BattleManager.h"
#include "../Battle/Combat/Lifecycle/BattleLifecycle.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// フィールドカメラへ切り替え
void UCameraManager::ChangeFieldCamera()
{
    APlayerController* playerController =
        UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!playerController) return;

    APawn* player = playerController->GetPawn();
    if (!player) return;

    playerController->SetViewTargetWithBlend(player, 0.0f);
}

// バトルカメラへ切り替え
void UCameraManager::ChangeBattleCamera(EBattleCameraType type)
{
    APlayerController* playerController =
        UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!playerController) return;

    ABattleAreaManager* area = GetBattleAreaManager();
    if (!area) return;

    ACameraActor* camera = area->GetCamera(type);
    if (!camera) return;

    playerController->SetViewTargetWithBlend(camera, 1.0f);
}

// バトルエリアを取得
ABattleAreaManager* UCameraManager::GetBattleAreaManager()
{
    UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
    if (!battleMg) return nullptr;

    return battleMg->GetLifecycle()->GetAreaManager();
}

// 現在のカメラを取得
ACameraActor* UCameraManager::GetCurrentCamera()
{
    APlayerController* playerController =
        UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!playerController) return nullptr;

    return Cast<ACameraActor>(playerController->GetViewTarget());
}
