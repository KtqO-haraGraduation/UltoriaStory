// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UEffectManager::LoadEffect()
{
	m_EffectList.Empty();

	FName effectPath = TEXT("/Game/Effect");

	// アセットレンジストリを取得
	FAssetRegistryModule& assetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	// エフェクトのアセットを検索して格納
	TArray<FAssetData> effectAssets;
	
	assetRegistry.GetAssetsByPath(
		effectPath,
		effectAssets,
		true
	);

	// 検索したエフェクトをリストに登録
	for (const FAssetData& asset : effectAssets)
	{
		UNiagaraSystem* effect = Cast<UNiagaraSystem>(asset.GetAsset());
		if (!effect) continue;

		m_EffectList.Add(asset.AssetName, effect);
	}
}

void UEffectManager::PlayEffect(FName name, FVector location)
{
	UNiagaraSystem* effect = FindEffect(name);
	if (!effect) return;

	// エフェクトを再生（指定された場所で）
	UNiagaraComponent* component =
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			effect,
			location,
			FRotator::ZeroRotator
		);

	// 現在再生中のリストに追加
	if (component)
	{
		m_PlayingEffectList.Add(component);
	}
}

void UEffectManager::PlayEffectAttached(FName name, AActor* actor)
{
	if (!actor) return;
	UNiagaraSystem* effect = FindEffect(name);
	if (!effect) return;

	// エフェクトを再生（追従）
	UNiagaraComponent* component =
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			effect,
			actor->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

	// 現在再生中のリストに追加
	if (component)
	{
		m_PlayingEffectList.Add(component);

		// 終了イベント登録
		component->OnSystemFinished.AddDynamic(
			this,
			&UEffectManager::OnEffectFinished
		);
	}
}

void UEffectManager::StopEffect(FName name)
{

}

void UEffectManager::StopAllEffect()
{
	for (UNiagaraComponent* effect : m_PlayingEffectList)
	{
		if (!IsValid(effect)) continue;
		effect->Deactivate();
	}
	m_PlayingEffectList.Empty();
}

UNiagaraSystem* UEffectManager::FindEffect(FName name)
{
	return m_EffectList.FindRef(name);
}

void UEffectManager::OnEffectFinished(UNiagaraComponent* effect)
{
	m_PlayingEffectList.Remove(effect);
}
