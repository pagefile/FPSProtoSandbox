#include "FPSProtoSandboxEditor.h"
 
IMPLEMENT_GAME_MODULE(FFPSProtoSandboxEditorModule, FPSProtoSandboxEditor);

DEFINE_LOG_CATEGORY(FPSProtoSandboxEditor)
 
#define LOCTEXT_NAMESPACE "FPSProtoSandboxEditor"
 
void FFPSProtoSandboxEditorModule::StartupModule()
{
	UE_LOG(FPSProtoSandboxEditor, Warning, TEXT("FPSProtoSandboxEditor: Log Started"));
}
 
void FFPSProtoSandboxEditorModule::ShutdownModule()
{
	UE_LOG(FPSProtoSandboxEditor, Warning, TEXT("FPSProtoSandboxEditor: Log Ended"));
}
 
#undef LOCTEXT_NAMESPACE