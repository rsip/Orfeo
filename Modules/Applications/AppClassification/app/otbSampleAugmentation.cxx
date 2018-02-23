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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbOGRDataSourceWrapper.h"

namespace otb
{
namespace Wrapper
{

class SampleAugmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef SampleAugmentation              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SampleAugmentation, otb::Application);

  /** Filters typedef */

private:
  SampleAugmentation() {}

  void DoInit()
  {
    SetName("SampleAugmentation");
    SetDescription("Generates synthetic samples from a sample data file.");

    // Documentation
    SetDocName("Sample Extraction");
    SetDocLongDescription("The application takes a sample data file as "
                          "generated by the SampleExtraction application and "
                          "generates synthetic samples to increase the number of "
                          "available samples.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputFilename, "vec", "Input samples");
    SetParameterDescription("vec","Vector data file containing samples (OGR format)");

    AddParameter(ParameterType_OutputFilename, "out", "Output samples");
    SetParameterDescription("out","Output vector data file storing new samples"
                            "(OGR format). If not given, the input vector data file is updated");
    MandatoryOff("out");

    AddParameter(ParameterType_ListView, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    SetListViewSingleSelectionMode("field",true);
    
    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);

    AddParameter(ParameterType_Int, "label", "Label of the class to be augmented");
    SetParameterDescription("label", "Label of the class of the input file for which "
                            "new samples will be generated.");
    SetDefaultParameterInt("label",1);

    AddParameter(ParameterType_Int, "samples", "Number of generated samples");
    SetParameterDescription("samples", "Number of synthetic samples that will "
                            "be generated.");
    SetDefaultParameterInt("samples",100);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("vec", "samples.sqlite");
    SetDocExampleParameterValue("field", "class");
    SetDocExampleParameterValue("label", "3");
    SetDocExampleParameterValue("samples", "100");
    SetDocExampleParameterValue("out","augmented_samples.sqlite");

    SetOfficialDocLink();
  }

  void DoUpdateParameters()
  {
    if ( HasValue("vec") )
      {
      std::string vectorFile = GetParameterString("vec");
      ogr::DataSource::Pointer ogrDS =
        ogr::DataSource::New(vectorFile, ogr::DataSource::Modes::Read);
      ogr::Layer layer = ogrDS->GetLayer(this->GetParameterInt("layer"));
      ogr::Feature feature = layer.ogr().GetNextFeature();

      ClearChoices("field");
      
      for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
        {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;
        std::string::iterator end = std::remove_if(key.begin(),key.end(),
                                                   [](auto c){return !std::isalnum(c);});
        std::transform(key.begin(), end, key.begin(), tolower);
        
        OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(iField)->GetType();
        
        if(fieldType == OFTString || fieldType == OFTInteger || ogr::version_proxy::IsOFTInteger64(fieldType))
          {
          std::string tmpKey="field."+key.substr(0, end - key.begin());
          AddChoice(tmpKey,item);
          }
        }
      }
  }

  void DoExecute()
    {
    ogr::DataSource::Pointer vectors;
    ogr::DataSource::Pointer output;
    if (IsParameterEnabled("out") && HasValue("out"))
      {
      vectors = ogr::DataSource::New(this->GetParameterString("vec"));
      output = ogr::DataSource::New(this->GetParameterString("out"),
                                    ogr::DataSource::Modes::Overwrite);
      }
    else
      {
      // Update mode
      vectors = ogr::DataSource::New(this->GetParameterString("vec"),
                                    ogr::DataSource::Modes::Update_LayerUpdate);
      output = vectors;
      }

    // Retrieve the field name
    std::vector<int> selectedCFieldIdx = GetSelectedItems("field");

    if(selectedCFieldIdx.empty())
      {
      otbAppLogFATAL(<<"No field has been selected for data labelling!");
      }

  std::vector<std::string> cFieldNames = GetChoiceNames("field");  
  std::string fieldName = cFieldNames[selectedCFieldIdx.front()];
    

  output->SyncToDisk();
    }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SampleAugmentation)
