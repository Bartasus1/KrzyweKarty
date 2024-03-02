﻿#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTileStatusAssetTypeActions;

class FKrzyweKartyEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TSharedPtr<FTileStatusAssetTypeActions> TileStatusAssetTypeActions;
};
