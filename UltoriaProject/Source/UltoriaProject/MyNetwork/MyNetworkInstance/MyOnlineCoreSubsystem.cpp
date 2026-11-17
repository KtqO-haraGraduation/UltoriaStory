// Fill out your copyright notice in the Description page of Project Settings.

#include "MyOnlineCoreSubsystem.h"


void UMyOnlineCoreSubsystem::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful,
	const FUniqueNetId& UserId, const FString& Error)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		bWasSuccessful ? FColor::Green : FColor::Red,
		bWasSuccessful ? TEXT("EOS Login Success") : Error
	);
}

FOnlineSessionSettings UMyOnlineCoreSubsystem::MakeSessionSettings()
{
	FOnlineSessionSettings Settings;

	Settings.NumPublicConnections = 4;

	Settings.bShouldAdvertise = true;
	Settings.bUsesPresence = false;
	Settings.bUseLobbiesIfAvailable = false;
	Settings.bAllowJoinViaPresence = false;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = false;

	return Settings;
}

void UMyOnlineCoreSubsystem::OnFindComplete(bool bSuccess)
{
	FString Text = FString::Printf(
		TEXT("Find Success=%d Results=%d"),
		bSuccess,
		m_SessionSearch->SearchResults.Num()
	);

	GEngine->AddOnScreenDebugMessage(
		-1,
		10.f,
		FColor::Yellow,
		Text
	);

	// 入れる部屋がないなら作る
	if (!bSuccess || m_SessionSearch->SearchResults.Num() == 0)
	{
		CreateSession();
		return;
	}

	// 入れる部屋があれば入る
	for (const FOnlineSessionSearchResult& Result : m_SessionSearch->SearchResults)
	{
		FString RoomID;
		Result.Session.SessionSettings.Get(TEXT("ROOM_ID"), RoomID);

		if (!RoomID.IsEmpty())
			continue;

		if (Result.IsValid())
		{
			JoinSession(Result);
			return;
		}
	}

	// 入れる部屋がないなら作る
	CreateSession();
}

void UMyOnlineCoreSubsystem::CreateSession()
{
	ULocalPlayer* LP = GetGameInstance()->GetFirstGamePlayer();

	if (!LP)
	{
		return;
	}

	if (!LP->GetPreferredUniqueNetId().IsValid())
	{
		return;
	}

	if (m_SessionInterface->GetNamedSession(NAME_GameSession))
	{
		m_SessionInterface->DestroySession(NAME_GameSession);
	}

	FOnlineSessionSettings Settings;
	Settings = MakeSessionSettings();

	// 作り終えたら呼ぶ関数を設定
	m_SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(
			this,
			&UMyOnlineCoreSubsystem::OnCreateComplete
		)
	);

	// Settingsで決めた設定で部屋を作成
	m_SessionInterface->CreateSession(
		*LP->GetPreferredUniqueNetId(),
		NAME_GameSession,
		Settings
	);
}

void UMyOnlineCoreSubsystem::OnCreateComplete(FName SessionName, bool bSuccess)
{
	if (!bSuccess)return;

	// 指定したレベルに移動
	GetWorld()->ServerTravel(TEXT("/Game/Lv/Lv_Room?listen"));
}

void UMyOnlineCoreSubsystem::JoinSession(const FOnlineSessionSearchResult& Result)
{
	ULocalPlayer* LP = GetGameInstance()->GetFirstGamePlayer();

	if (!LP)
	{
		return;
	}

	if (!LP->GetPreferredUniqueNetId().IsValid())
	{
		return;
	}

	// 入り終えたら呼ぶ関数を設定
	m_SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(
			this,
			&UMyOnlineCoreSubsystem::OnJoinComplete
		)
	);

	// セッションに入る
	m_SessionInterface->JoinSession(
		*LP->GetPreferredUniqueNetId(),
		NAME_GameSession,
		Result
	);
}

void UMyOnlineCoreSubsystem::OnJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Address;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (!PC)
	{
		return;
	}

	if (m_SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		// clientとして移動
		PC->ClientTravel(Address, TRAVEL_Relative);
	}
}

void UMyOnlineCoreSubsystem::OnFindBeforeCreate(bool bSuccess)
{
	if (!bSuccess)return;

	// 同じIDの部屋がないか検索
	for (const FOnlineSessionSearchResult& Result : m_SessionSearch->SearchResults)
	{
		FString FoundRoomID;

		Result.Session.SessionSettings.Get(TEXT("ROOM_ID"), FoundRoomID);

		if (FoundRoomID == m_TargetRoomID)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Red,
					FString::Printf(TEXT("CreateRoom failed"))
				);
			}
			// あれば作らない
			return;
		}
	}

	// なければ作る
	CreateRoom_Internal(m_TargetRoomID);
}

void UMyOnlineCoreSubsystem::CreateRoom_Internal(const FString& roomId)
{
	ULocalPlayer* LP = GetGameInstance()->GetFirstGamePlayer();

	if (!LP)
	{
		return;
	}

	if (!LP->GetPreferredUniqueNetId().IsValid())
	{
		return;
	}

	if (m_SessionInterface->GetNamedSession(NAME_GameSession))
	{
		m_SessionInterface->DestroySession(NAME_GameSession);
	}

	FOnlineSessionSettings Settings;

	Settings = MakeSessionSettings();

	// roomIDの設定
	Settings.Set(
		FName(TEXT("ROOM_ID")),
		roomId,
		EOnlineDataAdvertisementType::ViaOnlineService
	);

	// セッションを作り終えたら呼ぶ関数を設定
	m_SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(
			this,
			&UMyOnlineCoreSubsystem::OnCreateComplete
		)
	);

	// セッションを作成
	m_SessionInterface->CreateSession(
		*LP->GetPreferredUniqueNetId(),
		NAME_GameSession,
		Settings
	);
}

void UMyOnlineCoreSubsystem::OnFindCompleteRoom(bool bSuccess)
{
	if (!bSuccess)return;

	// 設定したroomIDと同じ部屋があれば入る
	for (const FOnlineSessionSearchResult& Result : m_SessionSearch->SearchResults)
	{
		FString FoundRoomID;

		Result.Session.SessionSettings.Get(TEXT("ROOM_ID"), FoundRoomID);

		if (FoundRoomID == m_TargetRoomID)
		{
			JoinSession(Result);
			return;
		}
	}
}

void UMyOnlineCoreSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		pc->ClientTravel(TEXT("/Game/MembarFiles/kamei/Lv/DLv_NetworkTest"), TRAVEL_Absolute);
	}
}
