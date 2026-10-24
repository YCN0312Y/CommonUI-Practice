// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "YCNWidget_ListViewBase.generated.h"

class UYCNWidgetDataAsset;

UCLASS()
class YCNUIDEMO_API UYCNWidget_ListViewBase : public UCommonListView
{
	GENERATED_BODY()
	
public:
	//在需要为数据项生成显示行时自动调用
	virtual TSubclassOf<UUserWidget> GetDesiredEntryClassForItem(UObject* Item) const override;

private:
	UPROPERTY(EditAnywhere, Category = "YCN Option Widget")
	TObjectPtr<UYCNWidgetDataAsset> WidgetDataAsset;
};
