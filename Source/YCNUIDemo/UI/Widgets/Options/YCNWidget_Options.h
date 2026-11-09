// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "Components/SlateWrapperTypes.h"
#include "GameplayTagContainer.h"
#include "YCNWidget_Options.generated.h"

class UYCNWidget_TabListBase;
class UYCNListData_RegisterData;
class UYCNWidget_ButtonBase;
class UYCNWidget_ListViewBase;
class UBorder;
class UYCNWidget_Entry_StringOption;
class UYCNWidget_DetailsBase;
class UYCNListData_Tab;
class UYCNListData_Base;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Options : public UYCNWidget_ActivatableWidgetBase
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

	virtual bool NativeOnHandleBackAction() override;
private:
	/*
	* BindWidget Begin----------
	*/
	//主标签
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_TabListBase>TabList_Tab;

	//子标签
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ListViewBase>ListView_SubTab;

	//控制器
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ListViewBase>ListView_Control;

	//下拉框背景
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder>Border_DropdownBlocker;

	//详情页
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_DetailsBase>DetailsBase_Details;

	/*
	* BindWidget End----------
	*/

	//重置
	UPROPERTY(EditAnywhere, Category = "YCN Action")
	FDataTableRowHandle ResetAction;
	//重置操作句柄
	FUIActionBindingHandle ResetActionHandle;

	//持有的注册数据对象
	UPROPERTY()
	TObjectPtr<UYCNListData_RegisterData>OwningRegisterData;

	//选择打开下拉菜单的条目
	UPROPERTY(Transient)
	TWeakObjectPtr<UYCNWidget_Entry_StringOption> SelectedDropdownEntry;

	//唯一选择的条目
	UPROPERTY(Transient)
	TWeakObjectPtr<UYCNListData_Base>UniqueSelectedItem;

	//添加重置操作
	bool bResetActionAdded;

	//显示的标签ID
	FName DisplayedTabID = NAME_None;

	//恢复标签
	bool bRestoringTab = false;

	//已打开确认弹窗
	bool bLeaveConfirmationOpen = false;
private:
	//点击重置回调
	void OnClicked_Reset();
	
	UFUNCTION()
	void OnTabSelected(FName TabID);

	//初始化持有的注册数据
	UYCNListData_RegisterData* InitOwningRegisterData();

	//列表生成小条目时
	void OnListEntryGenerated(UUserWidget& EntryWidget);

	//设置下拉菜单背景可视
	void SetDropdownBlockerVisible(bool bIsOpen);

	//打开下拉菜单回调
	void OnEntryDropdownOpenChanged(bool bIsOpen, UYCNWidget_Entry_StringOption* SourceEntry);

	//小条目选择更新回调
	void OnSubTabSelectionChanged(UObject* InSelectedItem);

	//更新详情页
	void UpdateDetails(UYCNListData_Tab* InData);

	//添加重置动作
	void AddResetAction(const FName& InTabID);

	//用户设置值修改回调
	void OnSettingValueChanged();

	//获取数据标签所有子数据
	const TArray<UYCNListData_Base*> GetTabAllChildData(UYCNListData_Tab* InTab, const FName& InTabID);

	UFUNCTION()
	FEventReply OnDropdownBlockerMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	//显示标签内容
	void ShowTabContent(FName TabID);

	//确认离开前的质量
	void ConfirmQualityBeforeLeaving(FName TargetTabID);

	void AddDataToList(FName TabID, UYCNWidget_ListViewBase* InList);

	void SetStackTransitionDuration(const FGameplayTag& InStackTag, float InDuration);
};
