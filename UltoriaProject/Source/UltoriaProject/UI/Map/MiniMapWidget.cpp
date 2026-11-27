// Fill out your copyright notice in the Description page of Project Settings.
#include "MiniMapWidget.h"
#include "Components/RetainerBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Character/Enemy/EnemyManager.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../Battle/Character/TestPlayer/TestPlayerMC.h"
#include "Kismet/GameplayStatics.h"

void UMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    m_RB_Slot = Cast<UCanvasPanelSlot>(RetainerBox->Slot);
    if (m_RB_Slot)
    {
        m_MapImageCenterPos = m_RB_Slot->GetPosition();
        m_MapImageSize = m_RB_Slot->GetSize();
    }
    // 
    m_MID = RetainerBox->GetEffectMaterial();

    if (m_MID)
    {
        m_MID->GetScalarParameterValue(TEXT("RangeArea"), m_DrawRangeArea);
    }
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMapPlayer();
    UpdateMapEnemy();
    UpdateObjectiveMarker(InDeltaTime);
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

void UMiniMapWidget::InitEnemySlot(TArray<AEnemyBase*> enemies)
{
    for (AEnemyBase* enemy : enemies)
    {
        if (!IsValid(enemy))
        {
            continue;
        }

        // 新しくUImageを生成
        UImage* newEnemyImage = NewObject<UImage>(this);
        // Brush設定をまとめてコピー
        newEnemyImage->SetBrush(EnemyImage->GetBrush());

        UCanvasPanelSlot* originalSlot = Cast<UCanvasPanelSlot>(EnemyImage->Slot);
        UCanvasPanelSlot* enemySlot = MapCanvas->AddChildToCanvas(newEnemyImage);

        enemySlot->SetAnchors(originalSlot->GetAnchors());
        enemySlot->SetAlignment(originalSlot->GetAlignment());
        enemySlot->SetSize(originalSlot->GetSize());
        enemySlot->SetAutoSize(originalSlot->GetAutoSize());
        enemySlot->SetOffsets(originalSlot->GetOffsets());

        m_EnemySlots.Add(enemy, enemySlot);
    }
}

void UMiniMapWidget::UpdateMapPlayer()
{
    // プレイヤーを取得
    AMainPlayer* player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // プレイヤーとマップのIDを取得出来ていなければ終了
    if (!player || !m_MID)
    {
        return;
    }

    // プレイヤーの回転を取得
    const float playerYaw = player->GetActorRotation().Yaw;

    // プレイヤーのアイコンがあれば
    if (PlayerImage)
    {
        // 向きを調整
        PlayerImage->SetRenderTransformAngle(playerYaw + m_PlayerIconYawOffset);
    }

    if (m_StageMosts.Num() < 4) return;

    // プレイヤーの位置とステージ端の位置を取得
    FVector playerLocation = player->GetActorLocation();
    const float stageLeftMost = m_StageMosts[0]->GetActorLocation().X;
    const float stageRightMost = m_StageMosts[1]->GetActorLocation().X;
    const float stageFrontMost = m_StageMosts[2]->GetActorLocation().Y;
    const float stageBackMost = m_StageMosts[3]->GetActorLocation().Y;

    // プレイヤーのUV座標
    const float uvMapPlayerX = (playerLocation.X - stageLeftMost) / (stageRightMost - stageLeftMost);
    const float uvMapPlayerY = (playerLocation.Y - stageBackMost) / (stageFrontMost - stageBackMost);

    // WBP側の余白の直径 = Tex側の余白の直径 * (WBP側の直径 / Tex側の直径)
    // テクスチャ上の長さを WBP 上の長さに変換する倍率
    const float imageMarginWidth = m_MapImageSize.X / m_MapTexDiameterLength;
    const float imageMarginHeight = m_MapImageSize.Y / m_MapTexDiameterLength;

    // WBPのマップ本来の幅 = WBP側の直径 - WBP側の余白の直径
    // 余白を引いたあとの WBP 上のマップ本体の長さ
    const float mapWidth = m_MapImageSize.X - (m_MarginLength.X * imageMarginWidth);
    const float mapHeight = m_MapImageSize.Y - (m_MarginLength.Y * imageMarginHeight);

    // rbの座標調整
    const float rbOffsetX = (0.5f - uvMapPlayerX) * mapWidth;
    const float rbOffsetY = (0.5f - uvMapPlayerY) * mapHeight;

    // テクスチャ全体に対する マップ本体の長さの割合（UVの長さ）
    const float mapUVWidth = (m_MapTexDiameterLength - m_MarginLength.X) / m_MapTexDiameterLength;
    const float mapUVHeight = (m_MapTexDiameterLength - m_MarginLength.Y) / m_MapTexDiameterLength;

    // 余白分の補正
    const float uvCenterOffsetX = (1.0f - mapUVWidth) * 0.5f;
    const float uvCenterOffsetY = (1.0f - mapUVHeight) * 0.5f;

    // UV図形の中心をUV座標で求めたもの
    m_UVMapCenter.X = uvCenterOffsetX + uvMapPlayerX * mapUVWidth;
    m_UVMapCenter.Y = uvCenterOffsetY + uvMapPlayerY * mapUVHeight;

    if (m_RB_Slot)
    {
        m_RB_Slot->SetPosition(FVector2D(rbOffsetX + m_MapImageCenterPos.X, rbOffsetY + m_MapImageCenterPos.Y));
        //UE_LOG(LogTemp, Warning, TEXT("MapPlayer : X=%f, Y=%f"), rbOffsetX, rbOffsetY);
    }

    // マテリアルグラフのノード、VectorParameterにMapCenterのノードを作成
    m_MID->SetVectorParameterValue(TEXT("MapCenter"), FLinearColor(m_UVMapCenter.X, m_UVMapCenter.Y, 0.0f, 0.0f));
    //UE_LOG(LogTemp, Warning, TEXT("MapCenter : X=%f, Y=%f"), m_UVMapCenter.X, m_UVMapCenter.Y);
}

