// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Msic/YCNDataType.h"
#include "YCNListData_Image.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_Image : public UYCNListData_Value
{
	GENERATED_BODY()

private:
	TFunction<void()>ClickCallBack;

	//预览键
	UPROPERTY(Transient)
	FKey PreviewKey;

public:
	//设置点击回调
	void SetClickCallBack(TFunction<void()>InClickCallBack);

	//获取点击回调
	TFunction<void()>GetClickCallBack() { return ClickCallBack; }

	//预览键是否有效
	bool HasPreviewKey() const;

	//设置预览键
	void SetPreviewKey(const FKey& InKey);

	//获取预览键
	const FKey& GetPreviewKey() const { return PreviewKey; }


};
