// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"
#include "YCNUIDemo/UI/ListData/YCNListData_String.h"
#include "YCNUIDemo/Msic/YCNDataType.h"
#include "YCNListData_RegisterData.generated.h"

class UYCNListData_Base;
class UYCNListData_Tab;
class UYCNListData_String;
class UYCNGameUserSettings;
class UYCNListData_Text;
class UYCNListData_Image;

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_RegisterData : public UObject
{
	GENERATED_BODY()

public:
	//初始化数据列表
	virtual void InitDataList();

	virtual UYCNListData_Tab* GetDataByDataID(const FName& InDataID);

private:
	UPROPERTY(Transient)
	TArray<UYCNListData_Tab*> AvailableDataList;

	TArray<FText>GraphicsLevelArray =
	{
		SetDataText(TEXT("低")),
		SetDataText(TEXT("中")),
		SetDataText(TEXT("高")),
		SetDataText(TEXT("超高")),
		SetDataText(TEXT("影视级")),
	};

public:
	//用户设置
	TWeakObjectPtr<UYCNGameUserSettings>UserSettings;

	//用户设置的值修改时回调
	DECLARE_DELEGATE(FOnSettingsValueChanged)
	FOnSettingsValueChanged OnSettingValueChanged;

private:
	//初始化控制设置
	void InitControllerList();

	//初始化键盘与鼠标设置
	void InitKeyboardMouseList();

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

	TArray<FText> GetResolutionOptions() const;

	//绑定值
	template<typename TData, typename TGetter, typename TSetter>
	void BindCachedValue(TData* InData, TGetter InGetter, TSetter InSetter, bool bSaveOnWrite = true)
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
                [this, InSetter, bSaveOnWrite](const TValue& InValue) -> bool
                {
                    UYCNGameUserSettings* Settings = UserSettings.Get();
                    if (!Settings) return false;

					(Settings->*InSetter)(InValue);
					if (bSaveOnWrite)
					{
						Settings->SaveSettings();
					}

                    if (UYCNListData_RegisterData* Register = this)
                    {
                        Register->OnSettingValueChanged.ExecuteIfBound();
                    }

                    return true;
                })
        );

	}
	//UYCNListData_Tab
	void CreateTabData(UYCNListData_Tab* InData, FName InDataID, const FText& InDisplayText,
		UYCNListData_Base* InParentData = nullptr);

	//UYCNListData_Text
	void CreateTextData(UYCNListData_Text* InData, FName InDataID, const FText& InDisplayText,
		const FText& InDescriptionText, EConfirmationWindowsType InWindowType,
		const FText& InMessage, TFunction<void(EConfirmationWindowsButtonType)>InFunction, UYCNListData_Base* InParentData);

	//UYCNListData_Image
	void CreateImageData(UYCNListData_Image* InData, FName InDataID, const FText& InDisplayText, const FText& InDescriptionText,
		UTexture2D* InDataIcon, TFunction<void()> InFunction, UYCNListData_Base* InParentData);

	//UYCNListData_String
	template<typename TGetter, typename TSetter>
	void CreateStringData(UYCNListData_String* InData, FName InDataID, const FText& InDisplayText,
		const FText& InDescriptionText, const FText& InDisableText, const TArray<FText>& InOptionList,
		const FText& InDefaultValue, TGetter InGetter, TSetter InSetter, UYCNListData_Base* InParentData,
		bool bIsShowModifyPoint = true, TFunction<bool()> ConditionFunc = {}, bool bInitiallyEnabled = true)
	{
		if (!IsValid(InData) || !IsValid(InParentData) || InDataID.IsNone()) return;

		for (const FText& Option : InOptionList)
		{
			InData->AddDataToOptionsList(Option);
		}

		RegisterValueData(InData, InDataID, InDisplayText, InDescriptionText, InDisableText,
			InDefaultValue, InGetter, InSetter, InParentData, bIsShowModifyPoint, MoveTemp(ConditionFunc), bInitiallyEnabled);
	}

	//UYCNListData_StringSwitch
	template<typename TGetter, typename TSetter>
	void CreateStringSwitchData(UYCNListData_StringSwitch* InData, FName InDataID, const FText& InDisplayText,
		const FText& InDescriptionText, const FText& InDisableText, const TArray<FText>& InOptionList,
		const FText& InDefaultValue, TGetter InGetter, TSetter InSetter, UYCNListData_Base* InParentData,
		bool bIsShowModifyPoint = true, TFunction<bool()> ConditionFunc = {}, bool bInitiallyEnabled = true)
	{
		CreateStringData(InData, InDataID, InDisplayText, InDescriptionText, InDisableText,
			InOptionList, InDefaultValue, InGetter, InSetter, InParentData, bIsShowModifyPoint, MoveTemp(ConditionFunc), bInitiallyEnabled);
	}

	//UYCNListData_StringBool
	template<typename TGetter, typename TSetter>
	void CreateStringBoolData(UYCNListData_StringBool* InData, FName InDataID, const FText& InDisplayText,
		const FText& InDescriptionText, const FText& InDisableText, bool bDefaultOn,
		TGetter InGetter, TSetter InSetter, UYCNListData_Base* InParentData,
		TFunction<bool()> ConditionFunc = {}, bool bInitiallyEnabled = true)
	{
		if (!IsValid(InData) || !IsValid(InParentData) || InDataID.IsNone()) return;

		InData->AddOffAndNo();
		RegisterValueData(InData, InDataID, InDisplayText, InDescriptionText, InDisableText,
			bDefaultOn ? InData->GetTrueString() : InData->GetFalseString(),
			InGetter, InSetter, InParentData, true, MoveTemp(ConditionFunc), bInitiallyEnabled);
	}

	//UYCNListData_Float
	template<typename TGetter, typename TSetter>
	void CreateFloatData(UYCNListData_Float* InData, FName InDataID, const FText& InDisplayText,
		const FText& InDescriptionText, const FText& InDisableText, float InMinValue, float InMaxValue,
		float InDefaultValue, TGetter InGetter, TSetter InSetter, UYCNListData_Base* InParentData,
		TFunction<bool()> ConditionFunc = {}, bool bInitiallyEnabled = true)
	{
		if (!IsValid(InData) || !IsValid(InParentData) || InDataID.IsNone()) return;

		InData->SetMinValue(InMinValue);
		InData->SetMaxValue(InMaxValue);
		RegisterValueData(InData, InDataID, InDisplayText, InDescriptionText, InDisableText,
			InDefaultValue, InGetter, InSetter, InParentData, true, MoveTemp(ConditionFunc), bInitiallyEnabled);
	}

	//注册值数据
	template<typename TData, typename TGetter, typename TSetter>
	void RegisterValueData(TData* InData, FName InDataID, const FText& InDisplayText,
		const FText& InDescriptionText, const FText& InDisableText,
		const typename TData::FSettingBinding::TValueType& InDefaultValue,
		TGetter InGetter, TSetter InSetter, UYCNListData_Base* InParentData, 
		bool bIsShowModifyPoint, TFunction<bool()> ConditionFunc, bool bInitiallyEnabled)
	{
		InData->SetDataID(InDataID);
		InData->SetDataDisplayText(InDisplayText);
		InData->SetDataDescription(InDescriptionText);
		InData->SetDataDisabledText(InDisableText);
		InData->SetbDataDisabled(bInitiallyEnabled);
		InData->GetSettingBinding().SetOwningDefaultValue(InDefaultValue);
		InData->SetbIsShowModifyPoint(bIsShowModifyPoint);
		if (ConditionFunc)
		{
			InData->AddCondition(MoveTemp(ConditionFunc));
		}

		const bool bSaveOnWrite = InParentData->GetDataID() != FName(TEXT("GraphicsDetail"));
		BindCachedValue(InData, InGetter, InSetter, bSaveOnWrite);
		InParentData->AddChildData(InData);
	}

public:
	const TArray<UYCNListData_Tab*>& GetAvailableDataList() const { return AvailableDataList; }
};
