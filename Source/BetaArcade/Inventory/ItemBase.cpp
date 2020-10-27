// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}


//Function for collision. (Destroy actor on overlap for now.)
void AItemBase::OnOverlap(AActor * MyOverlappedActor, AActor * OtherActor)
{
	Destroy();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

