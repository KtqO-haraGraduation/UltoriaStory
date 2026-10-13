// Fill out your copyright notice in the Description page of Project Settings.

#include "MyOnlineSubsystem.h"
#include "MyOnlineCoreSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

constexpr const int MAX_SEARCHRESULTS_NUM = 100;

void UMyOnlineSubsystem::Login()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	UMyOnlineCoreSubsystem* core = GetGameInstance()->GetSubsystem<UMyOnlineCoreSubsystem>();

	if (Subsystem && core)
	{
		core->m_SessionInterface = Subsystem->GetSessionInterface();

		IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

		if (Identity.IsValid())
		{
			// ログイン成功時に呼ばれる関数を設定
			Identity->AddOnLoginCompleteDelegate_Handle(
				0,
				FOnLoginCompleteDelegate::CreateUObject(
					core,
					&UMyOnlineCoreSubsystem::OnLoginComplete
				)
			);

			FOnlineAccountCredentials Creds;
			Creds.Type = TEXT("accountportal");
			// アカウントポータルを使いログイン
			Identity->Login(0, Creds);
		}
	}
}

void UMyOnlineSubsystem::FindSession()
{
	ULocalPlayer* LP = GetGameInstance()->GetFirstGamePlayer();
	UMyOnlineCoreSubsystem* core = GetGameInstance()->GetSubsystem<UMyOnlineCoreSubsystem>();

	if (!LP || !core)
	{
		return;
	}

	if (!LP->GetPreferredUniqueNetId().IsValid())
	{
		return;
	}

	core->m_SessionSearch = MakeShareable(new FOnlineSessionSearch());

	core->m_SessionSearch->MaxSearchResults = MAX_SEARCHRESULTS_NUM;

	core->m_SessionSearch->bIsLanQuery = false;

	// 探し終えたら呼ぶ関数を設定
	core->m_SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(
			core,
			&UMyOnlineCoreSubsystem::OnFindComplete
		)
	);

	// セッションを探す
	core->m_SessionInterface->FindSessions(
		*LP->GetPreferredUniqueNetId(),
		core->m_SessionSearch.ToSharedRef()
	);
}

void UMyOnlineSubsystem::CreateRoom(const FString& roomId)
{
	ULocalPlayer* LP = GetGameInstance()->GetFirstGamePlayer();
	UMyOnlineCoreSubsystem* core = GetGameInstance()->GetSubsystem<UMyOnlineCoreSubsystem>();

	if (!LP || !core)
	{
		return;
	}

	if (!LP->GetPreferredUniqueNetId().IsValid())
	{
		return;
	}
	// roomIDの設定(数字のみ(bluePrint側))
	core->m_TargetRoomID = roomId;

	core->m_SessionSearch = MakeShareable(new FOnlineSessionSearch());
	core->m_SessionSearch->MaxSearchResults = MAX_SEARCHRESULTS_NUM;

	// 探し終えたら呼ぶ関数を設定
	core->m_SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(
			core,
			&UMyOnlineCoreSubsystem::OnFindBeforeCreate
		)
	);

	// セッションを探す
	core->m_SessionInterface->FindSessions(
		*LP->GetPreferredUniqueNetId(),
		core->m_SessionSearch.ToSharedRef()
	);
}

void UMyOnlineSubsystem::JoinRoom(const FString& roomId)
{
	ULocalPlayer* LP = GetGameInstance()->GetFirstGamePlayer();
	UMyOnlineCoreSubsystem* core = GetGameInstance()->GetSubsystem<UMyOnlineCoreSubsystem>();

	if (!LP || !core)
	{
		return;
	}

	if (!LP->GetPreferredUniqueNetId().IsValid())
	{
		return;
	}

	core->m_SessionSearch = MakeShareable(new FOnlineSessionSearch());

	core->m_SessionSearch->MaxSearchResults = MAX_SEARCHRESULTS_NUM;
	core->m_SessionSearch->bIsLanQuery = false;

	// 入り終えたら呼ぶ関数を設定
	core->m_SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(
			core,
			&UMyOnlineCoreSubsystem::OnFindCompleteRoom
		)
	);

	core->m_TargetRoomID = roomId;

	// セッションに入る
	core->m_SessionInterface->FindSessions(
		*LP->GetPreferredUniqueNetId(),
		core->m_SessionSearch.ToSharedRef()
	);
}

void UMyOnlineSubsystem::LeaveSession()
{
	UMyOnlineCoreSubsystem* core = GetGameInstance()->GetSubsystem<UMyOnlineCoreSubsystem>();

	if (!core)
	{
		return;
	}

	// セッションから抜けるのに成功したら呼ばれる関数を設定
	core->m_SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
		FOnDestroySessionCompleteDelegate::CreateUObject(
			core,
			&UMyOnlineCoreSubsystem::OnDestroySessionComplete
		)
	);

	// セッションを破棄
	core->m_SessionInterface->DestroySession(NAME_GameSession);
}