void UMiniMapWidget::UpdateMapEnemy()
{
    TArray<AEnemyBase*> enemies = GetWorld()->GetSubsystem<UEnemyManager>()->GetEnemies();

    if (!m_InitEnemySlot)
    {
        InitEnemySlot(enemies);
        m_InitEnemySlot = true;
    }

    const float stageLeftMost = m_StageMosts[0]->GetActorLocation().X;
    const float stageRightMost = m_StageMosts[1]->GetActorLocation().X;
    const float stageFrontMost = m_StageMosts[2]->GetActorLocation().Y;
    const float stageBackMost = m_StageMosts[3]->GetActorLocation().Y;

    // WBP側の余白の直径 = Tex側の余白の直径 * (WBP側の直径 / Tex側の直径)
    // テクスチャ上の長さを WBP 上の長さに変換する倍率
    const float imageMarginWidth = m_MapImageSize.X / m_MapTexDiameterLength;
    const float imageMarginHeight = m_MapImageSize.Y / m_MapTexDiameterLength;

    // WBPのマップ本来の幅 = WBP側の直径 - WBP側の余白の直径
    // 余白を引いたあとの WBP 上のマップ本体の長さ
    const float mapWidth  = m_MapImageSize.X - (m_MarginLength.X * imageMarginWidth);
    const float mapHeight = m_MapImageSize.Y - (m_MarginLength.Y * imageMarginHeight);

    // テクスチャ全体に対する マップ本体の長さの割合（UVの長さ）
    const float mapUVWidth = (m_MapTexDiameterLength - m_MarginLength.X) / m_MapTexDiameterLength;
    const float mapUVHeight = (m_MapTexDiameterLength - m_MarginLength.Y) / m_MapTexDiameterLength;

    // 余白分の補正
    const float uvCenterOffsetX = (1.0f - mapUVWidth) * 0.5f;
    const float uvCenterOffsetY = (1.0f - mapUVHeight) * 0.5f;

    for (AEnemyBase* enemy : enemies)
    {
        if (!IsValid(enemy))
        {
            continue;
        }

        UCanvasPanelSlot* enemySlot = m_EnemySlots.FindRef(enemy);
        UImage* enemyImage = Cast<UImage>(enemySlot->Content);

        // 敵の座標を取得
        FVector enemyLocation = enemy->GetActorLocation();

        // 敵の進行度とステージ全体の幅を計算する
        // マップ上の敵座標 = 敵が進んだ距離 / ステージの直径 * マップの調整
        const float mapEnemyX = ((enemyLocation.X - stageLeftMost) / (stageRightMost - stageLeftMost) - 0.5f) * mapWidth;
        const float mapEnemyY = ((enemyLocation.Y - stageBackMost) / (stageFrontMost - stageBackMost) - 0.5f) * mapHeight;

        // 敵のUV座標
        const float uvMapEnemyX = uvCenterOffsetX + (enemyLocation.X - stageLeftMost) / (stageRightMost - stageLeftMost) * mapUVWidth;
        const float uvMapEnemyY = uvCenterOffsetY + (enemyLocation.Y - stageBackMost) / (stageFrontMost - stageBackMost) * mapUVHeight;

        // MapCenterはマテリアルに渡している値を使用
        const FVector2D uvEnemy(uvMapEnemyX, uvMapEnemyY);
        // マテリアルと同じ距離判定
        const float distance = FVector2D::Distance(uvEnemy, m_UVMapCenter);
        /*UE_LOG(LogTemp, Warning, TEXT("UVMapCenter : X=%f, Y=%f"), m_UVMapCenter.X, m_UVMapCenter.Y);
        UE_LOG(LogTemp, Warning, TEXT("UVMapEnemy  : X=%f, Y=%f"), uvMapEnemyX, uvMapEnemyY);
        UE_LOG(LogTemp, Warning, TEXT("MapEnemy    : dis=%f"), distance);*/

        // 
        const bool bInsideMap = distance < m_DrawRangeArea;

        if (enemySlot)
        {
            // 敵の座標 + RetainerBoxの現在の座標
            enemySlot->SetPosition(FVector2D(mapEnemyX + m_RB_Slot->GetPosition().X, mapEnemyY + m_RB_Slot->GetPosition().Y));
            //UE_LOG(LogTemp, Warning, TEXT("MapEnemy : X=%f, Y=%f"), mapEnemyX + m_RB_Slot->GetPosition().X, mapEnemyY + m_RB_Slot->GetPosition().Y);

            enemyImage->SetVisibility(bInsideMap ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}

void UMiniMapWidget::UpdateObjectiveMarker(float deltaTime)
{

}

bool UMiniMapWidget::WorldToMapPos(const FVector& worldLoc, FVector2D& outPos, bool& outInside) const
{
    // m_StageMostsがない、有効化されていなければ終了
    if (m_StageMosts.Num() < 4 || !m_RB_Slot)return false;
    for (AActor* stageMost : m_StageMosts)
    {
        if (!IsValid(stageMost)) return false;
    }

    // マップの端を取得
    const float left = m_StageMosts[0]->GetActorLocation().X;
    const float right = m_StageMosts[1]->GetActorLocation().X;
    const float front = m_StageMosts[2]->GetActorLocation().Y;
    const float back = m_StageMosts[3]->GetActorLocation().Y;

    // 上下左右の範囲を計算
    const float rangeX = right - left;
    const float rangeY = front - back;

	return false;
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