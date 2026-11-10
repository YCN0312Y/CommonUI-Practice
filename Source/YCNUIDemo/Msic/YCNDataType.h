
#pragma once

#include "CoreMinimal.h"

//窗口类型
UENUM()
enum class EConfirmationWindowsType : uint8
{
	OK,
	YesNO,
	None,
};

//窗口按钮类型
UENUM()
enum class EConfirmationWindowsButtonType : uint8
{
	Yes,
	No,
};
