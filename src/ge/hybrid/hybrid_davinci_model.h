/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HYBRID_HYBRID_DAVINCI_MODEL_H_
#define HYBRID_HYBRID_DAVINCI_MODEL_H_

#include <memory>
#include "external/ge/ge_api_error_codes.h"
#include "graph/load/new_model_manager/data_inputer.h"
#include "model/ge_root_model.h"

namespace ge {
namespace hybrid {
class HybridDavinciModel {
 public:
  ~HybridDavinciModel();

  HybridDavinciModel(const HybridDavinciModel &) = delete;
  HybridDavinciModel(HybridDavinciModel &&) = delete;
  HybridDavinciModel &operator=(const HybridDavinciModel &) = delete;
  HybridDavinciModel &operator=(HybridDavinciModel &&) = delete;

  static std::unique_ptr<HybridDavinciModel> Create(const GeRootModelPtr &ge_root_model);

  Status Init();

  Status ModelRunStart();

  Status ModelRunStop();

  Status EnqueueData(const std::shared_ptr<InputDataWrapper> &data);

  void SetListener(const shared_ptr<ModelListener> &listener);

  void SetModelId(uint32_t model_id);

  void SetDeviceId(uint32_t device_id);

 private:
  HybridDavinciModel() = default;
  class Impl;
  Impl *impl_ = nullptr;
};
}  // namespace hybrid
}  // namespace ge
#endif  // HYBRID_HYBRID_DAVINCI_MODEL_H_
