// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "Framework/Application/IInputProcessor.h"
#include "Input/Events.h"
#include "YCNWidget_KeyboardMouse.generated.h"

class UYCNWidget_ListViewBase;
class UYCNWidget_TabListBase;
class UYCNListData_RegisterKeyMouseData;
class FYCNKeyInputProcessor;
class UYCNListData_Image;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_KeyboardMouse : public UYCNWidget_ActivatableWidgetBase
{
	GENERATED_BODY()

protected:
	//初始化
	virtual void NativeOnInitialized()override;

	//预构造
	virtual void NativePreConstruct()override;

	//激活
	virtual void NativeOnActivated()override;

	//停用
	virtual void NativeOnDeactivated()override;

	//销毁
	virtual void NativeDestruct() override;

private:
	//标签列表
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_TabListBase>TabList_Key;

	//按键列表
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ListViewBase>ListView_Key;

	//持有的按键注册数据
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_RegisterKeyMouseData>OwningRegisterData;

	//待处理图标数据
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_Image> PendingIconData;

	//键输入处理器
	TSharedPtr<FYCNKeyInputProcessor> KeyInputProcessor;

private:
	//初始化注册数据
	UYCNListData_RegisterKeyMouseData* InitOwningRegisterData();

	//初始标签列表
	void InitTabList();

	//标签选择回调
	UFUNCTION()
	void OnTabSelected(FName InTabID);

	//小条目点击回调
	void OnKeyItemClicked(UObject* InItem);

	//停止键输入处理器
	void StopKeyInputProcessor();

	//输入处理器返回按键后，更新条目显示。
	void OnCapturedKey(FKey PressedKey);

	//按键已释放，或输入会话被取消。
	void OnCaptureFinished();
};

//按键输入处理器
class FYCNKeyInputProcessor : public IInputProcessor, public TSharedFromThis<FYCNKeyInputProcessor>
{
public:
	FYCNKeyInputProcessor(int32 InUserIndex, TSharedRef<SWidget> InCaptureWidget);

private:
	int32 UserIndex = 0;

	//捕获空间
	TWeakPtr<SWidget> CaptureWidget;

	//按下的键
	TSet<FKey> PressedKeys;

	FKey CapturedKey;

	//结果已完成
	bool bResultDelivered = false;

	//消请求
	bool bCancelRequested = false;

	//是否已完成
	bool bFinished = false;

public:
	//按键按下
	DECLARE_DELEGATE_OneParam(FOnKeyCaptured, FKey);
	FOnKeyCaptured OnKeyCaptured;

	//捕获完成
	DECLARE_DELEGATE(FOnCaptureFinished);
	FOnCaptureFinished OnCaptureFinished;

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	//按键按下
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& Event) override;

	//按键松开
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& Event) override;

	//鼠标按下
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& Event) override;

	//鼠标松开
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& Event) override;

private:
	bool IsInScope(FSlateApplication& SlateApp) const;

	bool IsPointerInside(const FPointerEvent& Event) const;

	void PressKey(const FKey& Key);

	void Finish();
};
