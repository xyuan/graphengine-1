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

#ifndef GE_GRAPH_LOAD_NEW_MODEL_MANAGER_DAVINCI_MODEL_H_
#define GE_GRAPH_LOAD_NEW_MODEL_MANAGER_DAVINCI_MODEL_H_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "common/ge_types.h"
#include "common/helper/model_helper.h"
#include "common/helper/om_file_helper.h"
#include "common/opskernel/ge_task_info.h"
#include "common/types.h"
#include "framework/common/util.h"
#include "graph/debug/ge_attr_define.h"
#include "graph/load/new_model_manager/data_dumper.h"
#include "graph/load/new_model_manager/data_inputer.h"
#include "graph/load/new_model_manager/model_utils.h"
#include "graph/load/new_model_manager/aipp_utils.h"
#include "graph/load/new_model_manager/zero_copy_task.h"
#include "graph/model.h"
#include "graph/node.h"
#include "graph/op_desc.h"
#include "graph/operator.h"
#include "graph/utils/attr_utils.h"
#include "graph/utils/tensor_utils.h"
#include "mmpa/mmpa_api.h"
#include "proto/task.pb.h"
#include "task_info/task_info.h"

namespace ge {
typedef enum tagModelProcStage {
  MODEL_LOAD_START = 1,
  MODEL_LOAD_END,
  MODEL_PRE_PROC_START,
  MODEL_PRE_PROC_END,
  MODEL_INFER_START,
  MODEL_INFER_END,
  MODEL_AFTER_PROC_START,
  MODEL_AFTER_PROC_END,
  MODEL_PROC_INVALID,
} ModelProcStage;

struct timeInfo {
  uint32_t modelId;
  int64_t processBeginTime;
  int64_t processEndTime;
  int64_t inferenceBeginTime;
  int64_t inferenceEndTime;
  int64_t dumpBeginTime;
  int64_t dumpEndTime;
};

// comments
class DavinciModel {
 public:
  ///
  /// @ingroup ge
  /// @brief DavinciModel constructor
  /// @author
  ///
  DavinciModel(int32_t priority, const std::shared_ptr<ModelListener> &listener);

  ///
  /// @ingroup ge
  /// @brief DavinciModel desctructor, free Parse and Init resources
  /// @author
  ///
  ~DavinciModel();

  ///
  /// @ingroup ge
  /// @brief apply model to model_def_
  ///
  Status Assign(const GeModelPtr &ge_model);

  ///
  /// @ingroup ge
  /// @brief DavinciModel initialization, including Stream, ccHandle, Event, DataInputer, etc
  /// @return execute result
  /// @author
  ///
  Status Init(void *dev_ptr = nullptr, size_t memsize = 0, void *weight_ptr = nullptr, size_t weightsize = 0);

  ///
  /// @ingroup ge
  /// @brief ACL case, Load task list with queue.
  /// @param [in] input_que_ids: input queue ids from user, nums equal Data Op.
  /// @param [in] output_que_ids: input queue ids from user, nums equal NetOutput Op.
  /// @return: 0 for success / others for fail
  ///
  Status SetQueIds(const std::vector<uint32_t> &input_queue_ids, const std::vector<uint32_t> &output_queue_ids);

  ///
  /// @ingroup ge
  /// @brief Get DataInputer
  /// @return model ID
  ///
  uint32_t Id() const { return model_id_; }

  ///
  /// @ingroup ge
  /// @brief Get DataInputer
  /// @return model ID
  ///
  void SetId(uint32_t model_id) { model_id_ = model_id; }

  static void *Run(DavinciModel *model_pointer);

  ///
  /// @ingroup ge
  /// @brief NnExecute
  /// @param [in] stream   execute stream
  /// @param [in] async_mode  is asynchronize mode.
  /// @param [in] input_data  model input data
  /// @param [out] output_data  model output data
  ///
  Status NnExecute(rtStream_t stream, bool async_mode, const InputData &input_data, OutputData &output_data);

  ///
  /// @ingroup ge
  /// @brief lock mutex run flag
  /// @author
  ///
  void LockRunFlg() { mux_run_flg_.lock(); }

  ///
  /// @ingroup ge
  /// @brief unlock mutex run flag
  /// @author
  ///
  void UnlockRunFlg() { mux_run_flg_.unlock(); }

