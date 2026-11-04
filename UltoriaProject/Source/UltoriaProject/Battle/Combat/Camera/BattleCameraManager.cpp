// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCameraManager.h"
#include "../../BattleAreaManager.h"
#include "../../BattleManager.h"
#include "../Turn/BattleTurnManager.h"
#include "../../../Character/CharacterBase.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"

void UBattleCameraManager::Init(UBattleManager* battleMg)
{
	if (!battleMg) return;
	m_BattleManager = battleMg;
}

void UBattleCameraManager::SetActionCameraPos()
{
	// 現在行動してるキャラクターの前方へカメラを移動
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	UBattleTurnManager* turnMg = battleMg->GetTurnManager();
	if (!turnMg) return;

	ACharacterBase* currentCharacter = turnMg->GetCurrentCharacter();
	if (!currentCharacter) return;



}
