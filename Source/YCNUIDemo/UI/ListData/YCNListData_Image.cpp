// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"

void UYCNListData_Image::SetClickCallBack(TFunction<void()> InClickCallBack)
{
	ClickCallBack = InClickCallBack;
}