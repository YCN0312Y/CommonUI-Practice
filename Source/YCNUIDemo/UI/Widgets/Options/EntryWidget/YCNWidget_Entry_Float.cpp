// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Float.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"
#include "CommonTextBlock.h"
#include "Components/Slider.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UYCNWidget_Entry_Float::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

    OwningValueData = Cast<UYCNListData_Float>(ListItemObject);

    if (IsValid(OwningValueData) && IsValid(Slider_VolumeSlider) && IsValid(CommonText_MinVolume) && IsValid(CommonText_MaxVolume))
    {
        CommonText_MinVolume->SetText(FText::AsNumber(OwningValueData->GetMinValue()));
        CommonText_MaxVolume->SetText(FText::AsNumber(OwningValueData->GetMaxValue()));
        Slider_VolumeSlider->SetMinValue(OwningValueData->GetMinValue());
        Slider_VolumeSlider->SetMaxValue(OwningValueData->GetMaxValue());
    }
    RefreshEntryData();
}

void UYCNWidget_Entry_Float::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    if (IsValid(Slider_VolumeSlider))
    {
		Slider_VolumeSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::NotifyUpdateCurrentVolume);
    }
}

void UYCNWidget_Entry_Float::NativePreConstruct()
{
	Super::NativePreConstruct();

    if (IsValid(Slider_VolumeSlider))
    {
        //离开作用域 bRefreshingEntry 自动恢复false
        TGuardValue<bool>RefreshGuard(bRefreshingEntry, true);
        NotifyUpdateCurrentVolume(Slider_VolumeSlider->GetValue());
    }
}

void UYCNWidget_Entry_Float::SwitchItemStatus(bool bIsSwitch)
{
	Super::SwitchItemStatus(bIsSwitch);

    if (!IsValid(Slider_VolumeSlider) || !IsValid(CommonText_CurrentVolume) || !IsValid(CommonText_MinVolume) || !IsValid(CommonText_MaxVolume))return;
    
    if (!IsValid(TextStyle_Default) || !IsValid(TextStyle_Selected))return;

	if (bIsSwitch)
	{
        Slider_VolumeSlider->SetSliderBarColor(FLinearColor(SliderSelectedColor));
        Slider_VolumeSlider->SetSliderHandleColor(FLinearColor(SliderSelectedColor));
		CommonText_CurrentVolume->SetStyle(TextStyle_Selected);
		CommonText_MinVolume->SetStyle(TextStyle_Selected);
		CommonText_MaxVolume->SetStyle(TextStyle_Selected);
	}
    else
    {
        Slider_VolumeSlider->SetSliderBarColor(FLinearColor(SliderDefaultColor));
        Slider_VolumeSlider->SetSliderHandleColor(FLinearColor(SliderDefaultColor));
        CommonText_CurrentVolume->SetStyle(TextStyle_Default);
        CommonText_MinVolume->SetStyle(TextStyle_Default);
        CommonText_MaxVolume->SetStyle(TextStyle_Default);
    }
}

void UYCNWidget_Entry_Float::RefreshEntryData()
{
    UpdatePointStatus();
    if (bRefreshingEntry || !IsValid(CommonText_CurrentVolume) || !IsValid(Slider_VolumeSlider) || !IsValid(OwningValueData)) return;
    
    float CurrentValue = 0.0f;
    auto& Binding = OwningValueData->GetSettingBinding();

    if (!Binding.TryRead(CurrentValue))
    {
        CurrentValue = Binding.GetOwningDefaultValue();
    }
    
    TGuardValue<bool> RefreshGuard(bRefreshingEntry, true);

    Slider_VolumeSlider->SetValue(CurrentValue);

    NotifyUpdateCurrentVolume(CurrentValue);
}

bool UYCNWidget_Entry_Float::IsValueModified() const
{
	return IsValid(OwningValueData) && OwningValueData->CanResetToDefault();
}

void UYCNWidget_Entry_Float::NotifyUpdateCurrentVolume(float InValue)
{
    if (!IsValid(OwningValueData) || !IsValid(Slider_VolumeSlider) || !IsValid(CommonText_CurrentVolume)) return;

    if (!bRefreshingEntry)
    {
        OwningValueData->GetSettingBinding().TryWrite(InValue);
        if (IsValid(SliderValueChangedSound))
        {
            UGameplayStatics::PlaySound2D(this, SliderValueChangedSound);
        }
    }
    CommonText_CurrentVolume->SetText(FText::AsNumber(FMath::RoundToInt(InValue)));

    UCanvasPanelSlot* TextSlot = Cast<UCanvasPanelSlot>(CommonText_CurrentVolume->Slot);

    if (TextSlot)
    {
        const float MinValue = Slider_VolumeSlider->GetMinValue();//最小值
        const float MaxValue = Slider_VolumeSlider->GetMaxValue();//最大值
        const float Range = MaxValue - MinValue;

        const float Alpha = Range > 0.0f ? FMath::Clamp((InValue - MinValue) / Range, 0.0f, 1.0f) : 0.0f;
        const float ThumbWidth = Slider_VolumeSlider->GetWidgetStyle().NormalThumbImage.ImageSize.X;
        const float ReservedWidth = ThumbWidth * (Slider_VolumeSlider->HasIndentHandle() ? 2.0f : 1.0f);

        TextSlot->SetAnchors(FAnchors(Alpha, 0.5f));
        TextSlot->SetAlignment(FVector2D(0.5f, 0.5f));
        TextSlot->SetAutoSize(true);
        TextSlot->SetPosition(FVector2D((0.5f - Alpha) * ReservedWidth, CurrentValueVerticalOffset));
    }
}
