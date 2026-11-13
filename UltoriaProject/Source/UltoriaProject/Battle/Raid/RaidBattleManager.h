#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaidBattleTypes.h"
#include "RaidBattleManager.generated.h"

class ACharacterBase;
class AEnemyBase;
class APlayerController;
class ARaidBattleCommandProxy;
class URaidBattleTurnManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRaidBattleChanged);

UCLASS()
class ULTORIAPROJECT_API ARaidBattleManager : public AActor
{
    GENERATED_BODY()

public:
    ARaidBattleManager();

    UFUNCTION(BlueprintCallable, Category = "Raid")
    ACharacterBase* GetCurrentCharacter() const { return m_CurrentCharacter; }

    UFUNCTION(BlueprintCallable, Category = "Raid")
    AEnemyBase* GetRaidBoss() const { return m_Boss; }

    UFUNCTION(BlueprintCallable, Category = "Raid")
    bool IsRaidStarted() const { return m_bRaidStarted; }

    UFUNCTION(BlueprintCallable, Category = "Raid")
    bool IsWaitingForCommand() const { return m_bWaitingForCommand; }

    UFUNCTION(BlueprintCallable, Category = "Raid")
    bool IsRaidEnded() const { return m_bRaidEnded; }

    UFUNCTION(BlueprintCallable, Category = "Raid")
    ERaidBattleResult GetRaidResult() const { return m_RaidResult; }

    const TArray<FRaidCharacterState>& GetRaidCharacterStates() const { return m_RaidStates; }
    const TArray<TObjectPtr<ACharacterBase>>& GetTurnOrder() const { return m_TurnOrder; }

    UPROPERTY(BlueprintAssignable, Category = "Raid")
    FOnRaidBattleChanged OnRaidBattleChanged;

    // CommandProxyからサーバー上で呼び出す
    void HandlePlayerCommand(APlayerController* RequestingController, ERaidCommandType Command, ACharacterBase* Target);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_RaidState();

private:
    void TryInitializeRaid();
    bool InitializeRaid();
    void CreateCommandProxies();
    void ExecuteBossTurn();
    void AdvanceTurn();
    void CheckRaidEnd();
    void FinishRaid(ERaidBattleResult Result);
    void RefreshReplicatedStates();
    bool IsPlayerCharacterAlive(ACharacterBase* Character) const;

private:
    // レイドマップに配置したボスをDetailsから指定する
    UPROPERTY(EditInstanceOnly, Category = "Raid")
    TObjectPtr<AEnemyBase> m_Boss;

    UPROPERTY(Transient)
    TObjectPtr<URaidBattleTurnManager> m_TurnManager;

    UPROPERTY(ReplicatedUsing = OnRep_RaidState)
    TArray<FRaidCharacterState> m_RaidStates;

    UPROPERTY(Replicated)
    TArray<TObjectPtr<ACharacterBase>> m_TurnOrder;

    UPROPERTY(ReplicatedUsing = OnRep_RaidState)
    TObjectPtr<ACharacterBase> m_CurrentCharacter;

    UPROPERTY(ReplicatedUsing = OnRep_RaidState)
    bool m_bRaidStarted = false;

    UPROPERTY(ReplicatedUsing = OnRep_RaidState)
    bool m_bWaitingForCommand = false;

    UPROPERTY(ReplicatedUsing = OnRep_RaidState)
    bool m_bRaidEnded = false;

    UPROPERTY(ReplicatedUsing = OnRep_RaidState)
    ERaidBattleResult m_RaidResult = ERaidBattleResult::NONE;

    FTimerHandle m_InitializeTimer;
    FTimerHandle m_ActionTimer;
    int32 m_InitializeAttempts = 0;
};
