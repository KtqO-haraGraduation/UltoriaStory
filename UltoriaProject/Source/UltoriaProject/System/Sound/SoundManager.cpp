// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"

void USoundManager::LoadSoundGroup(ESoundType type)
{
	m_BGMList.Empty();
	m_SEList.Empty();

	FString typeName;

	switch (type)
	{
	case ESoundType::TITLE:
		typeName = TEXT("Title");
		break;
	case ESoundType::PLAY:
		typeName = TEXT("Play");
		break;
	case ESoundType::GAME_OVER:
		typeName = TEXT("GameOver");
		break;
	case ESoundType::CLEAR:
		typeName = TEXT("Clear");
		break;
	}

	// Commonフォルダのパス
	FString commonBGMPath = TEXT("/Game/Audio/BGM/Common");
	FString commonSEPath = TEXT("/Game/Audio/SE/Common");

	// 指定グループのフォルダのパス
	FString groupBGMPath = TEXT("/Game/Audio/BGM/") + typeName;
	FString groupSEPath  = TEXT("/Game/Audio/SE/")  + typeName;

	// アセットレンジストリを取得
	// アセットレンジストリ : アセットを検索・管理するための情報を取得する
	FAssetRegistryModule& assetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	TArray<FAssetData> bgmAssets;
	TArray<FAssetData> seAssets;

	// CommonのBGMを検索して格納
	assetRegistry.GetAssetsByPath(
		FName(*commonBGMPath),
		bgmAssets
	);

	// CommonのSEを検索して格納
	assetRegistry.GetAssetsByPath(
		FName(*commonSEPath),
		seAssets
	);

	// 指定グループのBGMを検索して格納
	assetRegistry.GetAssetsByPath(
		FName(*groupBGMPath),
		bgmAssets,
		true
	);

	// 指定グループのSEを検索して格納
	assetRegistry.GetAssetsByPath(
		FName(*groupSEPath),
		seAssets,
		true
	);

	// 検索したBGMをBGMリストに登録
	for (const FAssetData& asset : bgmAssets)
	{
		USoundBase* sound = Cast<USoundBase>(asset.GetAsset());
		if (!sound) continue;

		m_BGMList.Add(asset.AssetName, sound);
	}

	// 検索したSEをSEリストに登録
	for (const FAssetData& asset : seAssets)
	{
		USoundBase* sound = Cast<USoundBase>(asset.GetAsset());
		if (!sound) continue;

		m_SEList.Add(asset.AssetName, sound);
	}
}

void USoundManager::PlayBGM(FName name, float fadeTime)
{
	USoundBase* sound = FindBGM(name);
	if (!sound) return;

	// 現在のBGMを停止
	StopBGM(fadeTime);

	// 新しいBGMを再生
	m_CurrentBGM = UGameplayStatics::SpawnSound2D(
		this,
		sound,
		m_BGMVolume
	);

	if (m_CurrentBGM)
	{
		m_CurrentBGM->FadeIn(fadeTime, m_BGMVolume);
	}
}

void USoundManager::StopBGM(float fadeTime)
{
	if (!m_CurrentBGM) return;

	m_CurrentBGM->FadeOut(fadeTime, 0.0f);
	m_CurrentBGM = nullptr;
}

void USoundManager::PlaySE(FName name)
{
	USoundBase* sound = FindSE(name);
	if (!sound) return;

	UGameplayStatics::PlaySound2D(
		this,
		sound,
		m_SEVolume
	);
}

USoundBase* USoundManager::FindBGM(FName name)
{
	return m_BGMList.FindRef(name);
}

USoundBase* USoundManager::FindSE(FName name)
{
	return m_SEList.FindRef(name);
}

float USoundManager::GetBGMVolume() const
{
	return m_BGMVolume;
}

void USoundManager::SetBGMVolume(float volume)
{
	m_BGMVolume = FMath::Clamp(volume, 0.0f, 1.0f);
}

float USoundManager::GetSEVolume() const
{
	return m_SEVolume;
}

void USoundManager::SetSEVolume(float volume)
{
	m_SEVolume = FMath::Clamp(volume, 0.0f, 1.0f);
}