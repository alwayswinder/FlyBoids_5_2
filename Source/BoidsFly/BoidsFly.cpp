// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoidsFly.h"
#include "Modules/ModuleManager.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "ShaderParameterStruct.h"
#include "RHIUtilities.h"
#include "RenderGraphUtils.h"


IMPLEMENT_PRIMARY_GAME_MODULE(FMyBoidModule, BoidsFly, "BoidsFly");


void FMyBoidModule::StartupModule()
{
	FString ShaderPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/MyShaders"), ShaderPath);
}

void FMyBoidModule::ShutdownModule()
{

}
