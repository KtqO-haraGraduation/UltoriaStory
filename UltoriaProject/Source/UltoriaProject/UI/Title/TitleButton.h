// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "TitleButton.generated.h"

class UButton;
class UImage;
class UTexture2D;

UENUM(BlueprintType)
enum class ETitleButtonAction : uint8
{
	NewGame,
	Continue,
	Settings,
	Quit
};

UCLASS()
class ULTORIAPROJECT_API UTitleButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintCallable)
	void HandleHovered();

	UFUNCTION(BlueprintCallable)
	void HandleClicked();

public:
	// ボタンの種類(Details で選ぶ)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title")
	ETitleButtonAction m_Action = ETitleButtonAction::NewGame;

	// m_StartLevelName を消して、これに替える
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title")
	TSoftObjectPtr<UWorld> m_StartLevel;

	// ボタンごとに Details で設定する文字画像
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title")
	TObjectPtr<UTexture2D> m_LabelTexture;

	// 通常時と選択中の色味(画像に乗算される)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title")
	FLinearColor m_NormalLabelColor = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Title")
	FLinearColor m_FocusedLabelColor = FLinearColor::White;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> m_Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> m_LabelImage;


private:
	FButtonStyle m_DefaultStyle;
};
