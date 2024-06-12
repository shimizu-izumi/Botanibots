// // Copyright Furkan Kursav, 2023-2024, Inc. All Rights Reserved.

#include "CommonMaps.h"
#include "AssetSelection.h"
#include "CommonMapsDeveloperSettings.h"
#include "FileHelpers.h"

#define LOCTEXT_NAMESPACE "FCommonMapsModule"

namespace CommonMapsFunctionLibrary
{
	static bool HasPlayWorld()
	{
		return GEditor->PlayWorld != nullptr;
	}

	static bool HasNoPlayWorld()
	{
		return !HasPlayWorld();
	}

	static void OpenCommonMap_Clicked(const FString MapPath)
	{
		constexpr bool bPromptUserToSave = true;
		constexpr bool bSaveMapPackages = true;
		constexpr bool bSaveContentPackages = true;
		constexpr bool bFastSave = false;
		constexpr bool bNotifyNoPackagesSaved = false;
		constexpr bool bCanBeDeclined = true;
		if (FEditorFileUtils::SaveDirtyPackages(bPromptUserToSave, bSaveMapPackages, bSaveContentPackages, bFastSave,
		                                        bNotifyNoPackagesSaved, bCanBeDeclined))
		{
			if (ensure(MapPath.Len()))
			{
				GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(MapPath);
			}
		}
	}

	static TSharedRef<SWidget> GetCommonMapsDropdown()
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		for (const auto& [CategoryName, Maps] : GetDefault<UCommonMapsDeveloperSettings>()->Maps)
		{
			TAttribute<FText> SectionText;
			SectionText.Set(FText::FromName(CategoryName));
			MenuBuilder.BeginSection(NAME_None, SectionText);
			for (const auto& Map : Maps.MapURL)
			{
				if (!Map.IsValid())
				{
					continue;
				}

				const FText DisplayName = FText::FromString(Map.GetAssetName());
				MenuBuilder.AddMenuEntry(
					DisplayName,
					FText::Format(LOCTEXT("CommonPathDescription", "{0}"), FText::FromString(Map.ToString())),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::OpenCommonMap_Clicked, Map.ToString()),
						FCanExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld),
						FIsActionChecked(),
						FIsActionButtonVisible::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld)
					)
				);
			}
			MenuBuilder.EndSection();
		}

		return MenuBuilder.MakeWidget();
	}

	static bool CanShowCommonMaps()
	{
		return HasNoPlayWorld() && !GetDefault<UCommonMapsDeveloperSettings>()->Maps.IsEmpty();
	}

	static void RegisterGameEditorMenus()
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = Menu->AddSection("PlayGameExtensions", TAttribute<FText>(),
		                                             FToolMenuInsert("Play", EToolMenuInsertType::Default));


		FToolMenuEntry CommonMapEntry = FToolMenuEntry::InitComboButton(
			"CommonMapOptions",
			FUIAction(
				FExecuteAction(),
				FCanExecuteAction::CreateStatic(&CommonMapsFunctionLibrary::HasNoPlayWorld),
				FIsActionChecked(),
				FIsActionButtonVisible::CreateStatic(&CommonMapsFunctionLibrary::CanShowCommonMaps)),
			FOnGetContent::CreateStatic(&CommonMapsFunctionLibrary::GetCommonMapsDropdown),
			LOCTEXT("CommonMaps_Label", "Common Maps"),
			LOCTEXT("CommonMaps_ToolTip", "Some commonly desired maps while using the editor"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level")
		);
		CommonMapEntry.StyleNameOverride = "CalloutToolbar";
		Section.AddEntry(CommonMapEntry);
	}
}

void FCommonMapsModule::StartupModule()
{
	ExtendContextMenu();

	if (!IsRunningGame())
	{
		if (FSlateApplication::IsInitialized())
		{
			ToolMenusHandle = UToolMenus::RegisterStartupCallback(
				FSimpleMulticastDelegate::FDelegate::CreateStatic(&CommonMapsFunctionLibrary::RegisterGameEditorMenus));
		}
	}
}

void FCommonMapsModule::ExtendContextMenu()
{
	UToolMenu* WorldAssetMenu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu.World");
	if (!WorldAssetMenu) return;

	FToolMenuSection& Section = WorldAssetMenu->AddSection("AssetContextCommonMaps",
	                                                       LOCTEXT("CommonMapsOptionsMenuHeading", "Common Maps"));
	
	Section.AddSubMenu(FName("Common Maps"),
		LOCTEXT("AddToCommonMapsFromMenu", "Add to Common Maps"),
		LOCTEXT("AddToCommonMapsFromMenuTooltip", "Add this map into Common Maps list."),
		FNewMenuDelegate::CreateRaw(this, &FCommonMapsModule::CreateCategorySelectionSubmenu),
		FToolUIActionChoice(),
		EUserInterfaceActionType::None,
		false,
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Star"));
}

void FCommonMapsModule::AddToCommonMapsFromMenu(FName CategoryName)
{
	TArray<FAssetData> SelectedAssets;
	AssetSelectionUtils::GetSelectedAssets(SelectedAssets);
	if (auto* Settings = GetMutableDefault<UCommonMapsDeveloperSettings>())
	{
		for (const auto& AssetData : SelectedAssets)
		{
			if (FCommonMapContainer* CurrentMaps = Settings->Maps.Find(CategoryName))
				CurrentMaps->MapURL.Add(AssetData.GetSoftObjectPath());
			
			else
				Settings->Maps.Add(CategoryName, FCommonMapContainer({AssetData.GetSoftObjectPath()}));
		}
		
		Settings->SaveConfig(CPF_Config);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("All maps added to Common Maps!"));
	}
}

void FCommonMapsModule::CreateCategorySelectionSubmenu(FMenuBuilder& MenuBuilder)
{
	const UCommonMapsDeveloperSettings* DevSettings = GetDefault<UCommonMapsDeveloperSettings>();
	if (!DevSettings) return;

	TArray<FName> Categories;
	DevSettings->Maps.GetKeys(Categories);
	for (const FName& CategoryName : Categories)
	{
		MenuBuilder.AddMenuEntry(
			FText::Format(LOCTEXT("CategoryLabel", "{0}"), FText::FromName(CategoryName)),
			FText::Format(LOCTEXT("CategoryTooltip", "Add this map to \"{0}\" category."), FText::FromName(CategoryName)),
			FSlateIcon(), FUIAction(FExecuteAction::CreateRaw(this, &FCommonMapsModule::AddToCommonMapsFromMenu, CategoryName)));
	}
}


void FCommonMapsModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommonMapsModule, CommonMaps)
