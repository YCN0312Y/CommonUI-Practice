
#include "YCNUIDemo/UObject/YCNSettingBinding.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"

bool UYCNSettingBinding::Initialize(UYCNGameUserSettings* InSettings, FName InPropertyName)
{
    Settings = nullptr;
    PropertyName = NAME_None;

    if (!IsValid(InSettings) || InPropertyName.IsNone())
    {
        return false;
    }

    const FProperty* Property =
        FindFProperty<FProperty>(
            InSettings->GetClass(),
            InPropertyName);

    // 这里只绑定单个 Config 属性。
    if (!Property ||
        !Property->HasAnyPropertyFlags(CPF_Config) ||
        Property->ArrayDim != 1)
    {
        return false;
    }

    Settings = InSettings;
    PropertyName = InPropertyName;

    return true;
}

UObject* UYCNSettingBinding::GetSettingsObject() const
{
    return IsValid(Settings.Get())
        ? Settings.Get()
        : nullptr;
}

bool UYCNSettingBinding::ResetToDefault()
{
    UObject* Target = GetSettingsObject();

    if (!Target)
    {
        return false;
    }

    FProperty* Property =
        FindFProperty<FProperty>(
            Target->GetClass(),
            PropertyName);

    if (!Property)
    {
        return false;
    }

    const UObject* Defaults =
        Target->GetClass()->GetDefaultObject();

    void* Destination =
        Property->ContainerPtrToValuePtr<void>(Target);

    const void* Source =
        Property->ContainerPtrToValuePtr<void>(Defaults);

    Property->CopyCompleteValue(Destination, Source);

    return true;
}