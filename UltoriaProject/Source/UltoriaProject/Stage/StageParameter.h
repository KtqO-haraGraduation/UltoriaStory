// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageParameter.generated.h"

UENUM()
enum class EStageType : uint8
{
	TUTORIAL,		// チュートリアル
	FOREST,			// 森（ステージ１）
	CAVE,			// 洞窟（ステージ２）
	POISON_SWAMP,	// 毒の沼（ステージ３）
	CASTLE,			// 城（ステージ４）
	TEST,			// 自分のLv（テスト）←後で消す
};
