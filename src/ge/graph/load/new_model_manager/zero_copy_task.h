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

#ifndef GE_GRAPH_LOAD_NEW_MODEL_MANAGER_ZERO_COPY_TASK_H_
#define GE_GRAPH_LOAD_NEW_MODEL_MANAGER_ZERO_COPY_TASK_H_

#include <map>
#include <set>
#include <vector>
#include <string>

#include "external/ge/ge_api_error_codes.h"
#include "framework/common/ge_types.h"
#include "runtime/mem.h"

using std::map;
using std::set;
using std::string;
using std::vector;

namespace ge {
class ZeroCopyTask {
 public:
  ZeroCopyTask(const string &name, uint8_t *args, size_t size);
  ~ZeroCopyTask();

  /**
   * @ingroup ge
   * @brief Set Task zero copy addr info.
   * @param [in] addr: task addr value.
   * @param [in] offset: saved offset in task args.
   * @return: 0 SUCCESS / others FAILED
   */
  ge::Status SetTaskArgsOffset(uintptr_t addr, size_t offset);

  /**
   * @ingroup ge
   * @brief Is need zero copy.
   * @return: true / false
   */
  bool IsTaskArgsSet() const { return !task_addr_offset_.empty(); }

  /**
   * @ingroup ge
   * @brief Save orignal data of task args.
   * @param [in] info: task args orignal data.
   * @param [in] size: args size.
   * @return: void
   */
  void SetOriginalArgs(const void *info, size_t size);

  /**
   * @ingroup ge
   * @brief Set user data addr to Task param.
   * @param [in] addr: virtual address value from Op.
   * @param [in] data: data buffer from user.
   * @param [in] batch_addrs: dynamic batch addr info.
   * @param [in] batch_label: batch label.
   * @return: 0 SUCCESS / others FAILED
   */
  ge::Status UpdateTaskParam(uintptr_t addr, const DataBuffer &data, const map<string, set<uintptr_t>> &batch_addrs,
                             const string &batch_label);

  /**
   * @ingroup ge
   * @brief Update task param to device.
   * @param [in] stream: Stream for asychronous update.
   * @return: 0 SUCCESS / others FAILED
   */
  ge::Status DistributeParam(rtStream_t stream);

 protected:
  bool CheckDynamicBatch(const map<string, set<uintptr_t>> &batch_addrs, const string &batch_label, uintptr_t addr);

 private:
  const string name_;

  uint8_t *args_addr_;
  const size_t args_size_;
  vector<uint8_t> args_info_;
  bool is_updated_;

  // <address from Op, {offset in args}>
  map<uintptr_t, vector<size_t>> task_addr_offset_;
};
}  // namespace ge
#endif  // GE_GRAPH_LOAD_NEW_MODEL_MANAGER_ZERO_COPY_TASK_H_