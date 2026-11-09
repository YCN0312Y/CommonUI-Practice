// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterKeyMouseData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"

void UYCNListData_RegisterKeyMouseData::InitDataList()
{
	InitKeyboardList();
	InitMouseList();
}

UYCNListData_Tab* UYCNListData_RegisterKeyMouseData::GetDataByDataID(const FName& InDataID)
{
	for (const TObjectPtr<UYCNListData_Tab>& Data : KeyboardMouseDataList)
	{
		if (Data && Data->GetDataID() == InDataID)
		{
			return Data.Get();
		}
	}
	return nullptr;
}

void UYCNListData_RegisterKeyMouseData::InitKeyboardList()
{
	UYCNListData_Tab* KeyboardTab = NewObject<UYCNListData_Tab>();
	CreateIconTabData(KeyboardTab, FName("KeyboardTab"));
	{
		//向前移动
		UYCNListData_Image* MoveForward = NewObject<UYCNListData_Image>();
		CreateKeyImageData(MoveForward, FName("MoveForward"), SetDataText(TEXT("向前移动")), 
			FText::GetEmpty(), EKeys::W, KeyboardTab);

		//向后移动
		UYCNListData_Image* MoveBack = NewObject<UYCNListData_Image>();
		CreateKeyImageData(MoveBack, FName("MoveBack"), SetDataText(TEXT("向后移动")),
			FText::GetEmpty(), EKeys::S, KeyboardTab);

		//向左移动
		UYCNListData_Image* MoveLeft = NewObject<UYCNListData_Image>();
		CreateKeyImageData(MoveLeft, FName("MoveLeft"), SetDataText(TEXT("向左移动")),
			FText::GetEmpty(), EKeys::A, KeyboardTab);

		//向右移动
		UYCNListData_Image* MoveRight = NewObject<UYCNListData_Image>();
		CreateKeyImageData(MoveRight, FName("MoveRight"), SetDataText(TEXT("向右移动")),
			FText::GetEmpty(), EKeys::D, KeyboardTab);

		//疾奔
		UYCNListData_Image* Sprint = NewObject<UYCNListData_Image>();
		CreateKeyImageData(Sprint, FName("Sprint"), SetDataText(TEXT("疾奔")),
			FText::GetEmpty(), EKeys::LeftShift, KeyboardTab);

		//翻滚
		UYCNListData_Image* DodgeRoll = NewObject<UYCNListData_Image>();
		CreateKeyImageData(DodgeRoll, FName("DodgeRoll"), SetDataText(TEXT("翻滚")),
			FText::GetEmpty(), EKeys::SpaceBar, KeyboardTab);

		//跳跃
		UYCNListData_Image* Jump = NewObject<UYCNListData_Image>();
		CreateKeyImageData(Jump, FName("Jump"), SetDataText(TEXT("跳跃")),
			FText::GetEmpty(), EKeys::LeftControl, KeyboardTab);

		//行走
		UYCNListData_Image* Walk = NewObject<UYCNListData_Image>();
		CreateKeyImageData(Walk, FName("Walk"), SetDataText(TEXT("行走")),
			FText::GetEmpty(), EKeys::LeftAlt, KeyboardTab);

		//拾取
		UYCNListData_Image* PickUp = NewObject<UYCNListData_Image>();
		CreateKeyImageData(PickUp, FName("PickUp"), SetDataText(TEXT("拾取")),
			FText::GetEmpty(), EKeys::E, KeyboardTab);
	}
}

void UYCNListData_RegisterKeyMouseData::InitMouseList()
{
	UYCNListData_Tab* MouseTab = NewObject<UYCNListData_Tab>();
	CreateIconTabData(MouseTab, FName("MouseTab"));
	{
		//攻击
		UYCNListData_Image* Attack = NewObject<UYCNListData_Image>();
		CreateKeyImageData(Attack, FName("Attack"), SetDataText(TEXT("攻击")),
			FText::GetEmpty(), EKeys::LeftMouseButton, MouseTab);

		//锁定
		UYCNListData_Image* LockTarget = NewObject<UYCNListData_Image>();
		CreateKeyImageData(LockTarget, FName("LockTarget"), SetDataText(TEXT("锁定")),
			FText::GetEmpty(), EKeys::MiddleMouseButton, MouseTab);
	}
}

void UYCNListData_RegisterKeyMouseData::CreateIconTabData(UYCNListData_Tab* InData, const FName& InDataID, UYCNListData_Base* InParentData)
{
	if (!IsValid(InData) || InDataID.IsNone() || (InParentData && !IsValid(InParentData))) return;

	InData->SetDataID(InDataID);

	if (InParentData)
	{
		InParentData->AddChildData(InData);
	}
	else
	{
		KeyboardMouseDataList.Add(InData);
	}
}

void UYCNListData_RegisterKeyMouseData::CreateKeyImageData(UYCNListData_Image* InData, FName InDataID, const FText& InDisplayText, 
	const FText& InDescriptionText, const FKey& InKey, UYCNListData_Base* InParentData)
{
	if (!IsValid(InData) || !IsValid(InParentData) || InDataID.IsNone()) return;
	
	//加入父标签
	CreateImageData(InData, InDataID, InDisplayText, InDescriptionText, InParentData, nullptr, {});

	//初始化当前预览按键和设置默认按键
	InData->SetPreviewKey(InKey);
	InData->GetSettingBinding().SetOwningDefaultValue(InKey);

	using FBinding = UYCNListData_Image::FSettingBinding;

	InData->GetSettingBinding().Bind(
		FBinding::FReadDelegate::CreateLambda(
			[InData](FKey& OutValue) -> bool
			{
				OutValue = InData->GetPreviewKey();
				return true;
			}),

		FBinding::FWriteDelegate::CreateLambda(
			[InData](const FKey& InValue) -> bool
			{
				InData->SetPreviewKey(InValue);
				return true;
			}));
}
