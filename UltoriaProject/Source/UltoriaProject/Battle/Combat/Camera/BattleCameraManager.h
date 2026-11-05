// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleCameraManager.generated.h"

class ACameraActor;
class ACharacterBase;
class UBattleManager;
enum class EBattleCameraType : uint8;
enum class ETargetTeam : uint8;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleCameraManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UBattleManager* battleMg);
	void SetActionCameraPos();
	void SetTargetCameraPos(ETargetTeam team, const TArray<ACharacterBase*>& targets);

private:
	// カメラを取得して切り替え
	ACameraActor* ChangeCamera(EBattleCameraType type);

	// カメラの位置・回転を設定して切り替え
	void SetCameraTransform(ACameraActor* camera, const FVector& pos, const FRotator& rot);

	// キャラクターを映すカメラ位置・回転を取得
	void GetCharacterCameraTransform(ACharacterBase* character, FVector& pos, FRotator& rot);

	// ターゲット全体の中央位置を取得
	bool GetTargetsCenter(const TArray<ACharacterBase*>& targets, FVector& center);

private:
	UBattleManager* m_BattleManager;
};