  ///
  /// @ingroup ge
  /// @brief get DataInputer
  /// @return DataInputer pointer
  ///
  DataInputer *const GetDataInputer() const { return data_inputer_; }

  // get Stream number
  uint32_t StreamNum() const { return runtime_param_.stream_num; }

  // get Event number
  uint32_t EventNum() const { return runtime_param_.event_num; }

  // get Lable number
  uint32_t LabelNum() const { return runtime_param_.label_num; }

  // get batch number
  uint32_t BatchNum() const { return runtime_param_.batch_num; }

  // get session id
  uint64_t SessionId() const { return runtime_param_.session_id; }

  vector<OpDescPtr> GetOpDesc() {
    vector<OpDescPtr> opDescVector;
    GE_IF_BOOL_EXEC(AttrUtils::GetListOpDesc(GetGeModel(), MODEL_ATTR_FUSION_MODEL_DEF, opDescVector),
                    GELOGI("get opDesc of opDescVector"));
    return opDescVector;
  }

  // get model priority
  int32_t Priority() const { return priority_; }

  // get total mem size
  size_t TotalMemSize() const { return runtime_param_.mem_size; }

  // model name
  string Name() { return name_; }

  // version
  uint32_t Version() const { return version_; }

  // get total weights mem size
  size_t TotalWeightsMemSize() const { return runtime_param_.weight_size; }

  size_t TotalVarMemSize() const { return runtime_param_.var_size; }

  // get base memory address
  uint8_t *MemBase() { return mem_base_; }

  // get weight base memory address
  uint8_t *WeightsMemBase() { return weights_mem_base_; }

  uint8_t *VarMemBase() { return var_mem_base_; }

  // get Event list
  const vector<rtEvent_t> &GetEventList() const { return event_list_; }

  const vector<rtStream_t> &GetStreamList() const { return stream_list_; }

  const vector<rtLabel_t> &GetLabelList() const { return label_list_; }

  Status DestroyThread();

  // Get Data Op.
  const vector<OpDescPtr> &GetDataList() const { return data_op_list_; }

  // get Op
  const map<uint32_t, OpDescPtr> &GetOpList() const { return op_list_; }

  OpDescPtr GetOpByIndex(uint32_t index) const {
    if (op_list_.find(index) == op_list_.end()) {
      return nullptr;
    }
    return op_list_.at(index);
  }

  OpDescPtr GetVariableOp(const string &name) {
    for (auto op_desc : variable_op_list_) {
      if (op_desc != nullptr && op_desc->GetName() == name) {
        return op_desc;
      }
    }
    return nullptr;
  }
  // get task info for profiling
  const std::vector<TaskDescInfo> &GetTaskDescInfo() const { return task_desc_info_; }

  // get updated task info list
  std::vector<TaskInfoPtr> GetTaskList() { return task_list_; }

  ///
  /// @ingroup ge
  /// @brief get model input and output format
  /// @return ccTensorFormat_t current model input and output format
  ///
  Format GetFormat();

  rtModel_t GetRtModelHandle() {
    rtModel_t res = rt_model_handle_;
    return res;
  }

  rtStream_t GetRtModelStream() {
    rtModel_t res = rt_model_stream_;
    return res;
  }

  uint64_t GetRtBaseAddr() const { return runtime_param_.logic_mem_base; }

  uint64_t GetRtWeightAddr() const { return runtime_param_.logic_weight_base; }

  uint64_t GetRtVarAddr() const { return runtime_param_.logic_var_base; }

  uint32_t GetFlowctrlIndex(uint32_t op_index);

  void PushHcclStream(rtStream_t value);

  bool IsBroadCastOpData(const NodePtr &var_node);

  ///
  /// @ingroup ge
  /// @brief For TVM Op, avoid Addr Reuse.
  /// @return void*
  ///
  static const char *GetRegisterStub(const string &tvm_binfile_key, const string &session_graph_model_id = "");

  ///
  /// @ingroup ge
  /// @brief get model input and output desc info
  /// @param [out] input_shape  model input size
  /// @param [out] output_shape model output size
  /// @return execute result
  ///
  Status GetInputOutputDescInfo(vector<InputOutputDescInfo> &input_desc, vector<InputOutputDescInfo> &output_desc);

  Status GetInputOutputDescInfo(vector<InputOutputDescInfo> &input_desc, vector<InputOutputDescInfo> &output_desc,
                                std::vector<uint32_t> &inputFormats, std::vector<uint32_t> &output_formats);

