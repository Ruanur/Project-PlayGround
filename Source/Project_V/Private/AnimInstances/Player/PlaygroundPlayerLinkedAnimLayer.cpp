// Copyright 2025. Jinsol Co. All rights reserved


#include "AnimInstances/Player/PlaygroundPlayerLinkedAnimLayer.h"
#include "AnimInstances/Player/PlaygroundPlayerAnimInstance.h"

UPlaygroundPlayerAnimInstance* UPlaygroundPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
    return Cast<UPlaygroundPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());;
}
