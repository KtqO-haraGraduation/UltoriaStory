#include "RaidBattleCommandProxy.h"
#include "RaidBattleManager.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "../../System/Effect/EffectManager.h"
#include "../../System/Sound/SoundManager.h"

ARaidBattleCommandProxy::ARaidBattleCommandProxy()
{
    bReplicates = true;
    SetReplicateMovement(false);
}

void ARaidBattleCommandProxy::Initialize(ARaidBattleManager* Manager, APlayerController* OwningController)
{
    if (!HasAuthority()) return;

    m_RaidManager = Manager;
    SetOwner(OwningController);
    ForceNetUpdate();
}

void ARaidBattleCommandProxy::SubmitCommand(ERaidCommandType Command, ACharacterBase* Target)
{
    ServerSubmitCommand(Command, Target);
}

void ARaidBattleCommandProxy::ClientShowDamage_Implementation(ACharacterBase* Target, int32 Damage)
{
    if (m_RaidManager) m_RaidManager->DisplayDamage(Target, Damage);
}

void ARaidBattleCommandProxy::ClientPlayHitFX_Implementation(FVector Location)
{
    UGameInstance* GI = GetGameInstance();
    if (!GI) return;

    if (USoundManager* Sound = GI->GetSubsystem<USoundManager>())  Sound->PlaySE("SE_Hit");
    if (UEffectManager* Fx = GI->GetSubsystem<UEffectManager>()) Fx->PlayEffect("NS_Hit", Location);
}

void ARaidBattleCommandProxy::ServerSubmitCommand_Implementation(ERaidCommandType Command, ACharacterBase* Target)
{
    if (!m_RaidManager) return;

    m_RaidManager->HandlePlayerCommand(Cast<APlayerController>(GetOwner()), Command, Target);
}

void ARaidBattleCommandProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARaidBattleCommandProxy, m_RaidManager);
}
