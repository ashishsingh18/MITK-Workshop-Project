#include "ExamplePythonInvoker.h"

#include "mitkLogMacros.h"

#include <mitkIPythonService.h>
#include <usModuleContext.h>
#include <usGetModuleContext.h>
#include <QString>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

namespace captk {

// Default static variables
bool ExamplePythonInvoker::pythonFilesDirFound = false;
std::string ExamplePythonInvoker::pythonFilesDirPath = "";

// Functions to ensure all python files are available
ExamplePythonInvoker::ExamplePythonInvoker()
{
	// Set up python service
    // We need to get access to the Python service through the microservices interface.
    // This is basically boilerplate. (TBD: figure out what preserves or refreshes the python context)
    // Note that each instance of this object keeps state of a different python instance.
    us::ModuleContext* context = us::GetModuleContext();
    m_PythonServiceRef = context->GetServiceReference<mitk::IPythonService>();
    m_PythonService = dynamic_cast<mitk::IPythonService*> (context->GetService<mitk::IPythonService>(m_PythonServiceRef));
    mitk::IPythonService::ForceLoadModule();
    
    // Find our python files if we haven't already and allow python to access them
    if (pythonFilesDirFound)
    {
        RegisterResourceDir(pythonFilesDirPath);
    }
    else 
    {
        RegisterResourceDir(LocatePythonFileDir());
    }

    // Set cwd in python as if it was run natively.
    ChangeWorkingDirectory(pythonFilesDirPath);
}

void ExamplePythonInvoker::RegisterResourceDir(std::string resourceDirPath)
{
    // construct a vector to feed to Python's absolute search dirs
    std::vector< std::string > absolute_search_dirs = { resourceDirPath };

    // Python will now also look in these dirs for modules/packages.
    // this function only takes a vector of std::strings.
    m_PythonService->AddAbsoluteSearchDirs(absolute_search_dirs);
}

std::string ExamplePythonInvoker::LocatePythonFileDir()
{
    // Build tree and install tree structure are different...
    // So we search in both the executable dir and one directory above.
    // We use the module name to help prevent resource mixups/conflicts.

    auto execDir = QCoreApplication::applicationDirPath();
    MITK_DEBUG << (QString("DEBUG: applicationDirPath == ") + execDir).toStdString();

    QString moduleResourceDir("/MitkExamplePythonInvoker/resources");
    QString installResourceDirPath(execDir + moduleResourceDir);
    QString buildResourceDirPath(execDir + "/.." + moduleResourceDir);

    // Find path to our python files
    if (QDir(installResourceDirPath).exists())
    {
        pythonFilesDirPath = installResourceDirPath.toStdString();
        pythonFilesDirFound = true;
    }
    else if (QDir(buildResourceDirPath).exists())
    {
        pythonFilesDirPath = buildResourceDirPath.toStdString();
        pythonFilesDirFound = true;
    }
    else // failure case
    {
        pythonFilesDirPath = "";
        pythonFilesDirFound = false;
    }

    // return the location found
    return pythonFilesDirPath;
}


// Utility functions
void ExamplePythonInvoker::PassArgsToPython(std::vector<std::string> args)
{
    /* This function allows us to spoof args by modifying sys.argv in python.
     * This allows us to basically emulate being run from the command line
     * for use with scripts that are usually run like that.
     * This means we can flexibly wrap any script with C++ parsing,
     * or just use the python script's parsing.
     * All we need to do is construct the args vector.
     */
    
    // import sys to make sure we can access this. 
    // No worries: this will not cause another sys import to fail.
    m_PythonService->Execute("import sys", mitk::IPythonService::SINGLE_LINE_COMMAND);
    
    // insert executable name
    args.insert(args.begin(), QCoreApplication::applicationFilePath().toStdString());
    // read in all args
    for (int i = 0; i < args.size(); i++)
    {
        auto numString = QString::number(i).toStdString();
        m_PythonService->Execute("sys.argv[" + numString + "] = " + args[i],
            mitk::IPythonService::SINGLE_LINE_COMMAND);
    }
    
}

bool ExamplePythonInvoker::ChangeWorkingDirectory(
    std::string newWorkingDirectoryPath = ExamplePythonInvoker::pythonFilesDirPath)
{
    // This function allows us to set the working directory in Python to whatever we want.
   
    bool isOk = true; // assume success unless an error arises
    // import os to make sure we can access this. 
    // No worries: this will not cause another os import to fail.
    m_PythonService->Execute("import os", mitk::IPythonService::SINGLE_LINE_COMMAND);
    // Mind the escaped quotes. These are necessary.
    m_PythonService->Execute("os.chdir(\"" + newWorkingDirectoryPath + "\")",
        mitk::IPythonService::SINGLE_LINE_COMMAND);
    if (m_PythonService->PythonErrorOccured())
    {
        isOk = false;
    }
    return isOk;
}

bool ExamplePythonInvoker::IsOkayToRun()
{
    bool ok = true; // Assumed ok unless below conditions fail

    // Check if python files were found
    if (!pythonFilesDirFound)
    {
        MITK_ERROR << "Required python files were not found for this module."
            "Check permissions or reinstall if this problem persists.";
        ok = false;
    }
    // Add more failure conditions here as necessary, especially if more setup is required

    // return final status
    return ok;

}


// Work functions

mitk::Image::Pointer ExamplePythonInvoker::InvertImageInPython(mitk::Image::Pointer inputImage)
{
    // This will showcase how to retrieve mitk image outputs from python scripts effectively.
    /* Note that this approach assumes certain things about the python code -- 
     * We could effectively make this an "interface" of variable names
     * that we ask algorithm writers to use.
     * This would be part of our sit-down talks with algorithm developers.
     */

    auto entryPointFilename = "invertImage.py";
    m_PythonService->CopyToPythonAsSimpleItkImage(inputImage, "in_image");

    m_PythonService->ExecuteScript(pythonFilesDirPath + "/" + entryPointFilename);

    // clean up in_image
    if (m_PythonService->DoesVariableExist("in_image"))
        m_PythonService->Execute("del in_image");

    // grab and return image
    if (m_PythonService->DoesVariableExist("out_image"))
    {
        mitk::Image::Pointer processedImage = m_PythonService->CopySimpleItkImageFromPython("out_image");
        return processedImage;
    }
    
    // If we errored out or the script didn't generate an out_image, return null
    return nullptr;
}

void ExamplePythonInvoker::RunSampleScript()
{
    // With everything else set up, the logic for running the script goes here.
    auto entryPointFilename = "sample_script.py";
    // Execute our entrypoint. Any last-second setup or checks should be done before this.
    m_PythonService->ExecuteScript(pythonFilesDirPath + "/" + entryPointFilename);

    // Now you can do post-processing. Retrieve results, call more python functions, etc...

    // Example: Get a variable from Python
    std::string resultFromPython = m_PythonService->GetVariable("result");
    MITK_INFO << "Result from Python: " + resultFromPython;

    // Example: Set a variable in Python generated in C++. 
    int someCppGeneratedValue = 42;
    QString passVarToPythonCommand = QString("var_from_cpp = ") 
        + QString::number(someCppGeneratedValue);
    // String manipulation to do this can get a little ugly... but it is doable.
    // We can define some utility functions for this sort of thing.
    m_PythonService->Execute(passVarToPythonCommand.toStdString(),
        mitk::IPythonService::SINGLE_LINE_COMMAND);
    // Maybe we could modify this to return a status string
    
    // See InvertImageInPython for an example of how to pass images back and forth.
}

} // end namespace captk

