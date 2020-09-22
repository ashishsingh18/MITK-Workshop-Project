#ifndef QmitkCaPTkExamplePyView_h
#define QmitkCaPTkExamplePyView_h

#include <QmitkAbstractView.h>
#include <mitkILifecycleAwarePart.h>

#include "ui_QmitkCaPTkExamplePyControls.h"

class QmitkCaPTkExamplePyView : public QmitkAbstractView, 
                              public mitk::ILifecycleAwarePart
{
  Q_OBJECT

public:
  static const std::string VIEW_ID;

  QmitkCaPTkExamplePyView();
  virtual ~QmitkCaPTkExamplePyView();

  // GUI setup
  void CreateQtPartControl(QWidget *parent);

  // ILifecycleAwarePart interface
  void Activated();
  void Deactivated();
  void Visible();
  void Hidden();

protected slots:

  /** \brief "Do Stuff" Button clicked slot */
  void OnDoStuffButtonClicked();

  /** \brief "Clone Image" Button clicked slot */
  void OnCloneImageButtonClicked();
  
  /** \brief "Invert Image" Button clicked slot */
  void OnInvertImageButtonClicked();

  /** \brief "Run Python Sample Script" Button clicked slot */
  void OnPythonButtonClicked();

  /** \brief "Invert In Python" Button clicked slot */
  void OnProcessImageInPythonButtonClicked();

protected:

  // reimplemented from QmitkAbstractView
  void OnSelectionChanged(berry::IWorkbenchPart::Pointer part, const QList<mitk::DataNode::Pointer> &nodes) override;

  // reimplemented from QmitkAbstractView
  void OnPreferencesChanged(const berry::IBerryPreferences *prefs) override;

  // reimplemented from QmitkAbstractView
  void NodeAdded(const mitk::DataNode *node) override;

  // reimplemented from QmitkAbstractView
  void NodeRemoved(const mitk::DataNode *node) override;

  void SetFocus();

  void UpdateControls();

  void InitializeListeners();

  /// \brief the Qt parent of our GUI (NOT of this object)
  QWidget *m_Parent;

  /// \brief Qt GUI file
  Ui::QmitkCaPTkExamplePyControls m_Controls;
  
};

#endif // ! QmitkCaPTkExamplePyView_h