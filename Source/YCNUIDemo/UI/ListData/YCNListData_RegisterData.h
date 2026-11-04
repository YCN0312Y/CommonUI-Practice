// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "YCNListData_RegisterData.generated.h"

class UYCNListData_Base;
class UYCNListData_Tab;
class UYCNGameUserSettings;
class UYCNListData_String;

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_RegisterData : public UObject
{
	GENERATED_BODY()

public:
	//初始化数据列表
	void InitDataList();

	UYCNListData_Tab* GetDataByDataID(const FName& InDataID);

private:
	UPROPERTY(Transient)
	TArray<UYCNListData_Tab*> AvailableDataList;

public:
	//用户设置
	TWeakObjectPtr<UYCNGameUserSettings>UserSettings;

public:
	DECLARE_DELEGATE(FOnSettingsValueChanged)
	FOnSettingsValueChanged OnSettingValueChanged;

private:
	//初始化控制设置
	void InitControllerList();

	void InitKeyboardMouseList();

	//初始化游戏设置
	void InitGameSettingsList();

	//初始化音频设置
	void InitAudioList();

	//初始化显示设置
	void InitDisplayList();

	//初始化画质
	void InitHDList();

	//初始化帮助设置
	void InitAssistList();

	//初始化帮助设置
	void InitQuitList();

	//设置数据字符串
	FText SetDataText(const FString& InDaraString);

	//获取最大分辨率值
	FText GetMaxResolutionText();

	//将可用的分辨率添加进选项列表
	void AppendResolutionList(UYCNListData_String* InData);

	//绑定值
	template<typename TData, typename TGetter, typename TSetter>
	void BindCachedValue(TData* InData, TGetter InGetter, TSetter InSetter)
	{
		if (!IsValid(InData) || !InGetter || !InSetter || !UserSettings.IsValid())return;

		using FBinding = typename TData::FSettingBinding;
		using TValue = typename FBinding::TValueType;
        using FReadDelegate = typename FBinding::FReadDelegate;
        using FWriteDelegate = typename FBinding::FWriteDelegate;

        InData->GetSettingBinding().Bind(
            FReadDelegate::CreateLambda(
                [this, InGetter](TValue& OutValue) -> bool
                {
                    UYCNGameUserSettings* Settings = UserSettings.Get();
                    if (!Settings) return false;
                    
                    OutValue = (Settings->*InGetter)();
                    return true;
                }),
            FWriteDelegate::CreateLambda(
                [this, InSetter](const TValue& InValue) -> bool
                {
                    UYCNGameUserSettings* Settings = UserSettings.Get();
                    if (!Settings) return false;

                    (Settings->*InSetter)(InValue);
                    Settings->SaveSettings();

                    if (UYCNListData_RegisterData* Register = this)
                    {
                        Register->OnSettingValueChanged.ExecuteIfBound();
                    }

                    return true;
                })
        );

	}

public:
	const TArray<UYCNListData_Tab*>& GetAvailableDataList() const { return AvailableDataList; }
};
