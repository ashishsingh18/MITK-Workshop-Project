### Add upenn.cbica.* to the list of allowed naming schemes
list(APPEND MITK_PLUGIN_REGEX_LIST "^upenn_cbica_[a-zA-Z0-9_]+$")

# The order matters! Dependencies should go first

set(MITK_PLUGINS
  upenn.cbica.captk.example1:ON
  upenn.cbica.captk.example2:ON
  upenn.cbica.captk.example3:ON
  upenn.cbica.captk.examplepy:ON
  upenn.cbica.captk.example5:ON
)
