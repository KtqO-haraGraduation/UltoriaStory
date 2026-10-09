// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MyOnlineCoreSubsystem.generated.h"

class UMyOnlineSubsystem;

UCLASS()
class ULTORIAPROJECT_API UMyOnlineCoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	friend class UMyOnlineSubsystem;

private:
    // Session操作本体
    IOnlineSessionPtr m_SessionInterface;

    // 検索結果
    TSharedPtr<FOnlineSessionSearch> m_SessionSearch;

    void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful,
        const FUniqueNetId& UserId, const FString& Error);

    // セッションの設定
    FOnlineSessionSettings MakeSessionSettings();

    // ホスト作成
    void CreateSession();

    // 部屋参加
    void JoinSession(const FOnlineSessionSearchResult& Result);

    // 検索完了
    void OnFindComplete(bool bSuccess);

    // 作成完了
    void OnCreateComplete(FName SessionName, bool bSuccess);

    // 参加完了
    void OnJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    void OnFindBeforeCreate(bool bSuccess);
    void CreateRoom_Internal(const FString& roomId);

    void OnFindCompleteRoom(bool bSuccess);

    // セッションの破棄に成功
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    FString m_TargetRoomID;
};
