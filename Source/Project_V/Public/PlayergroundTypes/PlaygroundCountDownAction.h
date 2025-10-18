// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"

class FPlaygroundCountDownAction : public FPendingLatentAction
{
public:
	FPlaygroundCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, EPlaygroundCountDownActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
		: bNeedToCancel(false)
		, TotalCountDownTime(InTotalCountDownTime)
		, UpdateInterval(InUpdateInterval)
		, OutRemainingTime(InOutRemainingTime)
		, CountDownOutput(InCountDownOutput)
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
		, ElapsedInterval(0.f)
		, ElapsedTimeSinceStart(0.f)
	{

	}

	virtual void UpdateOperation(FLatentResponse& Response) override;

	void CancelAction();

private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval; 
	float& OutRemainingTime;
	EPlaygroundCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
