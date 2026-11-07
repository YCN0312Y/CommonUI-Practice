// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_Image.generated.h"

class UYCNListData_Image;
class UCommonLazyImage;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_Image : public UYCNWidget_EntryBase
{
	GENERATED_BODY()

protected:
	//初始化
	virtual void NativeOnInitialized()override;

	//有数据加入本条目
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;

	//切换条目状态
	virtual void SwitchItemStatus(bool bIsSwitch)override;

	//释放条目
	virtual void NativeOnEntryReleased() override;

	//鼠标点击
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;

public:
	//刷新条目数据
	virtual void RefreshEntryData()override;

private:
	//图标
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonLazyImage>CommonImage_Icon;

	//持有的值数据
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_Image>OwningValueData;

};
