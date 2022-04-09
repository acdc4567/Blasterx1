// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/BlasterChar.h"
#include "Net/UnrealNetwork.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = 0;
	bReplicates=1;

	WeaponMesh=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	//WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);


	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AreaSphere=CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickupWidget=CreateDefaultSubobject<UWidgetComponent>("PickupWidget");
	PickupWidget->SetupAttachment(RootComponent);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(PickupWidget){
		PickupWidget->SetVisibility(0);
		
	}
	if(GetLocalRole()==ENetRole::ROLE_Authority){
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this,&AWeapon::OnSphereEndOverlap);
		
	}
	
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult){

	ABlasterChar* BlasterCharacter=Cast<ABlasterChar>(OtherActor);
	if(BlasterCharacter){

		BlasterCharacter->SetOverlappingWeapon(this);

	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	ABlasterChar* BlasterCharacter=Cast<ABlasterChar>(OtherActor);
	if(BlasterCharacter){

		BlasterCharacter->SetOverlappingWeapon(nullptr);
		
		
	}
}

void AWeapon::ShowPickupWidget(bool bShowWidget){
	if(PickupWidget)PickupWidget->SetVisibility(bShowWidget);

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon,WeaponState);

}


void AWeapon::OnRep_WeaponState(){
	switch(WeaponState){
		case EWeaponState::EWS_Equipped:
			ShowPickupWidget(0);
			//AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}


void AWeapon::SetWeaponState(EWeaponState State){
	WeaponState=State;

	switch (WeaponState)
	{
		case EWeaponState::EWS_Equipped:
				ShowPickupWidget(0);
				AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	
	
	}

	
	
}











//

















//AWeapon
