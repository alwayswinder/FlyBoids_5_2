// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct FMyBoidParameters
{
	TArray<int> TestInts;

	FMyBoidParameters()
	{
		TestInts.Add(0);
		TestInts.Add(0);
	}
};

class FMyBoidModule : public IModuleInterface
{
public:
	static inline FMyBoidModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FMyBoidModule>("BoidsFly");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("BoidsFly");
	}
	void RunComputeShader(FRHICommandListImmediate& RHICmdList);

	void UpdateParameters(struct FMyBoidParameters& BoidParameters);

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	struct FMyBoidParameters CachedBoidParameters;

private:
	FRWBuffer TestParametes;
	FCriticalSection RenderEveryFrameLock;
	volatile bool bCachedParametersValid;
};

