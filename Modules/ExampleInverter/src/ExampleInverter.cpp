#include "ExampleInverter.h"

#include <mitkImageToItk.h>
#include <mitkITKImageImport.h>
#include "mitkLogMacros.h"

namespace captk {

ExampleInverter::ExampleInverter()
{
	// any setup required goes here
}

void ExampleInverter::SetInput(mitk::Image::Pointer inImagePtr)
{
	this->m_InputPtr = inImagePtr;
}

void ExampleInverter::Update()
{
	// Read as 3D double image (hardcoded) for now (anything else will crash).
	// Later we can use predicates and/or AccessByItk to help with this.
	using TPixel = typename short;
	const unsigned int VImageDimension = 3;
	typedef itk::Image<TPixel, VImageDimension> itkImageType;
	typedef itk::InvertIntensityImageFilter<itkImageType, itkImageType> FilterType;

	// There are a few ways to convert from MITK to ITK...
	auto itkInImage = mitk::ImageToItkImage<TPixel, VImageDimension>(this->m_InputPtr.GetPointer());

	auto filter = FilterType::New();
	filter->SetInput(itkInImage);
	filter->Update();
	auto itkOutImage = filter->GetOutput();

	// There are also a few ways to convert from ITK to MITK.
	auto mitkOutImage = mitk::GrabItkImageMemory(itkOutImage); 
	this->m_LastOutput = mitkOutImage.GetPointer();

}

mitk::Image::Pointer ExampleInverter::GetOutput()
{
	return this->m_LastOutput;
}

} // end namespace captk

