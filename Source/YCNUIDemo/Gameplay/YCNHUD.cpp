// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Gameplay/YCNHUD.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_MainStack.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"

void AYCNHUD::BeginPlay()
{
    Super::BeginPlay();

    //只为本地玩家创建 UI
    APlayerController* PC = GetOwningPlayerController();

    if (!IsValid(PC) || !PC->IsLocalController()) return;

    //获取主堆栈的软类引用
    const TSoftClassPtr<UYCNWidget_MainStack> MainWidget = UYCNFunctionLibrary::GetMainStackWidget();

    if (MainWidget.IsNull()) return;

    //使用弱指针保存对象引用
    const TWeakObjectPtr<AYCNHUD> WeakHUD(this);
    const TWeakObjectPtr<APlayerController> WeakPC(PC);
    const TWeakObjectPtr<UWorld> WeakWorld(GetWorld());

    //异步加载主堆栈
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        MainWidget.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [WeakHUD, WeakPC, WeakWorld, MainWidget]()
            {
                AYCNHUD* HUD = WeakHUD.Get();
                APlayerController* OwningPC = WeakPC.Get();
                UWorld* ExpectedWorld = WeakWorld.Get();

                if (IsValid(HUD) && IsValid(OwningPC) && IsValid(ExpectedWorld))
                {
                    if (HUD->GetWorld() == ExpectedWorld && OwningPC->GetWorld() == ExpectedWorld && OwningPC->IsLocalController())
                    {
                        UClass* MainWidgetClass = MainWidget.Get();

                        if (IsValid(MainWidgetClass))
                        {
                            UYCNWidgetManagerSubsystem* WidgetManager = UYCNWidgetManagerSubsystem::Get(HUD);
                            if (IsValid(WidgetManager))
                            {

                                //创建主堆栈 Widget
                                UYCNWidget_MainStack* CreatedMainStack = CreateWidget<UYCNWidget_MainStack>(OwningPC, MainWidgetClass);
                                if (IsValid(CreatedMainStack))
                                {
                                    //注册主堆栈
                                    WidgetManager->RegisterMainStack(CreatedMainStack);
                                    WidgetManager->SetOwningMainStackBackground(HUD->bIsVisibleBackground);
                                    //添加到本地玩家的屏幕
                                    CreatedMainStack->AddToPlayerScreen();
                                    HUD->OnMainStackReady();
                                }
                            }

                        }
                    }
                }

            }
        )
    );
}