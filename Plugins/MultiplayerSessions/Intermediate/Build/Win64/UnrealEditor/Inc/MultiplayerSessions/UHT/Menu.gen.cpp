// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Menu.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeMenu() {}

// ********** Begin Cross Module References ********************************************************
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_UMenu();
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_UMenu_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_MultiplayerSessions();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UMenu Function HostButtonClicked *****************************************
struct Z_Construct_UFunction_UMenu_HostButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "HostButtonClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UMenu_HostButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execHostButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HostButtonClicked();
	P_NATIVE_END;
}
// ********** End Class UMenu Function HostButtonClicked *******************************************

// ********** Begin Class UMenu Function JoinButtonClicked *****************************************
struct Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "JoinButtonClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UMenu_JoinButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execJoinButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->JoinButtonClicked();
	P_NATIVE_END;
}
// ********** End Class UMenu Function JoinButtonClicked *******************************************

// ********** Begin Class UMenu Function MenuSetup *************************************************
struct Z_Construct_UFunction_UMenu_MenuSetup_Statics
{
	struct Menu_eventMenuSetup_Parms
	{
		int32 NumberOfPublicConnections;
		FString TypeOfMatch;
		FString LobbyPath;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Setup" },
		{ "CPP_Default_LobbyPath", "/Game/Levels/Lobby/Lobby?listen" },
		{ "CPP_Default_NumberOfPublicConnections", "4" },
		{ "CPP_Default_TypeOfMatch", "FreeForAll" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NumberOfPublicConnections;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TypeOfMatch;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LobbyPath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_NumberOfPublicConnections = { "NumberOfPublicConnections", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventMenuSetup_Parms, NumberOfPublicConnections), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_TypeOfMatch = { "TypeOfMatch", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventMenuSetup_Parms, TypeOfMatch), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_LobbyPath = { "LobbyPath", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventMenuSetup_Parms, LobbyPath), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_NumberOfPublicConnections,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_TypeOfMatch,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_LobbyPath,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "MenuSetup", Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_MenuSetup_Statics::Menu_eventMenuSetup_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_MenuSetup_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_MenuSetup_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_MenuSetup_Statics::Menu_eventMenuSetup_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_MenuSetup()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_MenuSetup_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execMenuSetup)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NumberOfPublicConnections);
	P_GET_PROPERTY(FStrProperty,Z_Param_TypeOfMatch);
	P_GET_PROPERTY(FStrProperty,Z_Param_LobbyPath);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->MenuSetup(Z_Param_NumberOfPublicConnections,Z_Param_TypeOfMatch,Z_Param_LobbyPath);
	P_NATIVE_END;
}
// ********** End Class UMenu Function MenuSetup ***************************************************

