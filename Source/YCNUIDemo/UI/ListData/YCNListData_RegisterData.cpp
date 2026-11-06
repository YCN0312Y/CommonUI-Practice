// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_String.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Text.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "GenericPlatform/GenericWindow.h"
#include "Kismet/KismetSystemLibrary.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/Tags/YCNGameplayTags.h"

void UYCNListData_RegisterData::InitDataList()
{
	AvailableDataList.Reset();

	if (!UserSettings.IsValid())
	{
		UserSettings = UYCNGameUserSettings::Get();
	}

	InitControllerList();
	InitKeyboardMouseList();
	InitDisplayList();
	InitHDList();
	InitAudioList();
	InitAssistList();
	InitQuitList();
}

UYCNListData_Tab* UYCNListData_RegisterData::GetDataByDataID(const FName& InDataID)
{
	for (const TObjectPtr<UYCNListData_Tab>& Data : AvailableDataList)
	{
		if (Data && Data->GetDataID() == InDataID)
		{
			return Data.Get();
		}
	}
	return nullptr;
}

void UYCNListData_RegisterData::InitControllerList()
{
	UYCNListData_Tab* ControllerTab = NewObject<UYCNListData_Tab>();
	CreateTabData(ControllerTab, FName("ControllerTab"), SetDataText(TEXT("控制器")));
	{
		
	}
}

void UYCNListData_RegisterData::InitKeyboardMouseList()
{
	UYCNListData_Tab* KeyboardMouseTab = NewObject<UYCNListData_Tab>();
	CreateTabData(KeyboardMouseTab, TEXT("KeyboardMouseTab"), SetDataText(TEXT("键盘与鼠标")));
	{
		UYCNListData_Image* UserKeys = NewObject<UYCNListData_Image>();
		CreateImageData(
			UserKeys, TEXT("UserKeys"), SetDataText(TEXT("自定义键位")), FText::GetEmpty(),
			UYCNFunctionLibrary::GetAssetImageById(FName("UserKeys")),
			[this]()
			{
				if (UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this))
				{
					Manager->PushSoftWidgetToStack(
						YCNGameplayTags::YCN_WidgetStack_Menu,
						UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_KeyboardMouse));
				}
			},
			KeyboardMouseTab);
	}
}

void UYCNListData_RegisterData::InitAudioList()
{
	UYCNListData_Tab* AudioTab = NewObject<UYCNListData_Tab>();
	CreateTabData(AudioTab, TEXT("AudioTab"), SetDataText(TEXT("音频")));
	{
		//音量
		{
			UYCNListData_Tab* Volume = NewObject<UYCNListData_Tab>();
			{
				CreateTabData(Volume, TEXT("Volume"), SetDataText(TEXT("音量")), AudioTab);

				//全局音量
				UYCNListData_Float* GlobalVolume = NewObject<UYCNListData_Float>();
				CreateFloatData(
					GlobalVolume,
					TEXT("GlobalVolume"),
					SetDataText(TEXT("全局音量")),
					SetDataText(TEXT("调整所有声音的总开关。")),
					FText::GetEmpty(),
					0.f, 100.f, 100.f,
					&UYCNGameUserSettings::GetCachedGlobalVolume, &UYCNGameUserSettings::SetCachedGlobalVolume, Volume);

				//音乐音量
				UYCNListData_Float* MusicVolume = NewObject<UYCNListData_Float>();
				CreateFloatData(
					MusicVolume,
					TEXT("MusicVolume"),
					SetDataText(TEXT("音乐音量")),
					SetDataText(TEXT("调整音乐音量。")),
					FText::GetEmpty(),
					0.f, 100.f, 100.f,
					&UYCNGameUserSettings::GetCachedMusicVolume, &UYCNGameUserSettings::SetCachedMusicVolume, Volume);

				//音效音量
				UYCNListData_Float* SFXVolume = NewObject<UYCNListData_Float>();
				CreateFloatData(
					SFXVolume,
					TEXT("SFXVolume"),
					SetDataText(TEXT("音效音量")),
					SetDataText(TEXT("调整动作、UI点击等特效的音量。")),
					FText::GetEmpty(),
					0.f, 100.f, 100.f,
					&UYCNGameUserSettings::GetCachedSFXVolume, &UYCNGameUserSettings::SetCachedSFXVolume, Volume);

				//对话音量
				UYCNListData_Float* VoiceVolume = NewObject<UYCNListData_Float>();
				CreateFloatData(
					VoiceVolume,
					TEXT("VoiceVolume"),
					SetDataText(TEXT("对话音量")),
					SetDataText(TEXT("调整角色配音与旁白的音量。")),
					FText::GetEmpty(),
					0.f, 100.f, 100.f,
					&UYCNGameUserSettings::GetCachedVoiceVolume, &UYCNGameUserSettings::SetCachedVoiceVolume, Volume);
			}
		}
	}
}

