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
        m_MapImagePos = m_MapSlot->GetPosition();
        m_MapImageSize = m_MapSlot->GetSize();
    }
    m_PlayerSlot = Cast<UCanvasPanelSlot>(PlayerImage->Slot);
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMapPlayer();
}

void UMapWidget::InitMap(float mapTexDiameterLength, FVector2D marginHarfLength)
{
    m_MapTexDiameterLength = mapTexDiameterLength;

    m_MarginLength.X = marginHarfLength.X * 2;
    m_MarginLength.Y = marginHarfLength.Y * 2;

    // Actor取得
    AddStageMosts(FName(TEXT("StageLeftMost")));
    AddStageMosts(FName(TEXT("StageRightMost")));
    AddStageMosts(FName(TEXT("StageFrontMost")));
    AddStageMosts(FName(TEXT("StageBackMost")));
}

void UMapWidget::UpdateMapPlayer()
{
    AMainPlayer* player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!player)
    {
        return;
    }

    FVector playerLocation = player->GetActorLocation();
    const float stageLeftMost  = m_StageMosts[0]->GetActorLocation().X;
    const float stageRightMost = m_StageMosts[1]->GetActorLocation().X;
    const float stageFrontMost = m_StageMosts[2]->GetActorLocation().Y;
    const float stageBackMost  = m_StageMosts[3]->GetActorLocation().Y;

    // WBPのマップ本来の幅 = WBP側の直径 - WBP側の余白の直径
    // WBP側の余白の直径 = Tex側の余白の直径 * (WBP側の直径 / Tex側の直径)
    const float mapWidth  = m_MapImageSize.X - (m_MarginLength.X * (m_MapImageSize.X / m_MapTexDiameterLength));
    const float mapHeight = m_MapImageSize.Y - (m_MarginLength.Y * (m_MapImageSize.Y / m_MapTexDiameterLength));

    // プレイヤーの進行度とステージ全体の幅を計算する。
    // マップ上のプレイヤー座標 = プレイヤーが進んだ距離 / ステージの直径 * マップの調整
    const float mapPlayerX = ((playerLocation.X - stageLeftMost) / (stageRightMost - stageLeftMost) - 0.5f) * mapWidth;
    const float mapPlayerY = ((playerLocation.Y - stageBackMost) / (stageFrontMost - stageBackMost) - 0.5f) * mapHeight;

    if (m_PlayerSlot)
    {
        m_PlayerSlot->SetPosition(FVector2D(mapPlayerX + m_MapImagePos.X, mapPlayerY + m_MapImagePos.Y));
        //UE_LOG(LogTemp, Warning, TEXT("Player Location : X=%f, Y=%f"), m_PlayerSlot->GetPosition().X,m_PlayerSlot->GetPosition().Y);
        //UE_LOG(LogTemp, Warning, TEXT("MapImageSize : X=%f, Y=%f"), m_MapImageSize.X, m_MapImageSize.Y);
    }
}

void UMapWidget::AddStageMosts(FName tag)
{
    // m_StageMostsは以下のタグをまとめた配列であるため、
    // 引数に入れると取得関数を呼ぶたびに毎回リセットされてしまう
    // そのため、temp用のローカル変数を用意する
    TArray<AActor*> tempStageMosts;
    // タグ名(FName)が付いたActorを取得
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), tag, tempStageMosts);
    // 取得したアクターをm_StageMostにAppend
    m_StageMosts.Append(tempStageMosts);
}