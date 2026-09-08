// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "SoundManager.generated.h"

class UAudioComponent;

// サウンドのグループ
UENUM(BlueprintType)
enum class ESoundType : uint8
{
	COMMON,		// 共通サウンド
	TITLE,
	PLAY,
	GAME_OVER,
	CLEAR,
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	// Sound 読み込み
	UFUNCTION(BlueprintCallable)
	void LoadSoundGroup(ESoundType type);

	// BGM
	UFUNCTION(BlueprintCallable)
	void PlayBGM(FName name, float fadeTime = 1.0f);
	void StopBGM(float fadeTime = 1.0f);

	// SE
	void PlaySE(FName name);

	// 検索・取得
	USoundBase* FindBGM(FName name);
	USoundBase* FindSE(FName name);

	float GetBGMVolume() const;
	void SetBGMVolume(float volume);

	float GetSEVolume() const;
	void SetSEVolume(float volume);

private:
	float m_BGMVolume = 0.5f;
	float m_SEVolume  = 0.2f;

	UPROPERTY()
	TObjectPtr<UAudioComponent> m_CurrentBGM;

	TMap<FName, USoundBase*> m_BGMList;
	TMap<FName, USoundBase*> m_SEList;
};
