// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_AnimInstance.h"

ULL_AnimInstance::ULL_AnimInstance()
{
}

void ULL_AnimInstance::Point_Message_Implementation(FVector _TargetLocation)
{
	TargetLocation = _TargetLocation;
	bIsPointing = true;
}

void ULL_AnimInstance::AnimNotify_PointEnded()
{
	bIsPointing = false;
}