void UYCNListData_RegisterData::InitDisplayList()
{
	UYCNListData_Tab* DisplayTab = NewObject<UYCNListData_Tab>();
	{
		CreateTabData(DisplayTab, TEXT("DisplayTab"), SetDataText(TEXT("显示")));
		UYCNListData_Tab* ScreenTab = NewObject<UYCNListData_Tab>();
		{
			CreateTabData(ScreenTab, TEXT("ScreenTab"), SetDataText(TEXT("显示")), DisplayTab);

			//显示模式
			UYCNListData_String* DisplayMode = NewObject<UYCNListData_String>();
			TArray<FText>DisplayModeArray =
			{
				SetDataText(TEXT("无边框模式")),
				SetDataText(TEXT("窗口模式"))
			};
			CreateStringData(
				DisplayMode,
				TEXT("DisplayMode"),
				SetDataText(TEXT("显示模式")),
				SetDataText(TEXT("调整游戏的显示模式。\n\n无边框模式：以无边框窗口填满屏幕。\n\n窗口模式：以普通窗口显示游戏。")),
				FText::GetEmpty(),
				DisplayModeArray,
				SetDataText(TEXT("无边框模式")),
				&UYCNGameUserSettings::GetCahcedDisplayMode, &UYCNGameUserSettings::SetCachedDisplayMode, ScreenTab);

			//屏幕宽高比
			UYCNListData_String* ScreenRatio = NewObject<UYCNListData_String>();
			TArray<FText>ScreenRatioArray
			{
				SetDataText(TEXT("自动")),
				SetDataText(TEXT("16:9")),
				SetDataText(TEXT("21:9"))
			};
			CreateStringData(
				ScreenRatio,
				TEXT("ScreenRatio"),
				SetDataText(TEXT("屏幕宽高比")),
				SetDataText(TEXT("调整游戏画面的宽高比。\n\n自动：按显示器桌面分辨率显示。\n\n16:9：固定为 1920x1080。\n\n21:9：使用显示器支持的最高分辨率。")),
				FText::GetEmpty(),
				ScreenRatioArray,
				SetDataText(TEXT("自动")),
				&UYCNGameUserSettings::GetCachedScreenRatio, &UYCNGameUserSettings::SetCachedScreenRatio, ScreenTab);

			//屏幕分辨率
			UYCNListData_String* ScreenResolution = NewObject<UYCNListData_String>();
			CreateStringData(ScreenResolution,
				TEXT("ScreenResolution"),
				SetDataText(TEXT("屏幕分辨率")),
				SetDataText(TEXT("调整游戏的屏幕分辨率。")),
				FText::GetEmpty(),
				GetResolutionOptions(),
				GetMaxResolutionText(),
				&UYCNGameUserSettings::GetCachedScreenResolution, &UYCNGameUserSettings::SetCachedScreenResolution, ScreenTab);

			//帧率上限
			UYCNListData_StringSwitch* FrameRateLimit = NewObject<UYCNListData_StringSwitch>();
			TArray<FText>FrameRateLimitArray =
			{
				SetDataText(TEXT("关闭")),
				SetDataText(TEXT("30")),
				SetDataText(TEXT("60")),
				SetDataText(TEXT("90")),
				SetDataText(TEXT("120"))
			};
			CreateStringSwitchData(FrameRateLimit,
				TEXT("FrameRateLimit"),
				SetDataText(TEXT("帧率上限")),
				SetDataText(TEXT("调整游戏的帧率。")),
				FText::GetEmpty(),
				FrameRateLimitArray,
				SetDataText(TEXT("关闭")),
				&UYCNGameUserSettings::GetCachedFrameRateLimit, &UYCNGameUserSettings::SetCachedFrameRateLimit, ScreenTab);

			//垂直同步
			UYCNListData_StringBool* VerticalSync = NewObject<UYCNListData_StringBool>();
			CreateStringBoolData(
				VerticalSync,
				TEXT("VerticalSync"),
				SetDataText(TEXT("垂直同步")),
				SetDataText(TEXT("调整游戏的垂直同步。")),
				FText::GetEmpty(), false,
				&UYCNGameUserSettings::GetVerticalSync, &UYCNGameUserSettings::SetVerticalSync, ScreenTab);
		}
		UYCNListData_Tab* ScreenEffect = NewObject<UYCNListData_Tab>();
		{
			CreateTabData(ScreenEffect, TEXT("ScreenEffect"), SetDataText(TEXT("显示效果")), DisplayTab);

			//动态模糊
			UYCNListData_StringSwitch* DynamicFuzzy = NewObject<UYCNListData_StringSwitch>();
			TArray<FText>DynamicFuzzyArray =
			{
				SetDataText(TEXT("自动")),
				SetDataText(TEXT("强")),
				SetDataText(TEXT("弱")),
				SetDataText(TEXT("关闭"))
			};
			CreateStringSwitchData(
				DynamicFuzzy,
				TEXT("DynamicFuzzy"),
				SetDataText(TEXT("动态模糊")),
				SetDataText(TEXT("调整游戏的动态模糊。")),
				FText::GetEmpty(),
				DynamicFuzzyArray,
				SetDataText(TEXT("自动")),
				&UYCNGameUserSettings::GetCachedDynamicFuzzy, &UYCNGameUserSettings::SetCachedDynamicFuzzy, ScreenEffect);

			//锐化强度
			UYCNListData_Float* SharpenAmount = NewObject<UYCNListData_Float>();
			CreateFloatData(
				SharpenAmount,
				TEXT("SharpenAmount"),
				SetDataText(TEXT("锐化强度")),
				SetDataText(TEXT("调整游戏的锐化强度。")),
				FText::GetEmpty(),
				0.f, 10.f, 5.f,
				&UYCNGameUserSettings::GetCachedSharpenAmount, &UYCNGameUserSettings::SetCachedSharpenAmount, ScreenEffect);

			//振动强度
			UYCNListData_Float* VibrationStrength = NewObject<UYCNListData_Float>();
			CreateFloatData(
				VibrationStrength,
				TEXT("VibrationStrength"),
				SetDataText(TEXT("振动强度")),
				SetDataText(TEXT("调整镜头的振动强度。")),
				FText::GetEmpty(),
				0.f, 10.f, 10.f,
				&UYCNGameUserSettings::GetCachedVibrationStrength, &UYCNGameUserSettings::SetCachedVibrationStrength, ScreenEffect);
		}
	}
}

