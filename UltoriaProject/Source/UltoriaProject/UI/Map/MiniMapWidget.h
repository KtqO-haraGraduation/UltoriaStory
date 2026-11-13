// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

class URetainerBox;
class UImage;
class UCanvasPanel;
class UCanvasPanelSlot;
class AEnemyBase;

/*
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 生成時の初期化処理
	virtual void NativeConstruct() override;
	// 更新処理
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void InitMap(float mapTexDiameterLength, FVector2D marginLength);

private:
	void InitEnemySlot(TArray<AEnemyBase*> enemies);

	void UpdateMapPlayer();
	void UpdateMapEnemy();

	void AddStageMosts(FName tag);

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapCanvas;
	UPROPERTY(meta = (BindWidget))
	URetainerBox* RetainerBox;
	UPROPERTY(meta = (BindWidget))
	UImage* MapImage;
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerImage;
	UPROPERTY(meta = (BindWidget))
	UImage* EnemyImage;

	TArray<AActor*> m_StageMosts;
	UCanvasPanelSlot* m_RB_Slot;
	UCanvasPanelSlot* m_MapSlot;
	TMap<AEnemyBase*, UCanvasPanelSlot*> m_EnemySlots;
	FVector2D m_MapImageCenterPos;	// RetainerBoxの中心座標
	FVector2D m_MapImageSize;
	FVector2D m_UVMapCenter;
	// テクスチャの直径の長さ
	// 縦と横の直径の長さは同じなので、変数はfloat型
	float m_MapTexDiameterLength;
	// 余白の長さ
	FVector2D m_MarginLength;
	// マップの状態をマテリアルグラフに渡す変数
	UMaterialInstanceDynamic* m_MID;
	// UV図形の描画範囲
	float m_DrawRangeArea;

	float m_PlayerInitYaw;

	bool m_InitEnemySlot;
};