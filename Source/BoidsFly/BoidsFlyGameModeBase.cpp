// Copyright Epic Games, Inc. All Rights Reserved.


#include "BoidsFlyGameModeBase.h"
#include "BoidsFly.h"


ABoidsFlyGameModeBase::ABoidsFlyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidsFlyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TArray<int> Out = FMyBoidModule::Get().CachedBoidParameters.TestInts;

	UE_LOG(LogTemp, Warning, TEXT("TestInts=%d,,%d"), Out[0], Out[1]);
}

void ABoidsFlyGameModeBase::ComputeTest()
{
	ENQUEUE_RENDER_COMMAND(BackgroundTickRenderThread)(
		[](FRHICommandListImmediate& RHICmdList) {
		FMyBoidModule::Get().RunComputeShader(RHICmdList);
	});
}
