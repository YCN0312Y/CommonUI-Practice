// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_None.generated.h"

class UYCNListData_Text;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_None : public UYCNWidget_EntryBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized()override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;

	//切换条目状态
	virtual void SwitchItemStatus(bool bIsSwitch)override;

	virtual void NativeOnEntryReleased() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;

public:
	// 从当前设置重新读取并显示选择文本。
	virtual void RefreshEntryData()override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_Text>OwningValueData;
	
};
