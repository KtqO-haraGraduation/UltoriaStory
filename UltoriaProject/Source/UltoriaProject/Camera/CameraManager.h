// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CameraManager.generated.h"

enum class EBattleCameraType : uint8;
class ACameraActor;
class ABattleAreaManager;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCameraManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// --- バトル関連 --- //
	// フィールドカメラへ切り替え
	void ChangeFieldCamera();
	// バトルカメラの切り替え
	ACameraActor* ChangeBattleCamera(EBattleCameraType type);


	// バトルエリアを取得
	ABattleAreaManager* GetBattleAreaManager();
	// 現在のカメラを取得
	ACameraActor* GetCurrentCamera();
};
