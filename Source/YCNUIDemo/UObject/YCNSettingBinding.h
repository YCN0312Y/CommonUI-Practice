#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/UnrealType.h"
#include "YCNSettingBinding.generated.h"

class UYCNGameUserSettings;

UCLASS()
class YCNUIDEMO_API UYCNSettingBinding : public UObject
{
    GENERATED_BODY()

public:
    // 将当前绑定关联到 Settings 中的一个 Config 属性。
    bool Initialize(
        UYCNGameUserSettings* InSettings,
        FName InPropertyName);

    // 读取当前值。
    // PropertyType 例如 FFloatProperty、FBoolProperty、FTextProperty。
    template<typename PropertyType, typename ValueType>
    bool TryGetValue(ValueType& OutValue) const
    {
        UObject* Target = GetSettingsObject();

        if (!Target)
        {
            return false;
        }

        const PropertyType* Property =
            FindFProperty<PropertyType>(
                Target->GetClass(),
                PropertyName);

        if (!Property)
        {
            return false;
        }

        OutValue =
            Property->GetPropertyValue_InContainer(Target);

        return true;
    }

    // 写入当前内存值，不自动保存，也不自动应用到游戏系统。
    template<typename PropertyType, typename ValueType>
    bool TrySetValue(const ValueType& InValue)
    {
        UObject* Target = GetSettingsObject();

        if (!Target)
        {
            return false;
        }

        PropertyType* Property =
            FindFProperty<PropertyType>(
                Target->GetClass(),
                PropertyName);

        if (!Property)
        {
            return false;
        }

        Property->SetPropertyValue_InContainer(Target, InValue);
        return true;
    }

    // 读取类默认对象中的值。
    template<typename PropertyType, typename ValueType>
    bool TryGetDefaultValue(ValueType& OutValue) const
    {
        UObject* Target = GetSettingsObject();

        if (!Target)
        {
            return false;
        }

        const PropertyType* Property =
            FindFProperty<PropertyType>(
                Target->GetClass(),
                PropertyName);

        if (!Property)
        {
            return false;
        }

        const UObject* Defaults =
            Target->GetClass()->GetDefaultObject();

        OutValue =
            Property->GetPropertyValue_InContainer(Defaults);

        return true;
    }

    // 将当前属性恢复为类默认对象中的值。
    bool ResetToDefault();

private:
    UObject* GetSettingsObject() const;

    UPROPERTY(Transient)
    TObjectPtr<UYCNGameUserSettings> Settings;

    UPROPERTY(Transient)
    FName PropertyName;
};