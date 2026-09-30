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
	// ¶¬‚Ì‰Šú‰»ˆ—
	virtual void NativeConstruct() override;
	// XVˆ—
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void InitMap();

	void SetStageMosts(TArray<AActor*> stageMosts) { m_StageMosts = stageMosts; }
	
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
};