  ///
  /// @ingroup ge
  /// @brief Get dynamic batch_info
  /// @param [out] batch_info
  /// @return execute result
  ///
  Status GetDynamicBatchInfo(std::vector<std::vector<int64_t>> &batch_info);

  void GetCurShape(std::vector<int64_t> &batch_info);

  void GetModelAttr(std::vector<std::string> &dynamic_output_shape_info);

  ///
  /// @ingroup ge
  /// @brief Get AIPP input info
  /// @param [in] index
  /// @param [out] aipp_info
  /// @return execute result
  ///
  Status GetAIPPInfo(uint32_t index, AippConfigInfo &aipp_info);

  ///
  /// @ingroup ge
  /// @brief Get model_id.
  /// @return model_id
  ///
  uint32_t GetModelId() const { return model_id_; }

  ///
  /// @ingroup ge
  /// @brief get unique identification for op when load two or more models
  /// @param [in] op_desc : current op.
  /// @param [in] string identification: unique identification for current op.
  /// @return None
  ///
  void GetUniqueId(const OpDescPtr &op_desc, std::string &unique_identification);

  ///
  /// @ingroup ge
  /// @brief get model input and output desc for zero copy
  /// @param [out] input_shape  model input size
  /// @param [out] output_shape model output size
  /// @return execute result
  ///
  Status GetInputOutputDescInfoForZeroCopy(vector<InputOutputDescInfo> &input_desc,
                                           vector<InputOutputDescInfo> &output_desc,
                                           std::vector<uint32_t> &inputFormats, std::vector<uint32_t> &output_formats);

  Status ReturnResult(uint32_t data_id, const bool rslt_flg, const bool seq_end_flg, OutputData *output_data);

  Status ReturnNoOutput(uint32_t data_id);

  ///
  /// @ingroup ge
  /// @brief dump all op input and output information
  /// @param [in] op_list model_id
  /// @return Status
  ///
  Status DumpOpInputOutput();

  ///
  /// @ingroup ge
  /// @brief dump single op input and output information
  /// @param [in] dump_op model_id
  /// @return Status
  ///
  Status DumpSingleOpInputOutput(const OpDescPtr &dump_op);

  Status ModelRunStart();

  ///
  /// @ingroup ge
  /// @brief stop run model
  /// @return Status
  ///
  Status ModelRunStop();

  ///
  /// @ingroup ge
  /// @brief model run flag
  /// @return Status
  ///
  bool RunFlag() const { return run_flg_; }

  Status GetOutputDescInfo(vector<InputOutputDescInfo> &output_desc, std::vector<uint32_t> &formats);

  ///
  /// @ingroup ge
  /// @brief Set Session Id
  /// @return void
  ///
  void SetSessionId(uint64_t session_id) { session_id_ = session_id; }

  ///
  /// @ingroup ge
  /// @brief Get Session Id
  /// @return sessionID
  ///
  uint64_t GetSessionId() const { return session_id_; }

  ///
  /// @ingroup ge
  /// @brief SetDeviceId
  /// @return void
  ///
  void SetDeviceId(uint32_t device_id) { device_id_ = device_id; }

  ///
  /// @ingroup ge
  /// @brief Get device Id
  /// @return  device id
  ///
  uint32_t GetDeviceId() const { return device_id_; }

  GeModelPtr GetGeModel() { return ge_model_; }

  const RuntimeParam &GetRuntimeParam() { return runtime_param_; }

  int32_t GetDataInputTid() const { return dataInputTid; }
  void SetDataInputTid(int32_t data_input_tid) { dataInputTid = data_input_tid; }

  void DisableZeroCopy(const void *addr);

  ///
  /// @ingroup ge
  /// @brief Save outside address of Data or NetOutput used info for ZeroCopy.
  /// @param [in] const OpDescPtr &op_desc: current op desc
  /// @param [in] const std::vector<void *> &outside_addrs: address of task
  /// @param [in] const void *args_offset: arguments address save the address.
  /// @return None.
  ///
  void SetZeroCopyAddr(const OpDescPtr &op_desc, const std::vector<void *> &outside_addrs, const void *info, void *args,
                       size_t size, size_t offset);

  void SetDynamicSize(const std::vector<uint64_t> &batch_num);

