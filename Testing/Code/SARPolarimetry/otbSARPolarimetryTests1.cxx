/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbPolarimetricSynthesisFilterNew);
  REGISTER_TEST(otbPolarimetricSynthesisFilter);
  REGISTER_TEST(otbHHnHVnVVPolarimetricSynthesisFilter);
  REGISTER_TEST(otbHHnHVPolarimetricSynthesisFilter);
  REGISTER_TEST(otbVHnVVPolarimetricSynthesisFilter);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilterNew);
  REGISTER_TEST(otbMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbVectorMultiChannelsPolarimetricSynthesisFilter);
  REGISTER_TEST(otbSinclairToCovarianceFunctor);
  REGISTER_TEST(otbSinclairImageFilter);
  REGISTER_TEST(otbMLCToCoherencyImageFilterNew);

}
