# This file is a simple example of a Python script that accepts input from CaPTk and can pass it back to CaPTk.
# Stay tuned to the CaPTk2 developer wiki for more details as they arise.


# The C++ code that calls this script copies an image selected on the MITK-based GUI to the 
# variable of CAPTK_IN_IMAGE before this script is even run.
# After this script is run, the C++ code copies back CAPTK_OUT_IMAGE expecting a SimpleITK image.
# By default, this script produces an inverted (by pixel value) copy of the original image.

# This script is linked to the ExamplePy plugin on the GUI. By changing this script,
# you will alter what happens when you click the "Process in Python" button.
# 

import SimpleITK as sitk
import sys
import os

#### Custom logic can go here.
# As long as you set CAPTK_OUT_IMAGE equal to a simpleITK image, it will be picked up on the MITK front-end.

inversionFilter = sitk.InvertIntensityImageFilter()
# Set CAPTK_OUT_IMAGE to a SimpleITK image and CaPTk will copy it back onto the C++ side.
CAPTK_OUT_IMAGE = inversionFilter.Execute(CAPTK_IN_IMAGE)