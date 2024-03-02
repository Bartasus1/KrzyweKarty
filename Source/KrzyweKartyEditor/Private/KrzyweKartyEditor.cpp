#include "KrzyweKartyEditor.h"

#include "TileStatus/TileStatusFactory.h"

#define LOCTEXT_NAMESPACE "FKrzyweKartyEditorModule"

void FKrzyweKartyEditorModule::StartupModule()
{
	TileStatusAssetTypeActions = MakeShared<FTileStatusAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(TileStatusAssetTypeActions.ToSharedRef());
}

void FKrzyweKartyEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(TileStatusAssetTypeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FKrzyweKartyEditorModule, KrzyweKartyEditor)