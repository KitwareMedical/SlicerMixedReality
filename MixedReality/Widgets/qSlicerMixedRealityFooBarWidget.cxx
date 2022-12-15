/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerMixedRealityFooBarWidget.h"
#include "ui_qSlicerMixedRealityFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_MixedReality
class qSlicerMixedRealityFooBarWidgetPrivate
  : public Ui_qSlicerMixedRealityFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerMixedRealityFooBarWidget);
protected:
  qSlicerMixedRealityFooBarWidget* const q_ptr;

public:
  qSlicerMixedRealityFooBarWidgetPrivate(
    qSlicerMixedRealityFooBarWidget& object);
  virtual void setupUi(qSlicerMixedRealityFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerMixedRealityFooBarWidgetPrivate
::qSlicerMixedRealityFooBarWidgetPrivate(
  qSlicerMixedRealityFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerMixedRealityFooBarWidgetPrivate
::setupUi(qSlicerMixedRealityFooBarWidget* widget)
{
  this->Ui_qSlicerMixedRealityFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerMixedRealityFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityFooBarWidget
::qSlicerMixedRealityFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerMixedRealityFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerMixedRealityFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerMixedRealityFooBarWidget
::~qSlicerMixedRealityFooBarWidget()
{
}
