// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "YCNWidget_Options.generated.h"

class UYCNWidget_TabListBase;
class UYCNListData_RegisterData;
class UYCNWidget_ButtonBase;
class UYCNWidget_ListViewBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Options : public UYCNWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
	
public:
	//初始化
	virtual void NativeOnInitialized()override;
	//预构造
	virtual void NativePreConstruct()override;
	//激活
	virtual void NativeOnActivated()override;

private:
	//主标签
	UPROPERTY(BlueprintReadOnly, Category = "YCN Option Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_TabListBase>TabList_MainTab;
	//子标签
	UPROPERTY(BlueprintReadOnly, Category = "YCN Option Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_ListViewBase>ListView_SubTab;
	//重置
	UPROPERTY(EditAnywhere, Category = "YCN Action")
	FDataTableRowHandle ResetAction;
	//重置操作句柄
	FUIActionBindingHandle ResetActionHandle;

	//拥有的注册数据对象
	UPROPERTY()
	TObjectPtr<UYCNListData_RegisterData>OwningRegisterData;
private:
	void OnClickedBack();
	void OnClickedReset();
	
	UFUNCTION()
	void OnMainTabSelected(FName TabID);
	UFUNCTION()
	void OnTabSelected_MainTab(FName TabId);

	UYCNListData_RegisterData* InitOwningRegisterData();
};