  bool GetL1FusionEnableOption() { return is_l1_fusion_enable_; }

  void SetProfileTime(ModelProcStage stage, int64_t endTime = 0);

  int64_t GetLoadBeginTime() { return load_begin_time_; }

  int64_t GetLoadEndTime() { return load_end_time_; }

  Status SinkModelProfile();

  Status SinkTimeProfile(const InputData &current_data);

  void SaveDumpTask(uint32_t task_id, uint32_t stream_id, const std::shared_ptr<OpDesc> &op_desc, uintptr_t args) {
    data_dumper_.SaveDumpTask(task_id, stream_id, op_desc, args);
  }

  void SetEndGraphId(uint32_t task_id, uint32_t stream_id);
  DavinciModel &operator=(const DavinciModel &model) = delete;

  DavinciModel(const DavinciModel &model) = delete;

  const vector<std::pair<rtStream_t, int64_t>> &GetHcclFolowStream() { return capacity_of_stream_; }
  void CreateHcclFollowStream(rtStream_t stream, int64_t remain_cap);
  void ReuseHcclFollowStream(int64_t remain_cap, int64_t &index);

  void InitRuntimeParams();
  Status InitVariableMem();

  void UpdateMemBase(uint8_t *mem_base) {
    runtime_param_.mem_base = mem_base;
    mem_base_ = mem_base;
  }
  void SetTotalArgsSize(uint32_t args_size) { total_args_size_ += args_size; }
  uint32_t GetTotalArgsSize() { return total_args_size_; }
  void *GetCurrentArgsAddr(uint32_t offset) {
    void *cur_args = static_cast<char *>(args_) + offset;
    return cur_args;
  }
  void SetKnownNode(bool known_node) { known_node_ = known_node; }
  bool IsKnownNode() { return known_node_; }
  Status MallocKnownArgs();
  Status UpdateKnownNodeArgs(const vector<void *> &inputs, const vector<void *> &outputs);
  Status CreateKnownZeroCopyMap(const vector<void *> &inputs, const vector<void *> &outputs);
  Status UpdateKnownZeroCopyAddr(vector<void *> &io_addrs, uint32_t args_offset);

  Status GetOrigInputInfo(uint32_t index, OriginInputInfo &orig_input_info);
  Status GetAllAippInputOutputDims(uint32_t index, std::vector<InputOutputDims> &input_dims,
                                   std::vector<InputOutputDims> &output_dims);

 private:
  // memory address of weights
  uint8_t *weights_mem_base_;
  uint8_t *var_mem_base_;
  // memory address of model
  uint8_t *mem_base_;
  bool is_inner_mem_base_;
  bool is_inner_weight_base_;
  // input data manager
  DataInputer *data_inputer_;

  int64_t load_begin_time_;
  int64_t load_end_time_;
  struct timeInfo time_info_;
  int32_t dataInputTid;

  void InitZeroCopyUtil(bool is_dynamic_batch, bool &input_zero_copy, bool &output_zero_copy);

  ///
  /// @ingroup ge
  /// @brief Save Batch label Info.
  /// @param [in] const OpDescPtr &op_desc
  /// @param [in] uintptr_t addr: address value in args block.
  /// @return None.
  ///
  void SetBatchLabelAddr(const OpDescPtr &op_desc, uintptr_t addr);

  ///
  /// @ingroup ge
  /// @brief Save Data address info for ZeroCopy.
  /// @param [in] const std::vector<void *> &outside_addrs
  /// @return None.
  ///
  void SetInputOutsideAddr(const std::vector<void *> &outside_addrs);

  ///
  /// @ingroup ge
  /// @brief Save NetOutput address info for ZeroCopy.
  /// @param [in] const std::vector<void *> &outside_addrs
  /// @return None.
  ///
  void SetOutputOutsideAddr(const std::vector<void *> &outside_addrs);

  ///
  /// @ingroup ge
  /// @brief Copy Check input size and model op size.
  /// @param [in] const int64_t &input_size: input size.
  /// @param [in] const int64_t &op_size: model op size.
  /// @param [in] is_dynamic: dynamic batch input flag.
  /// @return true if success
  ///
  bool CheckInputAndModelSize(const int64_t &input_size, const int64_t &op_size, bool is_dynamic);

