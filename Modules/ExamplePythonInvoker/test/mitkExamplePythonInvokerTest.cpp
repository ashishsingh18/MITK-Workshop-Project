/* ************************
 File: mitkExamplePythonInvokerTest.cpp
 
 This file is an example of how to create test suite containing tests for a custom MITK module,
 based on the MITK testing documentation available at http://docs.mitk.org/nightly/GeneralTests.html
 
 See also: http://www.mitk.org/images/5/5d/BugSquashingSeminars%24CppUnitFrameworkSeminar.pdf
 
 This file will use the MitkExamplePythonInvoker module as an example. While the implementation of tests
 will be different for your module, the format should generally look like this.
************************** */

// Testing includes -- these are mandatory if you want to utilize the MITK/CppUnit testing framework.
#include "mitkTestFixture.h"
#include "mitkTestingMacros.h"

// Any relevant std includes
#include <string>
// MITK includes -- Whatever unit you are testing should be included here. In our case, that's the ExamplePythonInvoker module.
#include "ExamplePythonInvoker.h"

// VTK includes
#include <vtkDebugLeaks.h> // used to detect vtk objects lingering in memory at program exit

class mitkExamplePythonInvokerTestSuite : public mitk::TestFixture // Always inherit from TestFixture for this
{
  CPPUNIT_TEST_SUITE(mitkExamplePythonInvokerTestSuite); // Put any individual tests in this section
  
  // Register one of the functions defined below to this test suite. 
  MITK_TEST(Append_ExampleString_AddsExampleStringToData); // pass the function pointer itself -- don't include parameters or ()!
  
  CPPUNIT_TEST_SUITE_END();
  
private: // Class members for this test suite. 
  //mitk::ExampleDataStructure::Pointer m_Data;
  std::string m_DefaultDataString;
  std::string m_DataString;
public:
  void setUp() override // This will run before every test.
  {
	// To keep individual tests as small as possible, perform setup / refresh the state here.
    m_DefaultDataString = "This is the example data content\nAnd a second line\n";
    m_DataString = "";
    //m_Data = mitk::ExampleDataStructure::New();
    //m_Data->SetData(m_DefaultDataString);
  }
  void tearDown() override // This runs after each test.
  {
	// Clean up memory for each test.
    m_DefaultDataString = "";
    //m_Data = nullptr;
  }
  
  // The function below is an individual test. Make sure you register these up in the CPPUNIT_TEST_SUITE section.
  // Try to keep tests as small and discretized as possible. 
  // A suggested naming convention for test methods is <METHOD_TO_BE_TESTED>_<INPUT_PARAMETERS/CONDITION>_<EXPECTED_RESULT>.
  void Append_ExampleString_AddsExampleStringToData()
  {
    std::string appendedString = "And a third line\n";
    std::string wholeString = m_DefaultDataString + appendedString;
    m_DataString = m_DefaultDataString + appendedString;
	// The below line is the "test" proper. Compare the generated output to expected output, and fail if they don't match.
    CPPUNIT_ASSERT_MESSAGE("Checking whether string was correctly appended.", m_DataString == wholeString);
	// MITK provides other useful asserts too. See: http://docs.mitk.org/nightly/group__MITKTestingAPI.html
	
  }
  
  // TODO: Add PythonInvoker-specific tests.
};
MITK_TEST_SUITE_REGISTRATION(mitkExamplePythonInvoker) // Register the test suite as part of the corresponding module.