// ********** Begin Class UMenu Function OnCreateSessionComplete ***********************************
struct Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics
{
	struct Menu_eventOnCreateSessionComplete_Parms
	{
		bool bWasSuccesful;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\n// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem\n//\n" },
#endif
		{ "ModuleRelativePath", "Public/Menu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Callbacks for the custom delegates on the MultiplayerSessionsSubsystem" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bWasSuccesful_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWasSuccesful;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::NewProp_bWasSuccesful_SetBit(void* Obj)
{
	((Menu_eventOnCreateSessionComplete_Parms*)Obj)->bWasSuccesful = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::NewProp_bWasSuccesful = { "bWasSuccesful", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventOnCreateSessionComplete_Parms), &Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::NewProp_bWasSuccesful_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::NewProp_bWasSuccesful,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnCreateSessionComplete", Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::Menu_eventOnCreateSessionComplete_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::Menu_eventOnCreateSessionComplete_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_OnCreateSessionComplete()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnCreateSessionComplete_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnCreateSessionComplete)
{
	P_GET_UBOOL(Z_Param_bWasSuccesful);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnCreateSessionComplete(Z_Param_bWasSuccesful);
	P_NATIVE_END;
}
// ********** End Class UMenu Function OnCreateSessionComplete *************************************

// ********** Begin Class UMenu Function OnDestroySessionComplete **********************************
struct Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics
{
	struct Menu_eventOnDestroySessionComplete_Parms
	{
		bool bWasSuccessful;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bWasSuccessful_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWasSuccessful;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::NewProp_bWasSuccessful_SetBit(void* Obj)
{
	((Menu_eventOnDestroySessionComplete_Parms*)Obj)->bWasSuccessful = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::NewProp_bWasSuccessful = { "bWasSuccessful", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventOnDestroySessionComplete_Parms), &Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::NewProp_bWasSuccessful_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::NewProp_bWasSuccessful,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnDestroySessionComplete", Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::Menu_eventOnDestroySessionComplete_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::Menu_eventOnDestroySessionComplete_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_OnDestroySessionComplete()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnDestroySessionComplete_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnDestroySessionComplete)
{
	P_GET_UBOOL(Z_Param_bWasSuccessful);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnDestroySessionComplete(Z_Param_bWasSuccessful);
	P_NATIVE_END;
}
// ********** End Class UMenu Function OnDestroySessionComplete ************************************

// ********** Begin Class UMenu Function OnStartSessionComplete ************************************
struct Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics
{
	struct Menu_eventOnStartSessionComplete_Parms
	{
		bool bWasSuccessful;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bWasSuccessful_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWasSuccessful;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::NewProp_bWasSuccessful_SetBit(void* Obj)
{
	((Menu_eventOnStartSessionComplete_Parms*)Obj)->bWasSuccessful = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::NewProp_bWasSuccessful = { "bWasSuccessful", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventOnStartSessionComplete_Parms), &Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::NewProp_bWasSuccessful_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::NewProp_bWasSuccessful,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnStartSessionComplete", Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::Menu_eventOnStartSessionComplete_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::Menu_eventOnStartSessionComplete_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_OnStartSessionComplete()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnStartSessionComplete_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnStartSessionComplete)
{
	P_GET_UBOOL(Z_Param_bWasSuccessful);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnStartSessionComplete(Z_Param_bWasSuccessful);
	P_NATIVE_END;
}
// ********** End Class UMenu Function OnStartSessionComplete **************************************

// ********** Begin Class UMenu ********************************************************************
void UMenu::StaticRegisterNativesUMenu()
{
	UClass* Class = UMenu::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "HostButtonClicked", &UMenu::execHostButtonClicked },
		{ "JoinButtonClicked", &UMenu::execJoinButtonClicked },
		{ "MenuSetup", &UMenu::execMenuSetup },
		{ "OnCreateSessionComplete", &UMenu::execOnCreateSessionComplete },
		{ "OnDestroySessionComplete", &UMenu::execOnDestroySessionComplete },
		{ "OnStartSessionComplete", &UMenu::execOnStartSessionComplete },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UMenu;
UClass* UMenu::GetPrivateStaticClass()
{
	using TClass = UMenu;
	if (!Z_Registration_Info_UClass_UMenu.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("Menu"),
			Z_Registration_Info_UClass_UMenu.InnerSingleton,
			StaticRegisterNativesUMenu,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UMenu.InnerSingleton;
}
UClass* Z_Construct_UClass_UMenu_NoRegister()
{
	return UMenu::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UMenu_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "Menu.h" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HostButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_JoinButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_HostButton;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_JoinButton;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UMenu_HostButtonClicked, "HostButtonClicked" }, // 746207048
		{ &Z_Construct_UFunction_UMenu_JoinButtonClicked, "JoinButtonClicked" }, // 1633719576
		{ &Z_Construct_UFunction_UMenu_MenuSetup, "MenuSetup" }, // 1738864151
		{ &Z_Construct_UFunction_UMenu_OnCreateSessionComplete, "OnCreateSessionComplete" }, // 112320208
		{ &Z_Construct_UFunction_UMenu_OnDestroySessionComplete, "OnDestroySessionComplete" }, // 868547521
		{ &Z_Construct_UFunction_UMenu_OnStartSessionComplete, "OnStartSessionComplete" }, // 1440539603
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMenu>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_HostButton = { "HostButton", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, HostButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HostButton_MetaData), NewProp_HostButton_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_JoinButton = { "JoinButton", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, JoinButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_JoinButton_MetaData), NewProp_JoinButton_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMenu_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_HostButton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_JoinButton,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UMenu_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSessions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMenu_Statics::ClassParams = {
	&UMenu::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UMenu_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::Class_MetaDataParams), Z_Construct_UClass_UMenu_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMenu()
{
	if (!Z_Registration_Info_UClass_UMenu.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMenu.OuterSingleton, Z_Construct_UClass_UMenu_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMenu.OuterSingleton;
}
UMenu::UMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMenu);
UMenu::~UMenu() {}
// ********** End Class UMenu **********************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Blaster_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h__Script_MultiplayerSessions_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMenu, UMenu::StaticClass, TEXT("UMenu"), &Z_Registration_Info_UClass_UMenu, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMenu), 446227801U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Blaster_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h__Script_MultiplayerSessions_378890537(TEXT("/Script/MultiplayerSessions"),
	Z_CompiledInDeferFile_FID_Blaster_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h__Script_MultiplayerSessions_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Blaster_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h__Script_MultiplayerSessions_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
