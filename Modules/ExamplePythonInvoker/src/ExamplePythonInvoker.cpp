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

// Functions to ensure all python files are available
ExamplePythonInvoker::ExamplePythonInvoker()
{
    std::string currentExecutableDir = QCoreApplication::applicationDirPath().toStdString();

    pythonFilesDirFound = false;
    // TODO: Set this dynamically, pick up other meta-info from app-specific directory
    pythonFilesDirPath = currentExecutableDir + "/MitkExamplePythonInvoker/resources/";

    // TODO: Figure out what we do for builds that aren't installed...
    // Maybe just install miniconda/python in the ${BINARY_DIR}/Python/. 
    // The above shouldn't be done on build agents though. Just developer machines.

    // TODO: Could put these from a header or a singleton class or something...
    // But for now this is fine. Note these are installation and OS dependent.
    std::string captkPythonInstallDir = ""; // Where the Python install is in the CaPTk2 tree
    std::string captkPackageRepoDir = ""; // Where we install versioned packages using pip
    std::string captkPythonInstallSitePackagesDir = ""; // site-packages of the Python install
    
#ifdef _WIN32  
    captkPythonInstallDir = currentExecutableDir + "/Python/";
    captkPackageRepoDir = captkPythonInstallDir + "/installedPackages/";
    captkPythonInstallSitePackagesDir = currentExecutableDir + "/Python/Lib/site-packages/";
#elif __linux__
    // TODO: Get these paths
#elif __APPLE__
    // TODO: Get these paths
#endif
	// Set up python service
    // We need to get access to the Python service through the microservices interface.
    // This is basically boilerplate. (TBD: figure out what preserves or refreshes the python context)
    // Note that each instance of this object keeps state of a different python instance.
    us::ModuleContext* context = us::GetModuleContext();
    m_PythonServiceRef = context->GetServiceReference<mitk::IPythonService>();
    m_PythonService = dynamic_cast<mitk::IPythonService*> (context->GetService<mitk::IPythonService>(m_PythonServiceRef));
    mitk::IPythonService::ForceLoadModule();
    
    // Remove existing entries from path
    // This ensures only dirs we explicitly allow will be found
    m_PythonService->Execute("import sys", mitk::IPythonService::SINGLE_LINE_COMMAND);
    m_PythonService->Execute("sys.path = []", mitk::IPythonService::SINGLE_LINE_COMMAND);

    // Find our python files if we haven't already and allow python to access them
    if (!pythonFilesDirFound)
    {
        LocatePythonFileDir();
    }
    
    // Add to path first just in case.
    RegisterResourceDir(pythonFilesDirPath);
    // Set cwd in python as if it was run natively.
    ChangeWorkingDirectory(pythonFilesDirPath);

    // TODO: Pick up listed (versioned) dependencies at run-time.
    // CONT: Add dirs corresponding to those dependency name-version strings to the search path dynamically.

    // Append additional dirs -- the more general, the later they should come!
    // App-specific dirs should appear as early in the path as possible.
    // This is necessary for the app-specific stuff to be picked up first.
    RegisterPackageDir(captkPackageRepoDir);

    // The base python installation packages should always come LAST. 
    // In general this directory should only have builtins.
    RegisterPackageDir(captkPythonInstallSitePackagesDir);
}

void ExamplePythonInvoker::RegisterResourceDir(std::string resourceDirPath)
{
    // construct a vector to feed to Python's absolute search dirs
    std::vector< std::string > absolute_search_dirs = { resourceDirPath };

    // Python will now also look in these dirs for modules/packages.
    // This path will be *APPENDED* to the search path.
    // this function only takes a vector of std::strings.
    m_PythonService->AddAbsoluteSearchDirs(absolute_search_dirs);
}

void ExamplePythonInvoker::RegisterPackageDir(std::string packageDirPath)
{
    // construct a vector to feed to Python's absolute search dirs
    std::vector< std::string > absolute_search_dirs = { packageDirPath };

    // Python will now also look in these dirs for modules/packages.
    // This path will be *APPENDED* to the search path.
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
     * All we need to do ahead of time is construct the args vector.
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
    std::string newWorkingDirectoryPath)
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

mitk::Image::Pointer ExamplePythonInvoker::ProcessImageInPython(mitk::Image::Pointer inputImage)
{
    // This will showcase how to retrieve mitk image outputs from python scripts effectively.
    /* Note that this approach assumes certain things about the python code -- 
     * We could effectively make this an "interface" of variable names
     * that we ask algorithm writers to use.
     * This would probably be part of our sit-down talks with algorithm developers.
     */

    auto entryPointFilename = "ProcessImage.py";
    m_PythonService->CopyToPythonAsSimpleItkImage(inputImage, "CAPTK_IN_IMAGE");

    m_PythonService->ExecuteScript(pythonFilesDirPath + "/" + entryPointFilename);

    // clean up CAPTK_IN_IMAGE after script has run
    if (m_PythonService->DoesVariableExist("CAPTK_IN_IMAGE"))
        m_PythonService->Execute("del CAPTK_IN_IMAGE");

    // grab and return image
    if (m_PythonService->DoesVariableExist("CAPTK_OUT_IMAGE"))
    {
        mitk::Image::Pointer processedImage = m_PythonService->CopySimpleItkImageFromPython("CAPTK_OUT_IMAGE");
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
    std::string resultFromPython = m_PythonService->GetVariable("CAPTK_RESULT_STRING");
    MITK_INFO << "Result from Python: " + resultFromPython;

    // Example: Set a variable in Python generated in C++. 
    // TODO: Replace this with a convenience function...
    int someCppGeneratedValue = 42;
    QString passVarToPythonCommand = QString("var_from_cpp = ") 
        + QString::number(someCppGeneratedValue);
    // String manipulation to do this can get a little ugly... but it is doable.
    // We can define some utility functions for this sort of thing.
    m_PythonService->Execute(passVarToPythonCommand.toStdString(),
        mitk::IPythonService::SINGLE_LINE_COMMAND);
    // Maybe we could modify this to return a status string
    
    // See ProcessImageInPython for an example of how to pass images back and forth.
}

} // end namespace captk

