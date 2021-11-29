// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BoidsFlyGameModeBase.generated.h"



UCLASS()
class BOIDSFLY_API ABoidsFlyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABoidsFlyGameModeBase();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Boid")
	bool UseGPU = true;
	UFUNCTION(BlueprintCallable)
	void ComputeTest();

	FCriticalSection RenderEveryFrameLock;
};
