// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	FString PathToLobby{TEXT("")};
	
public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/Levels/Lobby/Lobby?listen")));
protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	
	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccesful);
	void OnFindSessionComplete(const TArray<FOnlineSessionSearchResult> &SessionResults, bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySessionComplete(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSessionComplete(bool bWasSuccessful);
private:
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	void MenuTearDown();


};
