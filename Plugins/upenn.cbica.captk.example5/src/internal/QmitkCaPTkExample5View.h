#ifndef QmitkCaPTkExample5View_h
#define QmitkCaPTkExample5View_h

#include <QmitkAbstractView.h>
#include <mitkILifecycleAwarePart.h>

#include "ui_QmitkCaPTkExample5Controls.h"

class QmitkCaPTkExample5View : public QmitkAbstractView, 
                              public mitk::ILifecycleAwarePart
{
  Q_OBJECT

public:
  static const std::string VIEW_ID;

  QmitkCaPTkExample5View();
  virtual ~QmitkCaPTkExample5View();

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

  /** \brief "Invert Image using python" Button clicked slot */
  void OnInvertImageModulePythonButtonClicked();

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
  Ui::QmitkCaPTkExample5Controls m_Controls;
  
};

#endif // ! QmitkCaPTkExample5View_h