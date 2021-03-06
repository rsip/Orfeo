#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBSeamCarvingTests
otbSeamCarvingTestDriver.cxx
otbRemoveCarvingPathFilterNew.cxx
otbImageToCarvingPathFilterNew.cxx
otbAddCarvingPathFilterNew.cxx
)

add_executable(otbSeamCarvingTestDriver ${OTBSeamCarvingTests})
target_link_libraries(otbSeamCarvingTestDriver ${OTBSeamCarving-Test_LIBRARIES})
otb_module_target_label(otbSeamCarvingTestDriver)

# Tests Declaration

otb_add_test(NAME feTuRemoveCarvingPathFilterNew COMMAND otbSeamCarvingTestDriver
  otbRemoveCarvingPathFilterNew)

otb_add_test(NAME feTuImageToCarvingPathFilterNew COMMAND otbSeamCarvingTestDriver
  otbImageToCarvingPathFilterNew)

otb_add_test(NAME feTuAddCarvingPathFilterNew COMMAND otbSeamCarvingTestDriver
  otbAddCarvingPathFilterNew)
