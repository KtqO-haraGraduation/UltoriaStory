// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EffectManager.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UEffectManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// エフェクトの読み込み
	UFUNCTION(BlueprintCallable)
	void LoadEffect();

	// 指定位置で再生
	void PlayEffect(FName name, FVector location);
	// アクターに追従して再生
	void PlayEffectAttached(FName name, AActor* actor);

	void StopEffect(FName name);
	void StopAllEffect();


private:
	// 検索
	UNiagaraSystem* FindEffect(FName name);

	UFUNCTION()
	void OnEffectFinished(UNiagaraComponent* effect);

private:
	TMap<FName, UNiagaraSystem*> m_EffectList;		// 読み込んだEffect
	TArray<UNiagaraComponent*> m_PlayingEffectList;	// 現在再生中のエフェクト
};
