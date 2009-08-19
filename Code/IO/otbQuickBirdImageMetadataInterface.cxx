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


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <typeinfo>
#include <cassert>

#include "otbMacro.h"

#include "otbQuickBirdImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "itkCreateObjectFunction.h"
#include "itkVersion.h"


namespace otb
{


QuickBirdImageMetadataInterface
::QuickBirdImageMetadataInterface()
{
}


bool
QuickBirdImageMetadataInterface::CanRead( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("QB02") != std::string::npos)
    return true;
  else
    return false;
}


QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface::GetSolarIrradiance( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no QuickBird Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

 VariableLengthVectorType outputValuesVariableLengthVector;
 ossimKeywordlist kwl;
 imageKeywordlist.convertToOSSIMKeywordlist(kwl);

 std::string keyBId= "support_data.band_id";
 ossimString keywordStringBId = kwl.find(keyBId.c_str());
 if( keywordStringBId == ossimString("P") )
    {
        outputValuesVariableLengthVector.SetSize(1);
        outputValuesVariableLengthVector.Fill(1381.79);
    }
    else if(keywordStringBId == ossimString("Multi") )
    {
        outputValuesVariableLengthVector.SetSize(4);
        outputValuesVariableLengthVector[0]=1924.59;
        outputValuesVariableLengthVector[1]=1843.08;
        outputValuesVariableLengthVector[2]=1574.77;
        outputValuesVariableLengthVector[3]=1113.71;
      }
      else
      {
        itkExceptionMacro(<<"Invalid bandID "<<keywordStringBId);
      }

    return outputValuesVariableLengthVector;
}


int
QuickBirdImageMetadataInterface::GetDay( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no QuickBird Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.tlc_date";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Day");

 ossimString day = keywordStrings[2];

  return day.toInt();
}


int
QuickBirdImageMetadataInterface::GetMonth( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no QuickBird Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.tlc_date";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
QuickBirdImageMetadataInterface::GetYear( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no QuickBird Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.tlc_date";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(  keywordStrings.size() <= 2 )
    itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}


QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface
::GetPhysicalBias( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no QuickBird Image");
  }
  
  ImageKeywordlistType ImageKeywordlist;

 if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);

  VariableLengthVectorType outputValuesVariableLengthVector;
  std::string keyBId= "support_data.band_id";
  ossimString keywordStringBId = kwl.find(keyBId.c_str());
  if( keywordStringBId == ossimString("P") )
  {
      outputValuesVariableLengthVector.SetSize(1);
      outputValuesVariableLengthVector.Fill(0.0);
  }
  else if(keywordStringBId == ossimString("Multi") )
  {
      outputValuesVariableLengthVector.SetSize(4);
      outputValuesVariableLengthVector.Fill(0.0);
  }
  else
  {
     itkExceptionMacro(<<"Invalid bandID "<<keywordStringBId);
  }

  return outputValuesVariableLengthVector;
}


QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface
::GetPhysicalGain( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no QuickBird Image");
  }
 
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.generation_date";
  ossimString keywordString = kwl.find(key.c_str());
  std::string output(keywordString.chars());

  //The Ikonos production date has the format MM/DD/YY
  ossimString separatorList = "-";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  if (keywordStrings.size() < 3)
  {
    itkGenericExceptionMacro(<<"Could not retrieve the production date for Ikonos");
  }

  int productionYear = keywordStrings[0].toInt();
  int productionMonth = keywordStrings[1].toInt();
  int productionDay = keywordStrings[2].toInt();
  bool isPost20030606 = false;
  if(productionYear > 2003)
    isPost20030606 = true;
  else
  {
    if(productionYear == 2003)
    {
      if(productionMonth > 6)
        isPost20030606 = true;
      else
        if(productionDay >= 6)
          isPost20030606 = true;
    }
  }

  //Value computed from
  // Radiance Conversion of QuickBird Data - Technical Note.
  ossimString keywordStringBitsPerPixel = kwl.find("support_data.bits_per_pixel");
  int bitsPerPixel = keywordStringBitsPerPixel.toInt();
  if (bitsPerPixel != 16 && bitsPerPixel != 8)
  {
    itkExceptionMacro(<<"Invalid bitsPerPixel "<<bitsPerPixel);
  }

  ossimString keywordStringTDILevel = kwl.find("support_data.TDI_level");
  int TDILevel = keywordStringTDILevel.toInt();
  if (TDILevel != 10 && TDILevel != 13 && TDILevel != 18 && TDILevel != 24 && TDILevel != 32)
  {
    itkExceptionMacro(<<"Invalid TDILevel "<<TDILevel);
  }
  
  std::string keyBId= "support_data.band_id";
  ossimString keywordStringBId = kwl.find(keyBId.c_str());
  if (keywordStringBId != ossimString("P") && keywordStringBId != ossimString("Multi"))
  {
    itkExceptionMacro(<<"Invalid bandID "<<keywordStringBId);
  }

  VariableLengthVectorType outputValuesVariableLengthVector;
  if (keywordStringBId == ossimString("P") )
    outputValuesVariableLengthVector.SetSize(1);
  else
  	outputValuesVariableLengthVector.SetSize(4);
  outputValuesVariableLengthVector.Fill(1.);
 
  if (!isPost20030606)
  {
  	if(bitsPerPixel==16)
  	{
 	  if (keywordStringBId == ossimString("P"))
 	  {
 	  	if (TDILevel != 10)
 	  		outputValuesVariableLengthVector[0] = 0.08381880;
 	  	else if (TDILevel != 13)
 	  		outputValuesVariableLengthVector[0] = 0.06447600;
 	  	else if (TDILevel != 18)
 	  		outputValuesVariableLengthVector[0] = 0.04656600;
 	  	else if (TDILevel != 24)
 	  		outputValuesVariableLengthVector[0] = 0.03494440;
 	  	else if (TDILevel != 32)
 	  		outputValuesVariableLengthVector[0] = 0.02618840;
  	  }
  	  else
  	  {
 	  	outputValuesVariableLengthVector[0] = 0.01604120;
   	  	outputValuesVariableLengthVector[1] = 0.01438470;
 	  	outputValuesVariableLengthVector[2] = 0.01267350;
	  	outputValuesVariableLengthVector[3] = 0.01542420;
 	  }
  		
  	}
  	else
  	{
 	  if (keywordStringBId == ossimString("P"))
 	  {
 	  	if (TDILevel != 10)
 	  		outputValuesVariableLengthVector[0] = 1.02681367;
 	  	else if (TDILevel != 13)
 	  		outputValuesVariableLengthVector[0] = 1.02848939;
 	  	else if (TDILevel != 18)
 	  		outputValuesVariableLengthVector[0] = 1.02794702;
 	  	else if (TDILevel != 24)
 	  		outputValuesVariableLengthVector[0] = 1.02989685;
 	  	else if (TDILevel != 32)
 	  		outputValuesVariableLengthVector[0] = 1.02739898;

		ossimString keywordStringAbsCalFactor = kwl.find("support_data.absCalFactor");
  	  	outputValuesVariableLengthVector[0] *= keywordStringAbsCalFactor.toDouble(); 	  	  	
  	  }
  	  else
  	  {
  	    ossimString keywordStringAcalFact = kwl.find("support_data.B_band_absCalFactor");
	    outputValuesVariableLengthVector[0] = 1.12097834 * keywordStringAcalFact.toDouble();
    	keywordStringAcalFact = kwl.find("support_data.G_band_absCalFactor");
      	outputValuesVariableLengthVector[1] = 1.37652632 * keywordStringAcalFact.toDouble();
      	keywordStringAcalFact = kwl.find("support_data.R_band_absCalFactor");
     	outputValuesVariableLengthVector[2] = 1.30954587 * keywordStringAcalFact.toDouble();
     	keywordStringAcalFact = kwl.find("support_data.N_band_absCalFactor");
     	outputValuesVariableLengthVector[3] = 0.98368622 * keywordStringAcalFact.toDouble();
  	  }  		
  	}

    
   }
  else
  {
 	  if (keywordStringBId == ossimString("P"))
 	  {
 	  	ossimString keywordStringAbsCalFactor = kwl.find("support_data.absCalFactor");
  	  	outputValuesVariableLengthVector[0] = keywordStringAbsCalFactor.toDouble(); 
  	  }
  	  else
  	  {
  	    ossimString keywordStringAcalFact = kwl.find("support_data.B_band_absCalFactor");
	    outputValuesVariableLengthVector[0] = keywordStringAcalFact.toDouble();
      	keywordStringAcalFact = kwl.find("support_data.G_band_absCalFactor");
    	outputValuesVariableLengthVector[1] = keywordStringAcalFact.toDouble();
      	keywordStringAcalFact = kwl.find("support_data.R_band_absCalFactor");
     	outputValuesVariableLengthVector[2] = keywordStringAcalFact.toDouble();     	
     	keywordStringAcalFact = kwl.find("support_data.N_band_absCalFactor");
     	outputValuesVariableLengthVector[3] = keywordStringAcalFact.toDouble();
  	  }  		
  }

  if (keywordStringBId == ossimString("P") )
  {  
	outputValuesVariableLengthVector[0] = 0.398 / outputValuesVariableLengthVector[0];
  }
  else
  {
      outputValuesVariableLengthVector[0] = 0.068 / outputValuesVariableLengthVector[0];
	  outputValuesVariableLengthVector[1] = 0.099 / outputValuesVariableLengthVector[1];
      outputValuesVariableLengthVector[2] = 0.071 / outputValuesVariableLengthVector[2];
      outputValuesVariableLengthVector[3] = 0.114 / outputValuesVariableLengthVector[3];
  }

  return outputValuesVariableLengthVector;
}



/*************************************************
***** QuickBirdImageMetadataInterfaceFactory *****
**************************************************/
QuickBirdImageMetadataInterfaceFactory
::QuickBirdImageMetadataInterfaceFactory()
{
  this->RegisterOverride("ImageMetadataInterfaceBase",
                         "otbQuickBirdImageMetadataInterface",
                         "QuickBird Meteada Interface",
                         1,
                         itk::CreateObjectFunction<QuickBirdImageMetadataInterface >::New());
}

QuickBirdImageMetadataInterfaceFactory
::~QuickBirdImageMetadataInterfaceFactory()
{
}

const char*
QuickBirdImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
QuickBirdImageMetadataInterfaceFactory::GetDescription() const
{
  return "QuickBird Metadata Interface Factory, handle QuickBird metadata in OTB";
}

} // end namespace otb
