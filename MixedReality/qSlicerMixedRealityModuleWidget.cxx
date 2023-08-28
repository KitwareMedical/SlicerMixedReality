/*==============================================================================

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// Slicer includes
#include "qSlicerMixedRealityModuleWidget.h"
#include "ui_qSlicerMixedRealityModuleWidget.h"

// MixedReality includes
#include "vtkSlicerMixedRealityLogic.h"
#include "vtkMRMLMixedRealityViewNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerMixedRealityModuleWidgetPrivate: public Ui_qSlicerMixedRealityModuleWidget
{
public:
  qSlicerMixedRealityModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerMixedRealityModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityModuleWidgetPrivate::qSlicerMixedRealityModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerMixedRealityModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityModuleWidget::qSlicerMixedRealityModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerMixedRealityModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerMixedRealityModuleWidget::~qSlicerMixedRealityModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModuleWidget::setup()
{
  Q_D(qSlicerMixedRealityModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  connect(d->ConnectCheckBox, SIGNAL(toggled(bool)), this, SLOT(setMixedRealityConnected(bool)));
  connect(d->RenderingEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(setMixedRealityActive(bool)));

  this->updateWidgetFromMRML();

  // If virtual reality logic is modified it indicates that the view node may changed
  qvtkConnect(logic(), vtkCommand::ModifiedEvent, this, SLOT(updateWidgetFromMRML()));
}

//--------------------------------------------------------------------------
void qSlicerMixedRealityModuleWidget::updateWidgetFromMRML()
{
  Q_D(qSlicerMixedRealityModuleWidget);

  vtkSlicerMixedRealityLogic* xrLogic = vtkSlicerMixedRealityLogic::SafeDownCast(this->logic());
  vtkMRMLMixedRealityViewNode* xrViewNode = xrLogic->GetMixedRealityViewNode();

  bool wasBlocked = d->ConnectCheckBox->blockSignals(true);
  d->ConnectCheckBox->setChecked(xrViewNode != nullptr && xrViewNode->GetVisibility());
  d->ConnectCheckBox->blockSignals(wasBlocked);

  wasBlocked = d->PlayerIPAddressLineEdit->blockSignals(true);
  if (xrViewNode != nullptr)
  {
    d->PlayerIPAddressLineEdit->setText(QString::fromStdString(xrViewNode->GetPlayerIPAddress()));
  }
  d->PlayerIPAddressLineEdit->blockSignals(wasBlocked);

  QString errorText;
  if (xrViewNode && xrViewNode->HasError())
  {
    errorText = xrViewNode->GetError().c_str();
  }
  d->ConnectionStatusLabel->setText(errorText);

  wasBlocked = d->RenderingEnabledCheckBox->blockSignals(true);
  d->RenderingEnabledCheckBox->setChecked(xrViewNode != nullptr && xrViewNode->GetActive());
  d->RenderingEnabledCheckBox->blockSignals(wasBlocked);
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModuleWidget::setMixedRealityConnected(bool connect)
{
  Q_D(qSlicerMixedRealityModuleWidget);
  vtkSlicerMixedRealityLogic* xrLogic = vtkSlicerMixedRealityLogic::SafeDownCast(this->logic());
  xrLogic->SetMixedRealityConnected(connect, d->PlayerIPAddressLineEdit->text().toStdString());
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModuleWidget::setMixedRealityActive(bool activate)
{
  vtkSlicerMixedRealityLogic* xrLogic = vtkSlicerMixedRealityLogic::SafeDownCast(this->logic());
  xrLogic->SetMixedRealityActive(activate);
}
