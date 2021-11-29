// Copyright Epic Games, Inc. All Rights Reserved.


#include "BoidsFlyGameModeBase.h"
#include "BoidsFly.h"
#include "Kismet\GameplayStatics.h"
#include "MyBoid.h"


ABoidsFlyGameModeBase::ABoidsFlyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidsFlyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UseGPU)
	{
		ComputeTest();
	}
}

void ABoidsFlyGameModeBase::ComputeTest()
{
	RenderEveryFrameLock.Lock();
	FMyBoidModule::Get().BoidInfoSave.AovRadius = 20;
	FMyBoidModule::Get().BoidInfoSave.ViewRadius = 100;
	//FMyBoidModule::Get().BoidInfoSave.BoidBase.Empty();
	TArray<AActor*> AllBoid;
	UGameplayStatics::GetAllActorsOfClass(this, AMyBoid::StaticClass(), AllBoid);
	FMyBoidModule::Get().BoidInfoSave.NumTotal = AllBoid.Num();

	for (int i=0; i< AllBoid.Num(); i++)
	{
		AMyBoid* Bird = Cast<AMyBoid>(AllBoid[i]);
		if (Bird)
		{
			if (!FMyBoidModule::Get().BoidInfoSave.BoidBase.Contains(i))
			{
				FMyBoidModule::Get().BoidInfoSave.BoidBase.Add(Bird->BirdId, FMyBoidBase(Bird->GetActorLocation(), Bird->GetCurVelocity()));
			}
		}
	}
	RenderEveryFrameLock.Unlock();

	ENQUEUE_RENDER_COMMAND(BackgroundTickRenderThread)(
		[](FRHICommandListImmediate& RHICmdList) {
		FMyBoidModule::Get().RunComputeShader(RHICmdList);
	});
	/*
	for (int i = 0; i < AllBoid.Num(); i++)
	{
		if (FMyBoidModule::Get().BoidInfoSave.BoidBase.Contains(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("BoidId = %d, BoidNearNum = %d"), i, FMyBoidModule::Get().BoidInfoSave.BoidBase[i].BoidNearNum);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("DOonce!!!!!!"));*/
}
