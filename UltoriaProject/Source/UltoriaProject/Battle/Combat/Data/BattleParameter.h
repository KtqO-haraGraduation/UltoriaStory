// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

// ターゲットチーム
UENUM(BlueprintType)
enum class ETargetTeam : uint8
{
	SELF,	// プレイヤー
	ALLY,	// 仲間
	ENEMY,	// 敵
};

// ターゲットの範囲
UENUM(BlueprintType)
enum class ETargetType : uint8
{
	SINGLE,	// 単体
	ALL,	// 全員
};
