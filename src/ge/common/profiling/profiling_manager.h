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

#ifndef GE_COMMON_PROFILING_PROFILING_MANAGER_H_
#define GE_COMMON_PROFILING_PROFILING_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "framework/common/ge_inner_error_codes.h"
#include "framework/common/ge_types.h"
#include "external/register/register_types.h"
#include "toolchain/prof_engine.h"
#include "toolchain/prof_mgr_core.h"

using std::map;
using std::string;
using std::vector;

namespace ge {
const std::string GE_PROFILING_MODULE = "Framework";
// register Plugin
class FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY PluginImpl : public Msprof::Engine::PluginIntf {
 public:
  explicit PluginImpl(const std::string &module);
  ~PluginImpl() {}

  int Init(const Msprof::Engine::Reporter *reporter);
  int UnInit();
  static Msprof::Engine::Reporter *GetPluginReporter() { return reporter_; }

 private:
  static Msprof::Engine::Reporter *reporter_;
  std::string module_;
};

// register Engine
class ProfilingEngineImpl : public Msprof::Engine::EngineIntf {
 public:
  ProfilingEngineImpl() {}
  ~ProfilingEngineImpl() {}

  Msprof::Engine::PluginIntf *CreatePlugin();
  int ReleasePlugin(Msprof::Engine::PluginIntf *plugin);
};

class FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY ProfilingManager {
 public:
  ProfilingManager();
  virtual ~ProfilingManager();
  static ProfilingManager &Instance();
  ge::Status Init(const Options &options);
  ge::Status InitFromOptions(const Options &options);
  ge::Status InitFromAclCfg(const std::string &config);
  ge::Status StartProfiling(int32_t iter, int32_t device_id);
  void StopProfiling();
  bool ProfilingOpTraceOn() const { return is_op_trace_; }
  bool ProfilingLoadFlag() const { return is_load_; }
  bool ProfilingOn() const { return is_profiling_; }
  int32_t GetOpTraceIterNum() const { return op_trace_iter_num_; }
  void ReportProfilingData(const std::vector<TaskDescInfo> &task_desc_info,
                           const std::vector<ComputeGraphDescInfo> &compute_graph_desc_info);
  void Report(const int32_t &device_id, const string &data, Msprof::Engine::Reporter &reporter,
              Msprof::Engine::ReporterData &reporter_data);
  void ProfilingTaskDescInfo(const std::vector<TaskDescInfo> &task_desc_info, const int32_t &device_id);
  void ProfilingGraphDescInfo(const std::vector<ComputeGraphDescInfo> &compute_graph_desc_info,
                              const int32_t &device_id);
  void SetProfilingConfig(const string &profiling_cfg);
  vector<int32_t> GetProfilingDeviceId() const { return device_id_; }
  void PluginUnInit(const std::string &module) const;

 private:
  bool is_profiling_ = false;
  bool is_op_trace_ = false;
  bool is_load_ = false;
  int32_t op_trace_iter_num_ = 0;
  string job_id_;
  vector<int32_t> device_id_;
  vector<string> op_trace_conf_;
  vector<string> profiling_opts_;
  vector<void *> prof_handle_vec_;
  string recv_profiling_config_;
  string send_profiling_config_;
  string system_trace_conf_;
  const ProfilingEngineImpl engine_;
};
}  // namespace ge
#endif  // GE_COMMON_PROFILING_PROFILING_MANAGER_H_
