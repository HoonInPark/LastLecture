// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastLecture.h"
#include "Animation/AnimInstance.h"
#include "LL_Interface.h"
#include "LL_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LASTLECTURE_API ULL_AnimInstance : public UAnimInstance, public ILL_Interface
{
	GENERATED_BODY()

	ULL_AnimInstance();

protected:
	UPROPERTY(BlueprintReadOnly, Category=IKRig)
	FVector TargetLocation{ FVector(0.f, 0.f, 0.f) };
	UPROPERTY(BlueprintReadOnly, Category=IKRig)
	bool bIsPointing{ false };

private:
	virtual void Point_Message_Implementation(FVector _TargetLocation) override;

private:
	UFUNCTION()
	void AnimNotify_PointEnded();
};
