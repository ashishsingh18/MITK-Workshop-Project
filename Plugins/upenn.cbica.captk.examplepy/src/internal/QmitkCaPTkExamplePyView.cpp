#include "QmitkCaPTkExamplePyView.h"

#include <QMessageBox>
#include <ExamplePythonInvoker.h>


const std::string QmitkCaPTkExamplePyView::VIEW_ID =
    "upenn.cbica.captk.views.examplePy";


QmitkCaPTkExamplePyView::QmitkCaPTkExamplePyView()
{
  // ---- General setup operations ----
}

QmitkCaPTkExamplePyView::~QmitkCaPTkExamplePyView()
{

}

void QmitkCaPTkExamplePyView::CreateQtPartControl(QWidget *parent)
{
  // ---- Setup the basic GUI of this view ----
  m_Parent = parent;
  m_Controls.setupUi(parent);

  connect(m_Controls.pushButton_DoStuff, SIGNAL(clicked()), this, SLOT(OnDoStuffButtonClicked()));
  connect(m_Controls.pushButton_CloneImage, SIGNAL(clicked()), this, SLOT(OnCloneImageButtonClicked()));
  connect(m_Controls.pushButton_InvertImage, SIGNAL(clicked()), this, SLOT(OnInvertImageButtonClicked()));
  connect(m_Controls.pushButton_pythonscript, SIGNAL(clicked()), this, SLOT(OnPythonButtonClicked()));
  connect(m_Controls.pushButton_processInPython, SIGNAL(clicked()), this, SLOT(OnProcessImageInPythonButtonClicked()));
}

void QmitkCaPTkExamplePyView::Activated()
{
  // Not yet implemented
}

void QmitkCaPTkExamplePyView::Deactivated()
{
  // Not yet implemented
}

void QmitkCaPTkExamplePyView::Visible()
{
  // Not yet implemented
}

void QmitkCaPTkExamplePyView::Hidden()
{
  // Not yet implemented
}

/************************************************************************/
/* protected slots                                                      */
/************************************************************************/

void QmitkCaPTkExamplePyView::OnDoStuffButtonClicked()
{
    QMessageBox msgError;
    msgError.setText("Well, now I'm not doing it.");
    msgError.setIcon(QMessageBox::Critical);
    msgError.setWindowTitle("No!");
    msgError.exec();
}

void QmitkCaPTkExamplePyView::OnCloneImageButtonClicked()
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


void QmitkCaPTkExamplePyView::OnInvertImageButtonClicked()
{
 
}

void QmitkCaPTkExamplePyView::OnPythonButtonClicked()
{
	captk::ExamplePythonInvoker pythonInvoker;
	pythonInvoker.RunSampleScript();
}

void QmitkCaPTkExamplePyView::OnProcessImageInPythonButtonClicked()
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
		mitk::DataNode* node = nodes.front();

		auto data = node->GetData();
		// node has data?
		if (data != nullptr)
		{
			// get smart pointer from data
			mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data);
			// ... has IMAGE data? :D
			if (image.IsNotNull())
			{
				auto selectedImageName = node->GetName();
				MITK_INFO << "Processing image in python: \"" + selectedImageName + "\" ...";

				captk::ExamplePythonInvoker pythonInvoker;

				mitk::Image::Pointer processedImage = pythonInvoker.ProcessImageInPython(image);

				// Double check to make sure we aren't adding uninitalized or null images. 
				if (processedImage.IsNull() || !processedImage->IsInitialized())
					// Could do more diagnostics or raise an error message here...
					return;

				MITK_INFO << "  done";

				auto processedImageDataNode = mitk::DataNode::New(); // Create a new node
				MITK_INFO << "Adding to a data node";
				processedImageDataNode->SetData(processedImage); // assign the inverted image to the node

				MITK_INFO << "Adding a name";
				// Add a suffix so users can easily see what it is
				QString name = QString("%1_processed-in-python").arg(selectedImageName.c_str());
				processedImageDataNode->SetName(name.toStdString());

				// Finally, add the new node to the data storage.
				ds->Add(processedImageDataNode);
			}
		}
	}
}

/************************************************************************/
/* protected                                                            */
/************************************************************************/

void QmitkCaPTkExamplePyView::OnSelectionChanged(berry::IWorkbenchPart::Pointer, const QList<mitk::DataNode::Pointer>& /*nodes*/)
{
	
}

void QmitkCaPTkExamplePyView::OnPreferencesChanged(const berry::IBerryPreferences* /*prefs*/)
{

}

void QmitkCaPTkExamplePyView::NodeAdded(const mitk::DataNode* /*node*/)
{
	std::cout << " Node added " << std::endl;
}

void QmitkCaPTkExamplePyView::NodeRemoved(const mitk::DataNode* /*node*/)
{

}

void QmitkCaPTkExamplePyView::SetFocus()
{

}

void QmitkCaPTkExamplePyView::UpdateControls()
{
  // Here you can hide views that are not useful and more
}

void QmitkCaPTkExamplePyView::InitializeListeners()
{

}