#include "QmitkCaPTkExample4View.h"

#include <QMessageBox>
#include <QFile>

#include <ExampleImageInverter.h>

#include <usGetModuleContext.h>
#include <mitkIPythonService.h>
#include <usModuleContext.h>

const std::string QmitkCaPTkExample4View::VIEW_ID =
    "upenn.cbica.captk.views.example4";


QmitkCaPTkExample4View::QmitkCaPTkExample4View()
{
  // ---- General setup operations ----
}

QmitkCaPTkExample4View::~QmitkCaPTkExample4View()
{

}

void QmitkCaPTkExample4View::CreateQtPartControl(QWidget *parent)
{
  // ---- Setup the basic GUI of this view ----
  m_Parent = parent;
  m_Controls.setupUi(parent);

  connect(m_Controls.pushButton_DoStuff, SIGNAL(clicked()), this, SLOT(OnDoStuffButtonClicked()));
  connect(m_Controls.pushButton_CloneImage, SIGNAL(clicked()), this, SLOT(OnCloneImageButtonClicked()));
  connect(m_Controls.pushButton_InvertImage, SIGNAL(clicked()), this, SLOT(OnInvertImageButtonClicked()));
  connect(m_Controls.pushButton_InvertImagePython, SIGNAL(clicked()), this, SLOT(OnInvertImagePythonButtonClicked()));

}

void QmitkCaPTkExample4View::Activated()
{
  // Not yet implemented
}

void QmitkCaPTkExample4View::Deactivated()
{
  // Not yet implemented
}

void QmitkCaPTkExample4View::Visible()
{
  // Not yet implemented
}

void QmitkCaPTkExample4View::Hidden()
{
  // Not yet implemented
}

/************************************************************************/
/* protected slots                                                      */
/************************************************************************/

void QmitkCaPTkExample4View::OnDoStuffButtonClicked()
{
    QMessageBox msgError;
    msgError.setText("Well, now I'm not doing it.");
    msgError.setIcon(QMessageBox::Critical);
    msgError.setWindowTitle("No!");
    msgError.exec();
}

void QmitkCaPTkExample4View::OnCloneImageButtonClicked()
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


void QmitkCaPTkExample4View::OnInvertImageButtonClicked()
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

		auto data = node->GetData();
		// node has data?
		if (data != nullptr) 
		{
			// get smart pointer from data
			mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data);
			// ... has IMAGE data? :D
			if (image.IsNotNull())
			{
				auto imageName = node->GetName();
				MITK_INFO << "Processing image \"" << imageName << "\" ...";

				// get our inverter filter class (note this isn't a proper ITK-style smart pointer --
				// change this in your code if you are using a proper filter.
				auto filter = captk::ExampleImageInverter(); 
				auto filterPtr = &filter;

				filterPtr->SetInput(image);
				filterPtr->Update();
				mitk::Image::Pointer processedImage = filterPtr->GetOutput();

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
				QString name = QString("%1_inverted").arg(imageName.c_str());
				processedImageDataNode->SetName(name.toStdString());

				// Finally, add the new node to the data storage.
				ds->Add(processedImageDataNode);
			}
		}
	}
}

void QmitkCaPTkExample4View::OnInvertImagePythonButtonClicked()
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

		auto data = node->GetData();
		// node has data?
		if (data != nullptr)
		{
			// get smart pointer from data
			mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data);
			// ... has IMAGE data? :D
			if (image.IsNotNull())
			{
				//load python service
				us::ModuleContext* context = us::GetModuleContext();
				us::ServiceReference<mitk::IPythonService> m_PythonServiceRef = context->GetServiceReference<mitk::IPythonService>();
				mitk::IPythonService* m_PythonService = dynamic_cast<mitk::IPythonService*> (context->GetService<mitk::IPythonService>(m_PythonServiceRef));
				mitk::IPythonService::ForceLoadModule();

				auto imageName = node->GetName();
				MITK_INFO << "Processing image \"" << imageName << "\" ...";

				// transfer input image to python
				m_PythonService->CopyToPythonAsSimpleItkImage(image, "in_image");

				QString data;
				QString pythonScriptName(":/QExample4/invertImage.py");
				QFile file(pythonScriptName);
				if (!file.open(QIODevice::ReadOnly))
				{
					qDebug() << "filenot opened" << endl;
				}
				else
				{
					qDebug() << "file opened" << endl;
					data = file.readAll();
				}
				file.close();

				//call python script
				m_PythonService->Execute(data.toStdString(), mitk::IPythonService::MULTI_LINE_COMMAND);

				// clean up after running script (better way than deleting individual variables?)
				if (m_PythonService->DoesVariableExist("in_image"))
					m_PythonService->Execute("del in_image");

				if (m_PythonService->DoesVariableExist("out_image"))
				{
					//get image back from python
					mitk::Image::Pointer processedImage = m_PythonService->CopySimpleItkImageFromPython("out_image");

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
					QString name = QString("%1_inverted").arg(imageName.c_str());
					processedImageDataNode->SetName(name.toStdString());

					// Finally, add the new node to the data storage.
					ds->Add(processedImageDataNode);
				}
			}
		}
	}
}

/************************************************************************/
/* protected                                                            */
/************************************************************************/

void QmitkCaPTkExample4View::OnSelectionChanged(berry::IWorkbenchPart::Pointer, const QList<mitk::DataNode::Pointer>& /*nodes*/)
{
	
}

void QmitkCaPTkExample4View::OnPreferencesChanged(const berry::IBerryPreferences* /*prefs*/)
{

}

void QmitkCaPTkExample4View::NodeAdded(const mitk::DataNode* /*node*/)
{
	std::cout << " Node added " << std::endl;
}

void QmitkCaPTkExample4View::NodeRemoved(const mitk::DataNode* /*node*/)
{

}

void QmitkCaPTkExample4View::SetFocus()
{

}

void QmitkCaPTkExample4View::UpdateControls()
{
  // Here you can hide views that are not useful and more
}

void QmitkCaPTkExample4View::InitializeListeners()
{

}