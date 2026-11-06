// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNWidget_KeyboardMouse.generated.h"

class UYCNWidget_ListViewBase;
class UYCNWidget_TabListBase;
class UYCNListData_RegisterKeyMouseData;

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
	virtual void NativeOnDeactivated() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_TabListBase>TabList_Key;

	//按键列表
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ListViewBase>ListView_Key;


	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_RegisterKeyMouseData>OwningRegisterData;

private:
	//初始化注册数据
	UYCNListData_RegisterKeyMouseData* InitOwningRegisterData();

	void OnTabSelected(FName InTabID);
};
