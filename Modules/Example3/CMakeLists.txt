mitk_create_module(
  INCLUDE_DIRS PUBLIC include
  PACKAGE_DEPENDS ITK VTK Qt5|Core+Widgets # External package dependencies added here
  DEPENDS PUBLIC MitkCore # Module dependencies - be sure to add as needed
)

# You can also add additional commands here as needed.
