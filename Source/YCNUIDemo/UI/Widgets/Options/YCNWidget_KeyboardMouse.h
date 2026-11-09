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

	//当前值
	FKey CurrentKey;

	//重置
	UPROPERTY(EditAnywhere, Category = "YCN Action")
	FDataTableRowHandle ResetAction;
	//重置操作句柄
	FUIActionBindingHandle ResetActionHandle;

	bool bResetActionAdded = false;

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

	void SetItemWaitingForInput(UYCNListData_Image* InData, bool bWaiting);

	//停止捕获按键输入
	void StopKeyInputProcessor();

	void OnClick_Key(const FKey& InKey);

	void OnClicked_Reset();

	void AddResetAction(const FName& InTabID);
};

//按键输入处理器
class FYCNKeyInputProcessor : public IInputProcessor, public TSharedFromThis<FYCNKeyInputProcessor>
{
public:
	DECLARE_DELEGATE_OneParam(FOnClickDelegate, const FKey& InKey);
	FOnClickDelegate OnClickDelegate;
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	//按键按下
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& Event) override;

	//鼠标按下
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& Event) override;
};
