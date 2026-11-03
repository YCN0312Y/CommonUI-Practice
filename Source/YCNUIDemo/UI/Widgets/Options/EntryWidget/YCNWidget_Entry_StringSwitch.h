// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_StringSwitch.generated.h"

class UYCNWidget_ButtonBase;
class UYCNWidget_RotatorBase;
class UYCNListData_String;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_StringSwitch : public UYCNWidget_EntryBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized()override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;

	//切换条目状态
	virtual void SwitchItemStatus(bool bIsSwitch)override;

	virtual void NativeOnEntryReleased() override;

	virtual bool IsValueModified() const override;

public:
	// 从当前设置重新读取并显示选择文本。
	virtual void RefreshEntryData()override;

private:
	/*
	* BindWidget--------------------
	*/
	//上一个
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Previous;

	//旋转器
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_RotatorBase>CommonRotator_SelectedText;

	//下一个
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Next;

	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_String>OwningValueData;

private:
	//点击上一个
	void OnClick_Previous();

	//点击下一个
	void OnClick_Next();

	void SwitchText(int32 InIndex);
};
