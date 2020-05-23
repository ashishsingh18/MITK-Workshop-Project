/** \file ExamplePythonInvokerCmdApp.cpp
* \brief CLI program for invoking an internal python script (the user can supply args/params through this interface).
* 
*/

#include <mitkCommandLineParser.h>
#include <mitkLogMacros.h>


#include <QString>
#include <QStringList>
#include <QCoreApplication>
#include <string>
#include <map>

#include "ExamplePythonInvoker.h"


int main(int argc, char* argv[])
{
    QCoreApplication cmdapp(argc, argv); // needed for QCoreApplication::applicationDirPath() to work anywhere
    // consts placed here for now

    mitkCommandLineParser parser;

    /**** Set general information about the command line app ****/

    parser.setCategory("CaPTk Cmd App Category");
    parser.setTitle("Example Python Invoker Cmd App");
    parser.setContributor("CBICA");
    parser.setDescription(
                "This command line app runs the \"sample_script.py\" python script.");

    parser.setArgumentPrefix("--", "-");

    /**** Add arguments. Unless specified otherwise, each argument is optional.
            See mitkCommandLineParser::addArgument() for more information. ****/
    // No arguments for right now -- this is TODO. 
    auto parsedArgs = parser.parseArguments(argc, argv);
    
    // Optionally, we can collect args into a vector here and pass to python:
    // pythonInvoker.PassArgsToPython(std::vector<std::string> args).
    // This would let us implement a separate usage menu for example.

    captk::ExamplePythonInvoker pythonInvoker;
    pythonInvoker.RunSampleScript();

	return 0;

}
