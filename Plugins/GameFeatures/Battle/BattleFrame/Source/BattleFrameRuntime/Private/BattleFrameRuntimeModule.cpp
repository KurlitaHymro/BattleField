// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleFrameRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FBattleFrameRuntimeModule"

void FBattleFrameRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FBattleFrameRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBattleFrameRuntimeModule, BattleFrameRuntime)

DEFINE_LOG_CATEGORY(LogBattleFrame)