  ///
  /// @ingroup ge
  /// @brief Copy Input/Output to model for direct use.
  /// @param [in] const InputData &input_data: user input data info.
  /// @param [in/out] OutputData &output_data: user output data info.
  /// @param [in] bool is_dynamic: whether is dynamic input, true: is dynamic input; false: not is dynamic input
  /// @return SUCCESS handle successfully / others handle failed
  ///
  Status CopyModelData(const InputData &input_data, OutputData &output_data, bool is_dynamic);

  ///
  /// @ingroup ge
  /// @brief Copy Data addr to model for direct use.
  /// @param [in] data_info: model memory addr/size map { data_index, { tensor_size, tensor_addr } }.
  /// @param [in] is_input: input data or output data
  /// @param [in] blobs: user input/output data list.
  /// @param [in] is_dynamic: whether is dynamic input, true: is dynamic input; false: not is dynamic input
  /// @param [in] batch_label: batch label for multi-batch scenes
  /// @return SUCCESS handle successfully / others handle failed
  ///
  Status UpdateIoTaskArgs(const map<uint32_t, pair<int64_t, void *>> &data_info, bool is_input,
                          const vector<DataBuffer> &blobs, bool is_dynamic, const string &batch_label);

  Status CopyInputData(const InputData &input_data, bool device_data = false);

  Status CopyOutputData(uint32_t data_id, OutputData &output_data);

  Status CopyOutputDataToUser(OpDescPtr &op_desc, std::vector<DataBuffer> &blobs, uint32_t &data_index);

  Status SyncVarData();

  Status SyncDataAndDump();

  Status InitModelMem(void *dev_ptr, size_t memsize, void *weight_ptr, size_t weightsize);

  Status GetInputDescInfo(vector<InputOutputDescInfo> &input_desc, std::vector<uint32_t> &formats);

  Status InitTaskInfo(domi::ModelTaskDef &modelTaskInfo);

  void UnbindHcomStream();

  Status DistributeTask();

  uint8_t *MallocFeatureMapMem(size_t data_size);

  uint8_t *MallocWeightsMem(size_t weights_size);

  void FreeFeatureMapMem();

  void FreeWeightsMem();

  void ReleaseTask();

  void UnbindTaskSinkStream();

  bool IsAicpuKernelConnectSpecifiedLayer();

  Status MarkSpecifiedAicpuKernel();

  ///
  /// @ingroup ge
  /// @brief Travel all nodes and do some init.
  /// @param [in] compute_graph: ComputeGraph to load.
  /// @return Status
  ///
  Status InitNodes(const ComputeGraphPtr &compute_graph);

  ///
  /// @ingroup ge
  /// @brief Data Op Initialize.
  /// @param [in] NodePtr: Data Op.
  /// @param [in/out] data_op_index: NetOutput addr size info.
  /// @return Status
  ///
  Status InitDataOp(const NodePtr &node, uint32_t &data_op_index);

  ///
  /// @ingroup ge
  /// @brief input zero copy node Initialize.
  /// @param [in] NodePtr: Data Op.
  /// @return Status
  ///
  Status InitInputZeroCopyNodes(const NodePtr &node);

  ///
  /// @ingroup ge
  /// @brief NetOutput Op Initialize.
  /// @param [in] NodePtr: NetOutput Op.
  /// @return Status
  ///
  Status InitNetOutput(const NodePtr &node);

  ///
  /// @ingroup ge
  /// @brief output zero copy node Initialize.
  /// @param [in] NodePtr: Data Op.
  /// @return Status
  ///
  Status InitOutputZeroCopyNodes(const NodePtr &node);

  ///
  /// @ingroup ge
  /// @brief Constant Op Init.
  /// @return Status
  ///
  Status InitConstant(const OpDescPtr &op_desc);

  Status InitVariable(const OpDescPtr &op_desc);

  /// @ingroup ge
  /// @brief LabelSet Op Initialize.
  /// @param [in] op_desc: LabelSet Op descriptor.
  /// @return Status
  Status InitLabelSet(const OpDescPtr &op_desc);

  Status InitStreamSwitch(const OpDescPtr &op_desc);

  Status InitStreamActive(const OpDescPtr &op_desc);

  Status InitStreamSwitchN(const OpDescPtr &op_desc);

  ///
  /// @ingroup ge
  /// @brief TVM Op Init.
  /// @return Status
  ///
  Status InitTbeHandle(const OpDescPtr &op_desc);

