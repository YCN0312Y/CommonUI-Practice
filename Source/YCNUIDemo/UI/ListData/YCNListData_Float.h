// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNListData_Float.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_Float : public UYCNListData_Value
{
	GENERATED_BODY()
	
protected:
	//是否可以重置为默认值
	virtual bool CanResetToDefault()const { return false; }

public:
	//重置为默认值
	virtual bool ResetToDefault()const { return false; }

private:
	float OwningFloatDefaultValue;
};
