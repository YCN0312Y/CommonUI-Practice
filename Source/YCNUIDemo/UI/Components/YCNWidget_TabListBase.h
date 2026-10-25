// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "YCNWidget_TabListBase.generated.h"

class UVerticalBox;
class UCommonActionWidget;
class UYCNWidget_ButtonBase;
class UTexture2D;
class UYCNListData_Tab;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_TabListBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized()override;

	virtual void NativePreConstruct()override;

	virtual void NativeDestruct() override;
	
private:
	//上一个
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActionWidget>CommonAction_Previous;
	//标签容器
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox>VerticalBox_TabContainer;
	//下一个
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActionWidget>CommonAction_Next;

	UPROPERTY(EditAnywhere, Category = "YCN Tab List")
	TSubclassOf<UYCNWidget_ButtonBase>TabButton;

	//主标签图标
	UPROPERTY(EditAnywhere, Category = "YCN Button")
	TMap<FName, TSoftObjectPtr<UTexture2D>>MainTabIcon;

public:
	void ApplyRegisterTab(const UYCNListData_Tab* InData);
};
