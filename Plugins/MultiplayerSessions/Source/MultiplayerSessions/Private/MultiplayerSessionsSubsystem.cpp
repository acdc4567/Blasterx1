// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
    CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnCreateSessionComplete))
    ,FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::OnFindSessionsComplete))
    ,JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnJoinSessionComplete))
    ,DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this,&ThisClass::OnDestroySessionComplete))
    ,StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnStartSessionComplete))
{
    IOnlineSubsystem* Subsystem=IOnlineSubsystem::Get();
    if(Subsystem){
        SessionInterface=Subsystem->GetSessionInterface();
    }




}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections,FString MatchType){
    if(!SessionInterface.IsValid())return;
    auto ExistingSession=SessionInterface->GetNamedSession(NAME_GameSession);
    if(ExistingSession!=nullptr){
        SessionInterface->DestroySession(NAME_GameSession);

    }
    //Store dlegateIn a FDelegateHandle
    CreateSessionCompleteDelegateHandle=SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
    LastSessionSettings=MakeShareable(new FOnlineSessionSettings());
    LastSessionSettings->bIsLANMatch=IOnlineSubsystem::Get()->GetSubsystemName()=="NULL"?true:false;
    LastSessionSettings->NumPublicConnections=NumPublicConnections;
    LastSessionSettings->bAllowJoinInProgress=1;
    LastSessionSettings->bAllowJoinViaPresence=1;
    LastSessionSettings->bShouldAdvertise=1;
    LastSessionSettings->bUsesPresence=1;
    LastSessionSettings->Set(FName("MatchType"),MatchType,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    const ULocalPlayer* LocalPlayer=GetWorld()->GetFirstLocalPlayerFromController();
    if(!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession,*LastSessionSettings)){
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
        //Broadcast ourown custom delegate
        MultiplayerOnCreateSessionComplete.Broadcast(0);

    }


}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults){
    
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult){
    
}

void UMultiplayerSessionsSubsystem::DestroySession(){
    
}

void UMultiplayerSessionsSubsystem::StartSession(){
    
}


//Callbacks

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName,bool bWasSuccessful){
    if(SessionInterface){
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

    }
    MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
    
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful){
    
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type Result){
    
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName,bool bWasSuccessful){
    
}


void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName,bool bWasSuccessful){
    
}



//











//UMultiplayerSessionsSubsystem