  void StoreTbeHandle(const std::string &handle_key);
  void CleanTbeHandle();

  ///
  /// @ingroup ge
  /// @brief Make active stream list and bind to model.
  /// @return: 0 for success / others for fail
  ///
  Status BindModelStream();

  ///
  /// @ingroup ge
  /// @brief Init model stream for NN model.
  /// @return Status
  ///
  Status InitModelStream(rtStream_t stream);

  ///
  /// @ingroup ge
  /// @brief ACL, Load task list with queue entrance.
  /// @return: 0 for success / others for fail
  ///
  Status LoadWithQueue();

  ///
  /// @ingroup ge
  /// @brief ACL, Bind Data Op addr to input queue.
  /// @return: 0 for success / others for fail
  ///
  Status BindInputQueue();

  Status CpuTaskModelZeroCopy(std::vector<uintptr_t> &mbuf_list,
                              std::map<const void *, std::vector<void *>> &outside_addrs);

  ///
  /// @ingroup ge
  /// @brief ACL, Bind NetOutput Op addr to output queue.
  /// @return: 0 for success / others for fail
  ///
  Status BindOutputQueue();
  Status CpuModelPrepareOutput(uintptr_t addr, uint32_t size);

  ///
  /// @ingroup ge
  /// @brief definiteness queue schedule, bind input queue to task.
  /// @param [in] queue_id: input queue id from user.
  /// @param [in] addr: Data Op output tensor address.
  /// @param [in] size: Data Op output tensor size.
  /// @return: 0 for success / others for fail
  ///
  Status CpuModelDequeue(uint32_t queue_id);

  ///
  /// @ingroup ge
  /// @brief definiteness queue schedule, bind output queue to task.
  /// @param [in] queue_id: output queue id from user.
  /// @param [in] addr: NetOutput Op input tensor address.
  /// @param [in] size: NetOutput Op input tensor size.
  /// @return: 0 for success / others for fail
  ///
  Status CpuModelEnqueue(uint32_t queue_id, uintptr_t addr, uint32_t size);

  ///
  /// @ingroup ge
  /// @brief definiteness queue schedule, active original model stream.
  /// @param [in] streams: streams will active by S0.
  /// @return: 0 for success / others for fail
  ///
  Status CpuActiveStream(const std::vector<rtStream_t> &stream_list);

  ///
  /// @ingroup ge
  /// @brief definiteness queue schedule, wait for end graph.
  /// @return: 0 for success / others for fail
  ///
  Status CpuWaitEndGraph();

  Status BindEnqueue();
  Status CpuModelEnqueue(uint32_t queue_id, uintptr_t out_mbuf);
  ///
  /// @ingroup ge
  /// @brief definiteness queue schedule, repeat run model.
  /// @return: 0 for success / others for fail
  ///
  Status CpuModelRepeat();

  ///
  /// @ingroup ge
  /// @brief set ts device.
  /// @return: 0 for success / others for fail
  ///
  Status SetTSDevice();

  void CheckHasHcomOp();

  Status DoTaskSink();

  void CreateOutput(uint32_t index, OpDescPtr &op_desc, InputOutputDescInfo &output, uint32_t &format_result);

  uint32_t GetGraphID(const std::string &session_graph_id);

  Status TransAllVarData(ComputeGraphPtr &graph, uint32_t graph_id);
  Status CopyVarData(ComputeGraphPtr &graph);

  // get desc info of graph for profiling
  Status GetComputeGraphInfo(vector<ComputeGraphDescInfo> &compute_graph_desc_info);

  void SetDataDumperArgs();

  Status GenOutputTensorInfo(const OpDescPtr &op_desc, uint32_t data_index, OutputData *output_data,
                             std::vector<ge::OutputTensorInfo> &outputs);

  void ParseAIPPInfo(std::string in_out_info, InputOutputDims &dims_info);

  bool is_model_has_inited_;
  uint32_t model_id_;
  uint32_t runtime_model_id_;
  string name_;
  uint32_t version_;
  GeModelPtr ge_model_;

  map<uint32_t, OpDescPtr> op_list_;

  // data op_desc
  vector<OpDescPtr> data_op_list_;

  vector<OpDescPtr> output_op_list_;

  vector<OpDescPtr> variable_op_list_;

