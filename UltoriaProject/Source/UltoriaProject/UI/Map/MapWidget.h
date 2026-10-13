// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

class UImage;
class UCanvasPanelSlot;
/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMapWidget : public UUserWidget
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
	void UpdateMapPlayer();

	void AddStageMosts(FName tag);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* MapImage;
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerImage;

	TArray<AActor*> m_StageMosts;
	UCanvasPanelSlot* m_MapSlot;
	UCanvasPanelSlot* m_PlayerSlot;
	FVector2D m_MapImagePos;
	FVector2D m_MapImageSize;

	// テクスチャの直径の長さ
	// 縦と横の直径の長さは同じなので、変数はfloat型
	float m_MapTexDiameterLength;
	// 余白の長さ
	FVector2D m_MarginLength;
};
