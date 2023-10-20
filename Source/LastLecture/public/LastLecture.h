// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LastLecture, Log, All);

#define LLLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LLLOG_S(Verbosity) UE_LOG(LastLecture, Verbosity, TEXT("%s"), *LLLOG_CALLINFO)
#define LLLOG(Verbosity, Format, ...) UE_LOG(LastLecture, Verbosity, TEXT("%s%s"), *LLLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LLCHECK(Expr, ...)	{ if(!(Expr)) {LLLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}