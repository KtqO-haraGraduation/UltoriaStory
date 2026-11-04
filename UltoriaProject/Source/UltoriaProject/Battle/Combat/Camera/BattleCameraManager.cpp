// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCameraManager.h"
#include "../Turn/BattleTurnManager.h"
#include "../Data/BattleParameter.h"
#include "../../BattleAreaManager.h"
#include "../../BattleManager.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Camera/CameraManager.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"

constexpr float CAMERA_UPVEC_Z = 300.0f;
constexpr float CAMERA_OFFSET_Y_ALLY  = 600.0f;
constexpr float CAMERA_OFFSET_Y_ENEMY = 1500.0f;

void UBattleCameraManager::Init(UBattleManager* battleMg)
{
	if (!battleMg) return;
	m_BattleManager = battleMg;
}

void UBattleCameraManager::SetActionCameraPos()
{
	if (!m_BattleManager) return;

	// 現在行動してるキャラクターの前方へカメラを移動
	UBattleTurnManager* turnMg = m_BattleManager->GetTurnManager();
	if (!turnMg) return;

	ACharacterBase* currentCharacter = turnMg->GetCurrentCharacter();
	if (!currentCharacter) return;

	if (currentCharacter->GetTeamType() == ETeamType::PLAYER)
	{
		// プレイヤーターンはアクションカメラ
		FVector cameraPos;
		FRotator cameraRot;

		GetCharacterCameraTransform(
			currentCharacter,
			cameraPos,
			cameraRot);

		ACameraActor* camera = ChangeCamera(EBattleCameraType::ACTION);

		SetCameraTransform(camera, cameraPos, cameraRot);
	}
	else if (currentCharacter->GetTeamType() == ETeamType::ENEMY)
	{
		// メインカメラに切り替え
		ACameraActor* camera = ChangeCamera(EBattleCameraType::MAIN);

		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		playerController->SetViewTargetWithBlend(camera, 0.0f);
	}
}

void UBattleCameraManager::SetTargetCameraPos(ETargetTeam team, const TArray<ACharacterBase*>& targets)
{
	if (targets.IsEmpty()) return;

	// 現在行動してるキャラクターの前方へカメラを移動
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	UCameraManager* cameraMg = gameInstance->GetSubsystem<UCameraManager>();

	if (team == ETargetTeam::SELF)
	{
		// プレイヤーを映す
		FVector cameraPos;
		FRotator cameraRot;

		GetCharacterCameraTransform(
			targets[0],
			cameraPos,
			cameraRot);

		ACameraActor* camera = ChangeCamera(EBattleCameraType::ACTION);

		SetCameraTransform(camera, cameraPos, cameraRot);
	}
	else if (team == ETargetTeam::ALLY)
	{
		// 仲間全体を映す

	}
	else if (team == ETargetTeam::ENEMY)
	{
		// エネミー全体を映す
		FVector centerPos;

		if (!GetTargetsCenter(targets, centerPos))
			return;

		// 先頭の有効なターゲットの向きを基準にする
		ACharacterBase* firstTarget = nullptr;

		for (ACharacterBase* target : targets)
		{
			if (target)
			{
				firstTarget = target;
				break;
			}
		}

		if (!firstTarget) return;

		FVector forward = firstTarget->GetActorForwardVector();
		FVector upVec(0.0f, 0.0f, CAMERA_UPVEC_Z);

		FVector cameraPos =
			centerPos + ((forward * CAMERA_OFFSET_Y_ENEMY) + upVec);

		FVector direction = centerPos - cameraPos;
		FRotator cameraRot = direction.Rotation();

		ACameraActor* camera = ChangeCamera(EBattleCameraType::ACTION);

		SetCameraTransform(camera, cameraPos, cameraRot);
	}
}

ACameraActor* UBattleCameraManager::ChangeCamera(EBattleCameraType type)
{
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return nullptr;

	UCameraManager* cameraMg = gameInstance->GetSubsystem<UCameraManager>();
	if (!cameraMg) return nullptr;

	return cameraMg->ChangeBattleCamera(type);
}

void UBattleCameraManager::SetCameraTransform(ACameraActor* camera, const FVector& pos, const FRotator& rot)
{
	if (!camera) return;

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!playerController) return;

	// カメラの位置と向きを設定
	camera->SetActorLocation(pos);
	camera->SetActorRotation(rot);

	// カメラを切り替える
	playerController->SetViewTargetWithBlend(camera, 0.0f);
}

void UBattleCameraManager::GetCharacterCameraTransform(ACharacterBase* character, FVector& pos, FRotator& rot)
{
	if (!character) return;

	FVector characterPos = character->GetActorLocation();
	FVector forward = character->GetActorForwardVector();

	FVector upVec(0.0f, 0.0f, CAMERA_UPVEC_Z);

	// キャラクターの前方にカメラを配置
	pos = characterPos + ((forward * CAMERA_OFFSET_Y_ALLY) + upVec);

	// キャラクターを見る
	FVector dir = characterPos - pos;
	rot = dir.Rotation();
}

bool UBattleCameraManager::GetTargetsCenter(const TArray<ACharacterBase*>& targets, FVector& center)
{
	center = FVector::ZeroVector;

	int targetCount = 0;

	for (ACharacterBase* target : targets)
	{
		if (!target) continue;

		center += target->GetActorLocation();
		targetCount++;
	}

	if (targetCount == 0) return false;

	center /= targetCount;

	return true;
}
