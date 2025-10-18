// Copyright 2025. Jinsol Co. All rights reserved


#include "PlayergroundTypes/PlaygroundCountDownAction.h"

void FPlaygroundCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = EPlaygroundCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EPlaygroundCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = EPlaygroundCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

		ElapsedInterval = 0.f;
	}
}

void FPlaygroundCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
