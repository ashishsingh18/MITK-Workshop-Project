mitk_create_module(
  INCLUDE_DIRS PUBLIC include
  PACKAGE_DEPENDS ITK VTK Qt5|Core+Widgets # External package dependencies added here
  DEPENDS PUBLIC MitkCore # Module dependencies - be sure to add as needed
)

# You can add subdirectories as needed 
#add_subdirectory(cmdapps)

# You can also add additional commands here as needed.
# An example case involving deploying model files is given below.
# (This isn't related to this module, but is useful for ML modules)

# ---- Model deploying ----
# standard cmake way of copying files -- doesn't copy into the packaged install, but into the build directory
# file(COPY resources/models/survival_model DESTINATION ${CMAKE_BINARY_DIR}/bin/models)
#
# the below macro installs the models at package-time
# MITK_INSTALL( DIRECTORY resources/models)
# -------------------------