  std::map<uint32_t, std::pair<int64_t, void *>> input_data_info_;   // Virtual address from Data output.
  std::map<uint32_t, std::pair<int64_t, void *>> output_data_info_;  // Virtual address from NetOutput input.

  // output op: save cce op actual needed memory size
  vector<int64_t> output_memory_size_list_;

  std::thread thread_id_;

  std::shared_ptr<ModelListener> listener_;

  bool run_flg_;

  std::mutex mux_run_flg_;

  int32_t priority_;

  vector<rtStream_t> stream_list_;

  std::mutex all_hccl_stream_list_mutex_;
  vector<rtStream_t> all_hccl_stream_list_;

  // for reuse hccl_follow_stream
  std::mutex capacity_of_stream_mutex_;
  std::vector<std::pair<rtStream_t, int64_t>> capacity_of_stream_;

  vector<rtEvent_t> event_list_;

  vector<rtLabel_t> label_list_;
  set<uint32_t> label_id_indication_;

  std::mutex outside_addrs_mutex_;
  std::vector<ZeroCopyTask> zero_copy_tasks_;  // Task used Data or NetOutput addr.
  std::set<const void *> copy_only_addrs_;     // Address need copy to original place.
  // {node_addr, {addr_in_task_args}}
  std::map<const void *, std::vector<void *>> input_outside_addrs_;   // Key is virtual address from Data.
  std::map<const void *, std::vector<void *>> output_outside_addrs_;  // Key is virtual address from NetOutput.
  // {op_id, batch_label}
  std::map<int64_t, std::string> zero_copy_op_id_batch_label_;
  // {batch_label, addrs}
  std::map<std::string, std::set<uintptr_t>> zero_copy_batch_label_addrs_;

  std::vector<TaskInfoPtr> task_list_;
  // rt_moodel_handle
  rtModel_t rt_model_handle_;

  rtStream_t rt_model_stream_;

  bool is_inner_model_stream_;

  bool is_async_mode_;  // For NN execute, Async mode use rtMemcpyAsync on rt_model_stream_.

  // ACL queue schedule, save queue ids for Init.
  std::vector<TaskInfoPtr> cpu_task_list_;
  std::vector<uint32_t> input_queue_ids_;    // input queue ids created by caller.
  std::vector<uint32_t> output_queue_ids_;   // output queue ids created by caller.
  std::vector<uintptr_t> input_mbuf_list_;   // input mbuf created by dequeue task.
  std::vector<uintptr_t> output_mbuf_list_;  // output mbuf created by dequeue task.

  // save input/output tensor descriptor in maps
  std::map<std::string, ConstGeTensorDescPtr> data_op_input_tensor_desc_map_;
  std::map<std::string, ConstGeTensorDescPtr> data_op_output_tensor_desc_map_;

  uint64_t session_id_;

  uint32_t device_id_;

  std::mutex flowctrl_op_index_internal_map_mutex_;
  std::map<uint32_t, uint32_t> flowctrl_op_index_internal_map_;

  std::vector<rtStream_t> active_stream_list_;
  std::set<uint32_t> active_stream_indication_;

  std::shared_ptr<domi::ModelTaskDef> model_task_def_;
  std::set<uint32_t> aicpu_streams_;
  std::set<uint32_t> hcom_streams_;
  RuntimeParam runtime_param_;

  static std::mutex tvm_bin_mutex_;  // lock for tvm maps.
  static std::set<std::string> tvm_bin_kernel_;

  std::map<std::string, uint32_t> used_tbe_handle_map_;

  // for profiling task and graph info
  std::map<uint32_t, std::string> op_name_map_;
  std::vector<TaskDescInfo> task_desc_info_;
  ComputeGraphPtr compute_graph_;

  int64_t maxDumpOpNum_;
  // for data dump
  DataDumper data_dumper_;
  uint64_t iterator_count_;
  bool is_l1_fusion_enable_;

  bool known_node_ = false;
  uint32_t total_args_size_ = 0;
  void *args_ = nullptr;
  void *args_host_ = nullptr;
  std::map<const void *, void *> knonw_input_data_info_;
  std::map<const void *, void *> knonw_output_data_info_;

  vector<uint64_t> batch_size_;
};
}  // namespace ge
#endif  // GE_GRAPH_LOAD_NEW_MODEL_MANAGER_DAVINCI_MODEL_H_
