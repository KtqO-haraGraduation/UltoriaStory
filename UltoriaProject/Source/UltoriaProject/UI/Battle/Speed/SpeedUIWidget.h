// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleUIBase.h"
#include "Components/CanvasPanel.h"
#include "SpeedUIWidget.generated.h"

class USpeedWidget;
class ACharacterBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API USpeedUIWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// UIを新しく生成
	void CreateSpeedWidget(ACharacterBase* character);
	void CreateSpeedWidgets();

	// スピードUIのデータを設定
	void SetSpeedWidgetData();
	// 指定したIndexのUIを非表示にする
	void SetHideSpeedUI(int index);

	// UIを削除
	void RemoveSpeedWidget(USpeedWidget* widget);
	void ResetSpeedWidget();

	const TArray<USpeedWidget*>& GetSpeedWodgets() const { return m_SpeedWidgets; }

private:
	TArray<USpeedWidget*> m_SpeedWidgets;

	float m_Width = 30.0f;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SpeedPanel;
};
