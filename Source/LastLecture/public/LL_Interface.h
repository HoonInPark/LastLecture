// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastLecture.h"
#include "UObject/Interface.h"
#include "LL_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULL_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LASTLECTURE_API ILL_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void Point_Message(FVector _PointPosition);
};
