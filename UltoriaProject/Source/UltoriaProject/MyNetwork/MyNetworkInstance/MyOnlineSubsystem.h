// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyOnlineSubsystem.generated.h"

//class AMyNetworkPlayerController;

UCLASS()
class ULTORIAPROJECT_API UMyOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	/*friend class AMyNetworkPlayerController;*/

private:
	// EOSログイン
	void Login();

	// ランダムマッチ
	void FindSession();

	// 部屋を作る
	void CreateRoom(const FString& roomId);

	// 部屋に入る
	void JoinRoom(const FString& roomId);

	// セッションから抜ける
	void LeaveSession();
};
