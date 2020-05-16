option(BUILD_CaPTkExample3CmdApp "Build command-line app for CaPTkExample3" ON)

if(BUILD_CaPTkExample3CmdApp)
  mitkFunctionCreateCommandLineApp(
    NAME CaPTkExample3CmdApp
    CPP_FILES CaPTkExample3CmdApp.cpp
    PACKAGE_DEPENDS ITK Qt5|Core
    DEPENDS MitkExampleImageInverter
  )
endif()
