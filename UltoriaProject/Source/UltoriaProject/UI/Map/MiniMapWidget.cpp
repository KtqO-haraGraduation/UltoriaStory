// Fill out your copyright notice in the Description page of Project Settings.
#include "MiniMapWidget.h"
#include "Components/RetainerBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Battle/Character/TestPlayer/TestPlayerMC.h"
#include "Kismet/GameplayStatics.h"

void UMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    m_RB_Slot = Cast<UCanvasPanelSlot>(RetainerBox->Slot);
    if (m_RB_Slot)
    {
        m_MapImagePos = m_RB_Slot->GetPosition();
        m_MapImageSize = m_RB_Slot->GetSize();
    }
    // 
    m_MID = RetainerBox->GetEffectMaterial();
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMapPlayer();
}

void UMiniMapWidget::InitMap(float mapTexDiameterLength, FVector2D marginHarfLength)
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

void UMiniMapWidget::UpdateMapPlayer()
{
    AMainPlayer* player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    // プレイヤーが取得出来なかった　または　RetainerBoxにEffectMaterialがなかったら無視
    if (!player || !m_MID)
    {
        return;
    }

    FVector playerLocation = player->GetActorLocation();
    const float stageLeftMost  = m_StageMosts[0]->GetActorLocation().X;
    const float stageRightMost = m_StageMosts[1]->GetActorLocation().X;
    const float stageFrontMost = m_StageMosts[2]->GetActorLocation().Y;
    const float stageBackMost  = m_StageMosts[3]->GetActorLocation().Y;

    // 
    const float mapPlayerX = (playerLocation.X - stageLeftMost) / (stageRightMost - stageLeftMost);
    const float mapPlayerY = (playerLocation.Y - stageBackMost) / (stageFrontMost - stageBackMost);

    // WBPのマップ本来の幅 = WBP側の直径 - WBP側の余白の直径
    // WBP側の余白の直径 = Tex側の余白の直径 * (WBP側の直径 / Tex側の直径)
    // テクスチャ上の長さを WBP 上の長さに変換する倍率
    const float imageMarginWidth = m_MapImageSize.X / m_MapTexDiameterLength;
    const float imageMarginHeight = m_MapImageSize.Y / m_MapTexDiameterLength;

    // 余白を引いたあとの WBP 上のマップ本体の長さ
    const float mapWidth = m_MapImageSize.X - (m_MarginLength.X * imageMarginWidth);
    const float mapHeight = m_MapImageSize.Y - (m_MarginLength.Y * imageMarginHeight);

    // rbの座標調整
    const float rbOffsetX = (0.5f - mapPlayerX) * mapWidth;
    const float rbOffsetY = (0.5f - mapPlayerY) * mapHeight;

    // テクスチャ全体に対する マップ本体の長さの割合（UVの長さ）
    const float mapUVWidth  = (m_MapTexDiameterLength - m_MarginLength.X) / m_MapTexDiameterLength;
    const float mapUVHeight = (m_MapTexDiameterLength - m_MarginLength.Y) / m_MapTexDiameterLength;

    // 余白分の補正
    const float uvCenterOffsetX = (1.0f - mapUVWidth) * 0.5f;
    const float uvCenterOffsetY = (1.0f - mapUVHeight) * 0.5f;

    // UV図形の中心をUV座標で求めたもの
    const float uvCenterX = uvCenterOffsetX + mapPlayerX * mapUVWidth;
    const float uvCenterY = uvCenterOffsetY + mapPlayerY * mapUVHeight;

    if (m_RB_Slot)
    {
        // 
        m_RB_Slot->SetPosition(FVector2D(rbOffsetX + m_MapImagePos.X, rbOffsetY + m_MapImagePos.Y));
        //UE_LOG(LogTemp, Warning, TEXT("MapPlayer : X=%f, Y=%f"), rbOffsetX, rbOffsetY);
        
    }

    // マテリアルグラフのノード、VectorParameterにMapCenterのノードを作成
    m_MID->SetVectorParameterValue(TEXT("MapCenter"), FLinearColor(uvCenterX, uvCenterY, 0.0f, 0.0f));
    //UE_LOG(LogTemp, Warning, TEXT("MapCenter : X=%f, Y=%f"), uvCenterX, uvCenterY);
}

void UMiniMapWidget::AddStageMosts(FName tag)
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