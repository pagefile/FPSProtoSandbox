#pragma once
 
#include "Engine.h"
#include "FPSProtoSandboxEditor.h"
#include "ModuleManager.h"
#include "UnrealEd.h"
 
DECLARE_LOG_CATEGORY_EXTERN(FPSProtoSandboxEditor, All, All)
 
class FFPSProtoSandboxEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
 
};