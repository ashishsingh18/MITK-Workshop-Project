/** \file CaPTkExample4CmdApp.cpp
 * \brief CLI program for showcasing the example3
 * 
 * The example currently showcases image inversion
 */

#include <mitkCommandLineParser.h>
#include <mitkException.h>
#include <mitkLogMacros.h>
#include <mitkImage.h>
#include <mitkIOUtil.h>
#include <mitkImageAccessByItk.h>

#include "ExampleImageInverter.h"


/** \brief command-line app for example3 module
 *
 * This command-line app takes an image and inverts it.
 */
int main(int argc, char* argv[])
{
  mitkCommandLineParser parser;

  /*---- Set general information about the command-line app ----*/

  parser.setCategory("Example Cmd App Category");
  parser.setTitle("Example4 Cmd App");
  parser.setContributor("CBICA");
  parser.setDescription(
    "This command-line app showcases the use of example3 module.");

  // How should arguments be prefixed
  parser.setArgumentPrefix("--", "-");

  /*---- Add arguments. Unless specified otherwise, each argument is optional.
            See mitkCommandLineParser::addArgument() for more information. ----*/

  parser.addArgument(
    "input",
    "i",
    mitkCommandLineParser::File,
    "Input image path",
    "Path to the input image to be inverted",
    us::Any(),
    false); // false -> required parameter

  parser.addArgument(
    "output",
    "o",
    mitkCommandLineParser::File,
    "Output",
    "Output image target path",
    us::Any(),
    false); // false -> required parameter

  /*---- Parse arguments. This method returns a mapping of long argument names to
            their values. ----*/

  auto parsedArgs = parser.parseArguments(argc, argv);

  if (parsedArgs.empty())
  {
    return EXIT_FAILURE; // Just exit, usage information was already printed.
  }

  /*---- Required parameters ----*/

  if (parsedArgs["input"].Empty())
  {
    std::cerr << parser.helpText();
    return EXIT_FAILURE;
  }

  if (parsedArgs["output"].Empty())
  {
    std::cerr << parser.helpText();
    return EXIT_FAILURE;
  }

  auto inputPath  = us::any_cast<std::string>(parsedArgs["input"]);
  auto outputPath = us::any_cast<std::string>(parsedArgs["output"]);


  /*---- Run ----*/

  try
  {
    // Read input
    mitk::Image::Pointer inputimage = mitk::IOUtil::Load<mitk::Image>(inputPath);

    // Call example image inverter from module
	captk::ExampleImageInverter *inverfilter = new captk::ExampleImageInverter();
	inverfilter->SetInput(inputimage);
	inverfilter->Update();
	mitk::Image::Pointer outputimage = inverfilter->GetOutput();

    // Save result
    mitk::IOUtil::Save(outputimage, outputPath);
  }
  catch (const mitk::Exception& e)
  {
    MITK_ERROR << "MITK Exception: " << e.what();
  }
  catch (...)
  {
    std::cerr << "Unexpected error!\n";
    return EXIT_FAILURE;
  }
}
