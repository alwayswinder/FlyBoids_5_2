// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoidsFly.h"
#include "Modules/ModuleManager.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "ShaderParameterStruct.h"
#include "RHIUtilities.h"
#include "RenderGraphUtils.h"


IMPLEMENT_PRIMARY_GAME_MODULE(FMyBoidModule, BoidsFly, "BoidsFly");


class FMyBoidComputeShader : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMyBoidComputeShader)
	SHADER_USE_PARAMETER_STRUCT(FMyBoidComputeShader, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_UAV(RWBuffer<int>, TestParameters)
		SHADER_PARAMETER(int, NumBoid)
		END_SHADER_PARAMETER_STRUCT()

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static inline void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}
};
IMPLEMENT_SHADER_TYPE(, FMyBoidComputeShader, TEXT("/MyShaders/BirdCompute.usf"), TEXT("MainCS"), SF_Compute);


void FMyBoidModule::RunComputeShader(FRHICommandListImmediate& RHICmdList)
{
	TResourceArray<int32> InitialTestParams;
	InitialTestParams.Add(CachedBoidParameters.TestInts[0]);
	InitialTestParams.Add(CachedBoidParameters.TestInts[1]);
	TestParametes.Initialize(4, 2, PF_R32_SINT, BUF_UnorderedAccess | BUF_SourceCopy, TEXT("TestParametes"), &InitialTestParams);

	//QUICK_SCOPE_CYCLE_COUNTER(STAT_BoidFly_ComputeShader); // Used to gather CPU profiling data for the UE4 session frontend

	TShaderMapRef<FMyBoidComputeShader> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	RHICmdList.Transition(FRHITransitionInfo(TestParametes.UAV, ERHIAccess::UAVCompute, ERHIAccess::SRVMask | ERHIAccess::CopySrc));

	FMyBoidComputeShader::FParameters PassParameters;
	PassParameters.TestParameters = TestParametes.UAV;
	PassParameters.NumBoid = 2;

	FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, PassParameters, FIntVector(2, 1, 1));

	int32* Buffer = (int32*)RHICmdList.LockVertexBuffer(TestParametes.Buffer, 0, sizeof(int32)*2, EResourceLockMode::RLM_ReadOnly);
	CachedBoidParameters.TestInts[0] = Buffer[0];
	CachedBoidParameters.TestInts[1] = Buffer[1];
	RHICmdList.UnlockVertexBuffer(TestParametes.Buffer);

	/*FRHIGPUBufferReadback NumBricksReadback(TEXT("NumBricksReadback"));
	NumBricksReadback.EnqueueCopy(RHICmdList, TestParametes.Buffer);
	RHICmdList.BlockUntilGPUIdle();
	if (NumBricksReadback.IsReady())
	{
		int32* Buffer = (int32*)NumBricksReadback.Lock(8);
		CachedBoidParameters.TestInts[0] = Buffer[0];
		CachedBoidParameters.TestInts[1] = Buffer[1];
		NumBricksReadback.Unlock();
	}*/
}

void FMyBoidModule::UpdateParameters(struct FMyBoidParameters& BoidParameters)
{
	RenderEveryFrameLock.Lock();
	CachedBoidParameters = BoidParameters;
	bCachedParametersValid = true;
	RenderEveryFrameLock.Unlock();
}

void FMyBoidModule::StartupModule()
{
	FString ShaderPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/MyShaders"), ShaderPath);
}

void FMyBoidModule::ShutdownModule()
{

}
