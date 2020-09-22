option(BUILD_ExamplePythonInvokerCmdApp "Build command-line app for ExamplePythonInvoker" ON)

if(BUILD_ExamplePythonInvokerCmdApp)
  mitkFunctionCreateCommandLineApp(
    NAME ExamplePythonInvokerCmdApp
    CPP_FILES ExamplePythonInvokerCmdApp.cpp
    PACKAGE_DEPENDS Qt5|Core
    DEPENDS MitkExamplePythonInvoker
  )
endif()
