// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterAnimInstance.h"
#include "Character/BlasterChar.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBlasterAnimInstance::NativeInitializeAnimation(){
    Super::NativeInitializeAnimation();

    BlasterCharacter=Cast<ABlasterChar>(TryGetPawnOwner());

}



void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime){
    Super::NativeUpdateAnimation(DeltaTime);
    if(BlasterCharacter==nullptr){
        BlasterCharacter=Cast<ABlasterChar>(TryGetPawnOwner());

    }
    if(BlasterCharacter==nullptr)return;


    FVector Velocity=BlasterCharacter->GetVelocity();
    Velocity.Z=0.f;
    Speed=Velocity.Size();
    bIsInAir=BlasterCharacter->GetCharacterMovement()->IsFalling();
    bIsAccelerating=BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size()>0.f?1:0;
    bWeaponEquipped=BlasterCharacter->IsWeaponEquipped();
    



}









//
















//UBlasterAnimInstance