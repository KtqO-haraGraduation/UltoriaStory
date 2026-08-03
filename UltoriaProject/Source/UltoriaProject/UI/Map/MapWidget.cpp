// Fill out your copyright notice in the Description page of Project Settings.
#include "MapWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Battle/Character/TestPlayer/TestPlayerMC.h"
#include "Kismet/GameplayStatics.h"

constexpr float STAGE_LEFT_MOST  = -1580.0f; // ステージの左端
constexpr float STAGE_RIGHT_MOST =  4595.0f; // 右
constexpr float STAGE_FRONT_MOST = -4900.0f; // 下
constexpr float STAGE_BACK_MOST  =  7900.0f; // 上

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

    m_MapSlot = Cast<UCanvasPanelSlot>(MapImage->Slot);
    if (m_MapSlot)
    {
        m_MapImagePos  = m_MapSlot->GetPosition();
        m_MapImageSize = m_MapSlot->GetSize();
    }
    m_PlayerSlot = Cast<UCanvasPanelSlot>(PlayerImage->Slot);
    
    InitMap();
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMapPlayer();
}

void UMapWidget::InitMap()
{
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("StageMost")), m_StageMosts);

}

void UMapWidget::UpdateMapPlayer()
{
    AMainPlayer* player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!player)
    {
        return;
    }

    FVector playerLocation = player->GetActorLocation();

    // ステージの左または下端の座標を0とし、プレイヤーの進行度とステージ全体の幅を計算する。
    // マップ上のプレイヤー座標 = プレイヤーが左または下端から進んだ距離 / ステージの幅 * マップの幅
    const float mapPlayerX = (playerLocation.X - STAGE_LEFT_MOST)  / (STAGE_RIGHT_MOST - STAGE_LEFT_MOST) * m_MapImageSize.X;
    const float mapPlayerY = (playerLocation.Y - STAGE_FRONT_MOST) / (STAGE_BACK_MOST - STAGE_FRONT_MOST) * m_MapImageSize.Y;

    if (m_PlayerSlot)
    {
        m_PlayerSlot->SetPosition(FVector2D(mapPlayerX + m_MapImagePos.X, mapPlayerY + m_MapImagePos.Y));
    }
}