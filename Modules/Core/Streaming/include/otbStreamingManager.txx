/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbStreamingManager_txx
#define otbStreamingManager_txx

#include "otbStreamingManager.h"
#include "otbConfigurationManager.h"
#include "itkExtractImageFilter.h"

namespace otb
{

template <class TImage>
StreamingManager<TImage>::StreamingManager()
  : m_ComputedNumberOfSplits(0)
{
}

template <class TImage>
StreamingManager<TImage>::~StreamingManager()
{
}

template <class TImage>
typename StreamingManager<TImage>::MemoryPrintType
StreamingManager<TImage>::GetActualAvailableRAMInBytes(MemoryPrintType availableRAMInMB)
{
  MemoryPrintType availableRAMInBytes = availableRAMInMB * 1024 * 1024;

  if (availableRAMInBytes == 0)
    {
    otbMsgDevMacro(<< "Retrieving available RAM size from configuration");
    // Retrieve it from the configuration
    availableRAMInBytes = 1024*1024*ConfigurationManager::GetMaxRAMHint();
    }

  otbMsgDevMacro("RAM used to estimate memory footprint : " << availableRAMInBytes / 1024 / 1024  << " MB")
  return availableRAMInBytes;
}

template <class TImage>
unsigned int
StreamingManager<TImage>::EstimateOptimalNumberOfDivisions(itk::DataObject * input, const RegionType &region,
                                                           MemoryPrintType availableRAM,
                                                           double bias)
{
  otbMsgDevMacro(<< "availableRAM " << availableRAM)

  MemoryPrintType availableRAMInBytes = GetActualAvailableRAMInBytes(availableRAM);

  otb::PipelineMemoryPrintCalculator::Pointer memoryPrintCalculator;
  memoryPrintCalculator = otb::PipelineMemoryPrintCalculator::New();

  // Trick to avoid having the resampler compute the whole
  // displacement field
  double     regionTrickFactor = 1;
  ImageType* inputImage = dynamic_cast<ImageType*>(input);

  MemoryPrintType pipelineMemoryPrint;
  if (inputImage)
    {

    typedef itk::ExtractImageFilter<ImageType, ImageType> ExtractFilterType;
    typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
    extractFilter->SetInput(inputImage);

    // Define a small region to run the memory footprint estimation,
    // around the image center, 100 pixels wide in each dimension
    SizeType smallSize;
    smallSize.Fill(100);
    IndexType index;
    index[0] = region.GetIndex()[0] + region.GetSize()[0]/2 - 50;
    index[1] = region.GetIndex()[1] + region.GetSize()[1]/2 - 50;

    RegionType smallRegion;
    smallRegion.SetSize(smallSize);
    smallRegion.SetIndex(index);

    // In case the image is smaller than 100 pixels in a direction
    smallRegion.Crop(region);

    extractFilter->SetExtractionRegion(smallRegion);

    bool smallRegionSuccess = smallRegion.Crop(region);

    if (smallRegionSuccess)
      {
      otbMsgDevMacro("Using an extract to estimate memory : " << smallRegion)
      // the region is well behaved, inside the largest possible region
      memoryPrintCalculator->SetDataToWrite(extractFilter->GetOutput() );

      regionTrickFactor = static_cast<double>( region.GetNumberOfPixels() )
        / static_cast<double>(smallRegion.GetNumberOfPixels() );

      memoryPrintCalculator->SetBiasCorrectionFactor(regionTrickFactor * bias);
      }
    else
      {
      otbMsgDevMacro("Using the input region to estimate memory : " << region)
      // the region is not well behaved
      // use the full region
      memoryPrintCalculator->SetDataToWrite(input);
      memoryPrintCalculator->SetBiasCorrectionFactor(bias);
      }

    memoryPrintCalculator->Compute();

    pipelineMemoryPrint = memoryPrintCalculator->GetMemoryPrint();

    if (smallRegionSuccess)
      {
      // remove the contribution of the ExtractImageFilter
      MemoryPrintType extractContrib =
          memoryPrintCalculator->EvaluateDataObjectPrint(extractFilter->GetOutput());

      pipelineMemoryPrint -= extractContrib;
      }
    }
  else
    {
    // Use the original object to estimate memory footprint
    memoryPrintCalculator->SetDataToWrite(input);
    memoryPrintCalculator->SetBiasCorrectionFactor(1.0);

    memoryPrintCalculator->Compute();

    pipelineMemoryPrint = memoryPrintCalculator->GetMemoryPrint();
    }

  unsigned int optimalNumberOfDivisions =
      otb::PipelineMemoryPrintCalculator::EstimateOptimalNumberOfStreamDivisions(pipelineMemoryPrint, availableRAMInBytes);

  otbMsgDevMacro( "Estimated Memory print for the full image : "
                  << static_cast<unsigned int>(pipelineMemoryPrint * otb::PipelineMemoryPrintCalculator::ByteToMegabyte ) << std::endl)
  otbMsgDevMacro( "Optimal number of stream divisions: "
                  << optimalNumberOfDivisions << std::endl)

  return optimalNumberOfDivisions;
}

template <class TImage>
unsigned int
StreamingManager<TImage>::GetNumberOfSplits()
{
  return m_ComputedNumberOfSplits;
}

template <class TImage>
typename StreamingManager<TImage>::RegionType
StreamingManager<TImage>::GetSplit(unsigned int i)
{
  typename StreamingManager<TImage>::RegionType region( m_Region );
  m_Splitter->GetSplit(i, m_ComputedNumberOfSplits, region);
  return region;
}

} // End namespace otb

#endif

