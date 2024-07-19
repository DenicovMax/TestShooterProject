// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SPDeathWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void USPDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ExitButton->OnPressed.AddDynamic(this, &USPDeathWidget::OnExitButtonDown);
}

void USPDeathWidget::OnExitButtonDown()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
