
#pragma once

#include "CoreMinimal.h"

//窗口类型
UENUM()
enum class EConfirmationWindowsType : uint8
{
	OK,
	YesNO,
};

//窗口按钮类型
UENUM()
enum class EConfirmationWindowsButtonType : uint8
{
	Yes,
	No,
};