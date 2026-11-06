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

public:
	void SetClickCallBack(TFunction<void()>InClickCallBack);
	TFunction<void()>GetClickCallBack() { return ClickCallBack; }
};
