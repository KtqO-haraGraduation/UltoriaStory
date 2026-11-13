// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"
#include "../Battle/BattleAreaManager.h"
#include "../Battle/BattleManager.h"
#include "../Battle/Combat/Lifecycle/BattleLifecycle.h"
#include "../Battle/PlayerPartyManager.h"
#include "../Character/Ally/MainPlayer.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// フィールドカメラへ切り替え
void UCameraManager::ChangeFieldCamera()
{
    APlayerController* playerController =
        UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!playerController) return;

    UPlayerPartyManager* playerPartyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
    if (!playerPartyMg) return;

    AMainPlayer* fieldPlayer = playerPartyMg->GetFieldPlayer();
    if (!IsValid(fieldPlayer)) return;

    // 操作対象をフィールPlayerに変更
    playerController->Possess(fieldPlayer);

    // カメラをフィールドPlayerに変更
    playerController->SetViewTargetWithBlend(fieldPlayer, 0.0f);
}

// バトルカメラへ切り替え
ACameraActor* UCameraManager::ChangeBattleCamera(EBattleCameraType type)
{
    APlayerController* playerController =
        UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!playerController) return nullptr;

    ABattleAreaManager* area = GetBattleAreaManager();
    if (!area) return nullptr;

    ACameraActor* camera = area->GetCamera(type);
    if (!camera) return nullptr;

    playerController->SetViewTargetWithBlend(camera, 1.0f);
    return camera;
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
