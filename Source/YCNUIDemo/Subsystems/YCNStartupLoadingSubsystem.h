// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPath.h"
#include "YCNStartupLoadingSubsystem.generated.h"

struct FStreamableHandle;

UCLASS()
class YCNUIDEMO_API UYCNStartupLoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintPure, Category = "YCNWidgetManagerSubsystem", meta = (WorldContext = "WorldContextObject"))
    static UYCNStartupLoadingSubsystem* Get(const UObject* WorldContextObject);
public:
    UPROPERTY(BlueprintReadOnly, Category = "YCN|Startup")
    bool bLoading = false;

    UPROPERTY(BlueprintReadOnly, Category = "YCN|Startup")
    bool bReady = false;

    UPROPERTY(BlueprintReadOnly, Category = "YCN|Startup")
    FString ErrorMessage;

private:
    // 保存句柄，使预加载的资源继续保持有效。
    TSharedPtr<FStreamableHandle> LoadHandle;

    TArray<FSoftObjectPath> RequestedPaths;
private:
    void OnAssetsLoaded();

    void Fail(const FString& Message);

public:
    // 首次加载，或者失败后重试。
    UFUNCTION(BlueprintCallable, Category = "YCN|Startup")
    void StartLoading();

    // 当前资源批次的进度，范围 0~1。
    UFUNCTION(BlueprintPure, Category = "YCN|Startup")
    float GetLoadProgress() const;

    // 加载成功后，创建并推送 AnyKey。
    // 返回 true 才可以关闭加载窗口。
    UFUNCTION(BlueprintCallable, Category = "YCN|Startup")
    bool ShowAnyKey();
};