void UYCNListData_RegisterData::InitHDList()
{
	UYCNListData_Tab* HDTab = NewObject<UYCNListData_Tab>();
	CreateTabData(HDTab, TEXT("HDTab"), SetDataText(TEXT("画质")));
	{
		UYCNListData_Tab* HDSetting = NewObject<UYCNListData_Tab>();
		CreateTabData(HDSetting, TEXT("HDSetting"), SetDataText(TEXT("画质设置")), HDTab);
		{
			//应用画质等级
			UYCNListData_Text* ApplyHD = NewObject<UYCNListData_Text>();
			CreateTextData(
				ApplyHD,
				TEXT("ApplyHD"),
				SetDataText(TEXT("应用画质等级")),
				SetDataText(TEXT("调整整体画质预设。")),
				EConfirmationWindowsType::YesNO,
				SetDataText(TEXT("确认应用当前选择的画质等级吗？")),
				[this](EConfirmationWindowsButtonType InButtonType)
				{
					if (InButtonType != EConfirmationWindowsButtonType::Yes || !UserSettings.IsValid()) return;

					UYCNGameUserSettings* Settings = UserSettings.Get();
					if (Settings && Settings->ApplyPendingQuality())
					{
						OnSettingValueChanged.ExecuteIfBound();
					}
				},
				HDSetting);
		}

		UYCNListData_Tab* SRSampling = NewObject<UYCNListData_Tab>();
		CreateTabData(SRSampling, TEXT("SRSampling"), SetDataText(TEXT("超分辨率采样")), HDTab);
		{
			//超采样清晰度
			UYCNListData_Float* SRSharpness = NewObject<UYCNListData_Float>();
			CreateFloatData(
				SRSharpness,
				TEXT("SRSharpness"),
				SetDataText(TEXT("超采样清晰度")),
				SetDataText(TEXT("调节超分辨率技术的锐化度或画质档位。")),
				FText::GetEmpty(),
				25.f, 100.f, 58.f,
				&UYCNGameUserSettings::GetCachedSRSharpness, &UYCNGameUserSettings::SetCachedSRSharpness, SRSampling);

			//超分辨率采样
			UYCNListData_StringSwitch* AAMethod = NewObject<UYCNListData_StringSwitch>();
			TArray<FText>AAMethodArray =
			{
				SetDataText(TEXT("FXAA")),
				SetDataText(TEXT("TAA")),
				SetDataText(TEXT("MSAA")),
				SetDataText(TEXT("TSR"))
			};
			CreateStringSwitchData(
				AAMethod,
				TEXT("AAMethod"),
				SetDataText(TEXT("超分辨率采样")),
				SetDataText(TEXT("低分辨率渲染后通过算法放大输出，提升帧率。")),
				FText::GetEmpty(),
				AAMethodArray,
				SetDataText(TEXT("TSR")),
				&UYCNGameUserSettings::GetCachedAAMethod, &UYCNGameUserSettings::SetCachedAAMethod, SRSampling);

			//帧生成
			UYCNListData_StringSwitch* FrameGeneration = NewObject<UYCNListData_StringSwitch>();
			TArray<FText>FrameGenerationArray =
			{
				SetDataText(TEXT("1X")),
				SetDataText(TEXT("2X")),
				SetDataText(TEXT("4X")),
				SetDataText(TEXT("8X"))
			};
			CreateStringSwitchData(
				FrameGeneration,
				TEXT("FrameGeneration"),
				SetDataText(TEXT("帧生成")),
				SetDataText(TEXT("插入过渡帧以提高流畅度。")),
				FText::GetEmpty(),
				FrameGenerationArray,
				SetDataText(TEXT("4X")),
				&UYCNGameUserSettings::GetCachedFrameGeneration, &UYCNGameUserSettings::SetCachedFrameGeneration, SRSampling);

			UYCNListData_Tab* NVIDIARayTracing = NewObject<UYCNListData_Tab>();

			CreateTabData(NVIDIARayTracing, TEXT("NVIDIARayTracing"), SetDataText(TEXT("NVIDIA全景光线追踪")), HDTab);

			//全景光线追踪
			UYCNListData_StringBool* RayTracing = NewObject<UYCNListData_StringBool>();
			CreateStringBoolData(
				RayTracing,
				TEXT("RayTracing"),
				SetDataText(TEXT("全景光线追踪")),
				SetDataText(TEXT("物理级模拟所有光影，效果逼真但极度耗性能。")),
				FText::GetEmpty(),
				false,
				&UYCNGameUserSettings::GetbRayTracing, &UYCNGameUserSettings::SetbRayTracing, NVIDIARayTracing);

			//全景光线追踪等级
			UYCNListData_StringSwitch* RayTracingLevel = NewObject<UYCNListData_StringSwitch>();
			TArray<FText>RayTracingLevelArray =
			{
				SetDataText(TEXT("弱")),
				SetDataText(TEXT("中")),
				SetDataText(TEXT("超高"))
			};

			CreateStringSwitchData(
				RayTracingLevel,
				TEXT("RayTracingLevel"),
				SetDataText(TEXT("全景光线追踪等级")),
				SetDataText(TEXT("物理级模拟所有光影，效果逼真但极度耗性能。")),
				SetDataText(TEXT("需要开启全景光线追踪")),
				RayTracingLevelArray,
				SetDataText(TEXT("弱")),
				&UYCNGameUserSettings::GetCachedRayTracingLevel, &UYCNGameUserSettings::SetCachedRayTracingLevel, NVIDIARayTracing,
				false,
				[this]()
				{
					return UserSettings.IsValid() && UserSettings->IsRayTracingEnabled();
				});
		}
		
		UYCNListData_Tab* GraphicsDetail = NewObject<UYCNListData_Tab>();
		CreateTabData(GraphicsDetail, TEXT("GraphicsDetail"), SetDataText(TEXT("画质细节")), HDTab);
		{
			//画质等级
			UYCNListData_StringSwitch* OverallScalabilityLevel = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				OverallScalabilityLevel,
				TEXT("OverallScalabilityLevel"),
				SetDataText(TEXT("画质等级")),
				SetDataText(TEXT("一键调整整体画质预设方案。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetOverallScalabilityLevelOverride, &UYCNGameUserSettings::SetOverallScalabilityLevelOverride, GraphicsDetail, false);

			//视野距离
			UYCNListData_StringSwitch* ViewDistanceQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				ViewDistanceQuality,
				TEXT("ViewDistanceQuality"),
				SetDataText(TEXT("视野距离")),
				SetDataText(TEXT("控制远处物体和场景的渲染范围。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetViewDistanceQualityOverride, &UYCNGameUserSettings::SetViewDistanceQualityOverride, GraphicsDetail, false);

			//抗锯齿
			UYCNListData_StringSwitch* AntiAliasingQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				AntiAliasingQuality,
				TEXT("AntiAliasingQuality"),
				SetDataText(TEXT("抗锯齿")),
				SetDataText(TEXT("平滑物体边缘，减少画面锯齿。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetAntiAliasingQualityOverride, &UYCNGameUserSettings::SetAntiAliasingQualityOverride, GraphicsDetail, false);

			//后期处理
			UYCNListData_StringSwitch* PostProcessingQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				PostProcessingQuality,
				TEXT("PostProcessingQuality"),
				SetDataText(TEXT("后期处理")),
				SetDataText(TEXT("调整画面色彩、镜头光晕等特效。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetPostProcessingQualityOverride, &UYCNGameUserSettings::SetPostProcessingQualityOverride, GraphicsDetail, false);

			//阴影质量
			UYCNListData_StringSwitch* ShadowQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				ShadowQuality,
				TEXT("ShadowQuality"),
				SetDataText(TEXT("阴影质量")),
				SetDataText(TEXT("决定阴影的清晰度与细腻程度。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetShadowQualityOverride, &UYCNGameUserSettings::SetShadowQualityOverride, GraphicsDetail, false);

			//贴图质量
			UYCNListData_StringSwitch* TextureQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				TextureQuality,
				TEXT("TextureQuality"),
				SetDataText(TEXT("贴图质量")),
				SetDataText(TEXT("控制游戏表面材质的清晰度。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetTextureQualityOverride, &UYCNGameUserSettings::SetTextureQualityOverride, GraphicsDetail, false);

			//特效质量
			UYCNListData_StringSwitch* VisualEffectQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				VisualEffectQuality,
				TEXT("VisualEffectQuality"),
				SetDataText(TEXT("特效质量")),
				SetDataText(TEXT("调节粒子、爆炸等特效的精细度。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetVisualEffectQualityOverride, &UYCNGameUserSettings::SetVisualEffectQualityOverride, GraphicsDetail, false);

			//植被质量
			UYCNListData_StringSwitch* FoliageQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				FoliageQuality,
				TEXT("FoliageQuality"),
				SetDataText(TEXT("植被质量")),
				SetDataText(TEXT("调节草木数量及渲染精细度。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetFoliageQualityOverride, &UYCNGameUserSettings::SetFoliageQualityOverride, GraphicsDetail, false);

			//全局光照
			UYCNListData_StringSwitch* GlobalIlluminationQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				GlobalIlluminationQuality,
				TEXT("GlobalIlluminationQuality"),
				SetDataText(TEXT("全局光照")),
				SetDataText(TEXT("模拟光线相互反射的真实光影。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetGlobalIlluminationQualityOverride, &UYCNGameUserSettings::SetGlobalIlluminationQualityOverride, GraphicsDetail, false);

			//反射质量
			UYCNListData_StringSwitch* ReflectionQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				ReflectionQuality,
				TEXT("ReflectionQuality"),
				SetDataText(TEXT("反射质量")),
				SetDataText(TEXT("控制水面及光滑表面的反射效果。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetReflectionQualityOverride, &UYCNGameUserSettings::SetReflectionQualityOverride, GraphicsDetail, false);

			//着色质量
			UYCNListData_StringSwitch* ShadingQuality = NewObject<UYCNListData_StringSwitch>();
			CreateStringSwitchData(
				ShadingQuality,
				TEXT("ShadingQuality"),
				SetDataText(TEXT("着色质量")),
				SetDataText(TEXT("调整游戏物体的表面明暗与光影精细度。")),
				FText::GetEmpty(),
				GraphicsLevelArray,
				SetDataText(TEXT("中")),
				&UYCNGameUserSettings::GetShadingQualityOverride, &UYCNGameUserSettings::SetShadingQualityOverride, GraphicsDetail, false);
		}
	}
}

void UYCNListData_RegisterData::InitAssistList()
{
	UYCNListData_Tab* AssistTab = NewObject<UYCNListData_Tab>();
	CreateTabData(AssistTab, TEXT("AssistTab"), SetDataText(TEXT("辅助设置")));

	//字幕
	UYCNListData_StringBool* Subtitle = NewObject<UYCNListData_StringBool>();
	CreateStringBoolData(
		Subtitle,
		TEXT("Subtitle"),
		SetDataText(TEXT("字幕")),
		SetDataText(TEXT("设置是否显示字幕。")),
		FText::GetEmpty(),
		true,
		&UYCNGameUserSettings::GetbSubtitle, &UYCNGameUserSettings::SetbSubtitle, AssistTab);

}

void UYCNListData_RegisterData::InitQuitList()
{
	UYCNListData_Tab* QuitTab = NewObject<UYCNListData_Tab>();
	CreateTabData(QuitTab, TEXT("QuitTab"), SetDataText(TEXT("退出游戏")));

	//退出至桌面
	UYCNListData_Text* QuitGame = NewObject<UYCNListData_Text>();
	CreateTextData(
		QuitGame,
		TEXT("QuitGame"),
		SetDataText(TEXT("退出至桌面")),
		SetDataText(TEXT("退出游戏到桌面。")),
		EConfirmationWindowsType::YesNO,
		SetDataText(TEXT("确认退出至桌面？")),
		[WeakContext = TWeakObjectPtr<UObject>(GetOuter())](EConfirmationWindowsButtonType InButtonType)
		{
			if (InButtonType != EConfirmationWindowsButtonType::Yes) return;

			if (UObject* Context = WeakContext.Get())
			{
				UKismetSystemLibrary::QuitGame(Context, nullptr, EQuitPreference::Quit, false);
			}
		},
		QuitTab);
}

FText UYCNListData_RegisterData::SetDataText(const FString& InDaraString)
{
	return FText::FromString(InDaraString);
}

FText UYCNListData_RegisterData::GetMaxResolutionText()
{
	if (UserSettings.IsValid())
	{
		return UserSettings->GetMaxResolutionText();
	}
	return FText();
}

TArray<FText> UYCNListData_RegisterData::GetResolutionOptions() const
{
	TArray<FText> Options;
	if (UserSettings.IsValid())
	{
		for (const FIntPoint& AddedOption : UserSettings->GetAvailableResolution())
		{
			Options.Add(UYCNGameUserSettings::GetResolutionText(AddedOption));
		}
	}
	return Options;
}

void UYCNListData_RegisterData::CreateTextData(UYCNListData_Text* InData, FName InDataID, const FText& InDisplayText,
	const FText& InDescriptionText, EConfirmationWindowsType InWindowType,
	const FText& InMessage, TFunction<void(EConfirmationWindowsButtonType)>InFunction, UYCNListData_Base* InParentData)
{
	if (!IsValid(InData) || !IsValid(InParentData) || InDataID.IsNone()) return;

	InData->SetDataID(InDataID);
	InData->SetDataDisplayText(InDisplayText);
	InData->SetDataDescription(InDescriptionText);
	InData->SetClickedWindowsType(InWindowType);
	InData->SetWindowsMessageText(InMessage);
	InData->SetClickCallBack(InFunction);
	InParentData->AddChildData(InData);
}

void UYCNListData_RegisterData::CreateImageData(UYCNListData_Image* InData, FName InDataID, const FText& InDisplayText, const FText& InDescriptionText, 
	UTexture2D* InDataIcon, TFunction<void()> InFunction, UYCNListData_Base* InParentData)
{
	if (!IsValid(InData) || !IsValid(InParentData) || InDataID.IsNone()) return;

	InData->SetDataID(InDataID);
	InData->SetDataDisplayText(InDisplayText);
	InData->SetDataDescription(InDescriptionText);
	InData->SetDataIcon(InDataIcon);
	InData->SetClickCallBack(InFunction);
	InParentData->AddChildData(InData);
}

void UYCNListData_RegisterData::CreateTabData(UYCNListData_Tab* InData, FName InDataID,
	const FText& InDisplayText, UYCNListData_Base* InParentData)
{
	if (!IsValid(InData) || InDataID.IsNone() || (InParentData && !IsValid(InParentData))) return;

	InData->SetDataID(InDataID);
	InData->SetDataDisplayText(InDisplayText);
	if (InParentData)
	{
		InParentData->AddChildData(InData);
	}
	else
	{
		AvailableDataList.Add(InData);
	}
}
