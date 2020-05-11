#include "QmitkCaPTkExample2View.h"

#include <QMessageBox>


const std::string QmitkCaPTkExample2View::VIEW_ID =
    "upenn.cbica.captk.views.example2";


QmitkCaPTkExample2View::QmitkCaPTkExample2View()
{
  // ---- General setup operations ----
}

QmitkCaPTkExample2View::~QmitkCaPTkExample2View()
{

}

void QmitkCaPTkExample2View::CreateQtPartControl(QWidget *parent)
{
  // ---- Setup the basic GUI of this view ----
  m_Parent = parent;
  m_Controls.setupUi(parent);

  connect(m_Controls.pushButton_DoStuff, SIGNAL(clicked()), this, SLOT(OnDoStuffButtonClicked()));
  connect(m_Controls.pushButton_CloneImage, SIGNAL(clicked()), this, SLOT(OnCloneImageButtonClicked()));
}

void QmitkCaPTkExample2View::Activated()
{
  // Not yet implemented
}

void QmitkCaPTkExample2View::Deactivated()
{
  // Not yet implemented
}

void QmitkCaPTkExample2View::Visible()
{
  // Not yet implemented
}

void QmitkCaPTkExample2View::Hidden()
{
  // Not yet implemented
}

/************************************************************************/
/* protected slots                                                      */
/************************************************************************/

void QmitkCaPTkExample2View::OnDoStuffButtonClicked()
{
    QMessageBox msgError;
    msgError.setText("Well, now I'm not doing it.");
    msgError.setIcon(QMessageBox::Critical);
    msgError.setWindowTitle("No!");
    msgError.exec();
}

void QmitkCaPTkExample2View::OnCloneImageButtonClicked()
{
	//get datastorage( we use it further down )
	auto ds = this->GetDataStorage();

	//get selected nodes
	QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();

	//we don't handle the case where data is not loaded or more than 1 nodes are selected
	if (nodes.empty() || nodes.size() > 1)
	{
		QMessageBox msgError;
		msgError.setText("Please load and select a dataset.");
		msgError.setIcon(QMessageBox::Critical);
		msgError.setWindowTitle("selection error");
		msgError.exec();
	}
	else
	{
		//get first node from list
		mitk::DataNode *node = nodes.front();

		//clone the selected node
		mitk::DataNode::Pointer clonedNode = node->Clone();

		//set some new property to cloned node to distinguish it from existing node
		clonedNode->SetProperty("name", mitk::StringProperty::New("child"));

		//get data from selected node
		mitk::BaseData* data = node->GetData();

		//set it to cloned node
		clonedNode->SetData(data);

		//add cloned node to ds
		ds->Add(clonedNode, node);
	}
}

/************************************************************************/
/* protected                                                            */
/************************************************************************/

void QmitkCaPTkExample2View::OnSelectionChanged(berry::IWorkbenchPart::Pointer, const QList<mitk::DataNode::Pointer>& /*nodes*/)
{
	
}

void QmitkCaPTkExample2View::OnPreferencesChanged(const berry::IBerryPreferences* /*prefs*/)
{

}

void QmitkCaPTkExample2View::NodeAdded(const mitk::DataNode* /*node*/)
{
	std::cout << " Node added " << std::endl;
}

void QmitkCaPTkExample2View::NodeRemoved(const mitk::DataNode* /*node*/)
{

}

void QmitkCaPTkExample2View::SetFocus()
{

}

void QmitkCaPTkExample2View::UpdateControls()
{
  // Here you can hide views that are not useful and more
}

void QmitkCaPTkExample2View::InitializeListeners()
{

}