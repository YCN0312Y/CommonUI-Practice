// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNListData_Tab.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_Tab : public UYCNListData_Base
{
	GENERATED_BODY()
	
public:
	virtual void InitializationData()override;
	//添加子类
	void AddChildData(UYCNListData_Base* InChildData);
	//根据子类数据ID获取数据
	UYCNListData_Base* GetDataByChildDataID(const FName& InDataID);
	//获取子类数据列表
	TArray<UYCNListData_Base*>GetChildDataList()const { return ChildDataList; }

private:
	UPROPERTY(Transient)
	TArray<UYCNListData_Base*>ChildDataList;
};
