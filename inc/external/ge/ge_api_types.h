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

#ifndef INC_EXTERNAL_GE_GE_API_TYPES_H_
#define INC_EXTERNAL_GE_GE_API_TYPES_H_

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <memory>

namespace ge {
// Option key: graph run mode
const char *const OPTION_GRAPH_RUN_MODE = "ge.graphRunMode";

// Option key: ome init
const char *const OPTION_EXEC_SESSION_ID = "ge.exec.sessionId";
const char *const OPTION_EXEC_DEVICE_ID = "ge.exec.deviceId";
const char *const OPTION_EXEC_JOB_ID = "ge.exec.jobId";
const char *const OPTION_EXEC_IS_USEHCOM = "ge.exec.isUseHcom";
const char *const OPTION_EXEC_IS_USEHVD = "ge.exec.isUseHvd";
const char *const OPTION_EXEC_RANK_ID = "ge.exec.rankId";
const char *const OPTION_EXEC_POD_NAME = "ge.exec.podName";
const char *const OPTION_EXEC_DEPLOY_MODE = "ge.exec.deployMode";
const char *const OPTION_EXEC_RANK_TABLE_FILE = "ge.exec.rankTableFile";
const char *const GE_AICPU_FLAG = "ge.aicpuFlag";
const char *const OPTION_EXEC_EXTERN_PLUGIN_PATH = "ge.soLoadPath";
const char *const OPTION_EXEC_ENABLE_DUMP = "ge.exec.enableDump";
const char *const OPTION_EXEC_DUMP_PATH = "ge.exec.dumpPath";
const char *const OPTION_EXEC_DUMP_STEP = "ge.exec.dumpStep";
const char *const OPTION_EXEC_DUMP_MODE = "ge.exec.dumpMode";
const char *const OPTION_EXEC_ENABLE_INCRE_BUILD = "ge.exec.enableIncreBuild";
const char *const OPTION_EXEC_INCRE_BUILD_CACHE_PATH = "ge.exec.increBuildCachePath";
// profiling flag
const char *const OPTION_EXEC_PROFILING_MODE = "ge.exec.profilingMode";
const char *const OPTION_EXEC_PROFILING_OPTIONS = "ge.exec.profilingOptions";
// Hccl flag, if ge.exec.hcclFlag =1, it means load plugin for opskernel, else:ge.exec.hcclFlag =0
const char *const OPTION_EXEC_HCCL_FLAG = "ge.exec.hcclFlag";
const char *const OPTION_EXEC_ATOMIC_FLAG = "ge.exec.enable_atomic";
const char *const OPTION_EXEC_DISABLE_REUSED_MEMORY = "ge.exec.disableReuseMemory";
const char *const OPTION_EXEC_ENABLE_TAILING_OPTIMIZATION = "ge.exec.isTailingOptimization";

// Option key: memory init
const char *const GRAPH_MEMORY_MAX_SIZE = "ge.graphMemoryMaxSize";
const char *const VARIABLE_MEMORY_MAX_SIZE = "ge.variableMemoryMaxSize";

// Configure stream num by Session constructor options param,
// its value should be int32_t type, default value is "1"
const std::string STREAM_NUM = "ge.streamNum";

// Configure add head stream to model.
// its value should be "0" or "1", default value is "0"
const std::string HEAD_STREAM = "ge.headStream";

// Configure perf level by Session constructor options param,
// its value please see enum PerfLevel, default value is "4"
const std::string PERF_LEVEL = "ge.perfLevel";

// Configure encrypt mode by Session constructor options param,
// its value should be int32_t type, default value is "-1"
const std::string ENCRYPT_MODE = "ge.encryptMode";

// configure ek file by Session constructor options param,
// its value should be file path, default value is ""
const std::string EK_FILE = "ge.ekFile";

// Configure cert file by Session constructor options param,
// its value should be file path, default value is ""
const std::string CERT_FILE = "ge.certFile";

// Configure hw key file by Session constructor options param,
// its value should be file path, default value is ""
const std::string HW_KEY_FILE = "ge.hwKeyFile";

// Configure private file by Session constructor options param,
// its value should be file path, default value is ""
const std::string PRIVATE_KEY_FILE = "ge.privateKeyFile";

// Configure framework type by Session constructor options param,
// its value please see enum FrameworkType, default value is "3"
const std::string FRAMEWORK_TYPE = "ge.frameworkType";

// Configure calibration info file by Session constructor options param,
// its value should be file path, default value is ""
const std::string CALIBRATION_CONF_FILE = "ge.calibrationConfFile";

// Configure insert op info file by Session constructor options param,
// its value should be file path, default value is ""
const std::string INSERT_OP_FILE = "ge.insertOpFile";

// Configure output node name by Session constructor options param,
// its value should be std::string type, default value is ""
const std::string OUTPUT_NODE_NAME = "ge.outputNodeName";

// Configure weight compress flag by Session constructor options param,
// its value should be "0" or "1", default value is "0"
const std::string COMPRESS_FLAG = "ge.compressFlag";

const std::string PRECISION_MODE = "ge.exec.precision_mode";

// Configure single op flag for FE
// its value should be "0" or "1", default value is "0"
const std::string SINGLE_OP_FLAG = "ge.exec.single_op";

// Configure train flag by Session constructor options param,
// its value should be "0" or "1", default value is "0"
const std::string TRAIN_FLAG = "ge.trainFlag";

// Configure run flag by Session constructor options param,
// its value should be "0" or "1", default value is "0"
const std::string RUN_FLAG = "ge.runFlag";

// Configure run flag by Session constructor options param,
// its value should be "0" or "1", default value is "0"
// this option is to enable local framework op feature
const std::string LOCAL_FMKOP_FLAG = "ge.enabledLocalFmkop";

// Configure run flag by Session constructor options param,
// its value should be a path
// this option is to obtain the TBE op plugin path
const std::string TBE_PLUGIN_PATH_FLAG = "ge.TBE_plugin_path";

// Configure run flag by Session constructor options param,
// its value should be a path
// this option is to obtain the DDK Version info
const std::string DDK_VERSION_FLAG = "ge.DDK_version";

// Configure run flag by Session constructor options param,
// its value should be a path
// this option is to obtain fe flag
const std::string GE_FE_FLAG = "ge.feFlag";

// Configure stream max parallel num only by Session constructor options param,
// its value should be stream:int, such as "DNN_V100:2,DNN_HCCL:3",
// default value is "1", such as "DNN_V100:1,DNN_HCCL:1"
// this option is to obtain stream max parallel num
const std::string STREAM_MAX_PARALLEL_NUM = "ge.streamMaxParallelNum";

// congigure outputDatatype to setting net output type
const std::string OUTPUT_DATATYPE = "ge.outputDatatype";

// congigure opSelectImplmode to setting op select implmode
const std::string OP_SELECT_IMPL_MODE = "ge.opSelectImplmode";

// congigure optypelist_for_implmode to setting which op use implmode
const std::string OPTYPELIST_FOR_IMPLMODE = "ge.optypelistForImplmode";

// configure whether to enable hcom parallel by session constructor options param,
// its value should be "0" or "1", default value is "0"
const std::string HCOM_PARALLEL = "ge.hcomParallel";

// configure whether to use dynamic batch size
const char *const kDynamicBatchSize = "ge.dynamicBatchSize";

// configure whether to use dynamic image size
const char *const kDynamicImageSize = "ge.dynamicImageSize";

// Configure auto tune mode, this option only take effect while AUTO_TUNE_FLAG is Y,
// example: GA|RL, support configure multiple, split by |
const std::string AUTO_TUNE_MODE = "ge.autoTuneMode";

// Configure soc version , example: "Ascend310"
const std::string SOC_VERSION = "ge.socVersion";

// Configure core type "VectorEngine", default value is "AIcoreEngine"
const std::string CORE_TYPE = "ge.engineType";

// Configure AICORE NUM
const std::string AICORE_NUM = "ge.aicoreNum";

// Configure L1FUSION
const std::string L1_FUSION = "ge.l1Fusion";

// Configure l1,l2,and others optimize option
const std::string BUFFER_OPTIMIZE = "ge.bufferOptimize";

// Configure Small Channel flag
const std::string ENABLE_SMALL_CHANNEL = "ge.enableSmallChannel";

// Configure Compress Weight flag
const std::string ENABLE_COMPRESS_WEIGHT = "ge.enableCompressWeight";

// Configure fusion switch file path
const std::string FUSION_SWITCH_FILE = "ge.fusionSwitchFile";

// Save original model
const std::string SAVE_ORIGINAL_MODEL = "ge.saveOriginalModel";

// Save original model file name
const std::string ORIGINAL_MODEL_FILE = "ge.originalModelFile";

// FE enable quant optimize
const std::string QUANT_OPTIMIZE = "ge.quantOptimize";

const char *const OPTION_GE_MAX_DUMP_FILE_NUM = "ge.maxDumpFileNum";
const char *const OPTION_GE_MAX_DUMP_FILE_SIZE = "ge.maxDumpFileSize";
const char *const OPTION_GE_MAX_DUMP_OP_NUM = "ge.maxDumpOpNum";

// Configure for print op pass
// Its value should be "0" or "1", default value is "1"
const char *const ENABLE_PRINT_OP_PASS = "ge.enablePrintOpPass";

// Configure whether to use single stream.
// Its value should be "true" or "false", default value is "false"
const char *const ENABLE_SINGLE_STREAM = "ge.enableSingleStream";

// Configure input fp16 nodes
const std::string INPUT_FP16_NODES = "ge.INPUT_NODES_SET_FP16";

// Graph run mode
enum GraphRunMode { PREDICTION = 0, TRAIN };

// Input/Output tensor info
struct InputTensorInfo {
  uint32_t data_type;         // data type
  std::vector<int64_t> dims;  // shape description
  void *data;                 // tensor data
  int64_t length;             // tensor length
};

struct OutputTensorInfo {
  uint32_t data_type;               // data type
  std::vector<int64_t> dims;        // shape description
  std::unique_ptr<uint8_t[]> data;  // tensor data
  int64_t length;                   // tensor length
  OutputTensorInfo() : data_type(0), dims({}), data(nullptr), length(0) {}
  OutputTensorInfo(OutputTensorInfo &&out)
      : data_type(out.data_type), dims(out.dims), data(std::move(out.data)), length(out.length) {}

  OutputTensorInfo &operator=(OutputTensorInfo &&out) {
    if (this != &out) {
      data_type = out.data_type;
      dims = out.dims;
      data = std::move(out.data);
      length = out.length;
    }
    return *this;
  }
  OutputTensorInfo(const OutputTensorInfo &) = delete;
  OutputTensorInfo &operator=(const OutputTensorInfo &) = delete;
};

using Status = uint32_t;
using RunAsyncCallback = std::function<void(Status, std::vector<ge::OutputTensorInfo> &)>;
// for ir build
namespace ir_option {
static const char *const INPUT_FORMAT = "input_format";
static const char *const INPUT_SHAPE = "input_shape";
static const char *const OP_NAME_MAP = "op_name_map";
static const char *const DYNAMIC_BATCH_SIZE = kDynamicBatchSize;
static const char *const DYNAMIC_IMAGE_SIZE = kDynamicImageSize;
static const char *const INSERT_OP_FILE = ge::INSERT_OP_FILE.c_str();
static const char *const PRECISION_MODE = ge::PRECISION_MODE.c_str();
static const char *const EXEC_DISABLE_REUSED_MEMORY = ge::OPTION_EXEC_DISABLE_REUSED_MEMORY;
static const char *const AUTO_TUNE_MODE = ge::AUTO_TUNE_MODE.c_str();
static const char *const CORE_TYPE = ge::CORE_TYPE.c_str();
static const char *const SOC_VERSION = ge::SOC_VERSION.c_str();
static const char *const ENABLE_SINGLE_STREAM = ge::ENABLE_SINGLE_STREAM;
static const char *const AICORE_NUM = ge::AICORE_NUM.c_str();
static const char *const FUSION_SWITCH_FILE = ge::FUSION_SWITCH_FILE.c_str();
static const char *const ENABLE_SMALL_CHANNEL = ge::ENABLE_SMALL_CHANNEL.c_str();
static const char *const QUANT_OPTIMIZE = ge::QUANT_OPTIMIZE.c_str();
static const char *const OP_SELECT_IMPL_MODE = ge::OP_SELECT_IMPL_MODE.c_str();
static const char *const OUTPUT_TYPE = ge::OUTPUT_DATATYPE.c_str();
static const char *const BUFFER_OPTIMIZE = ge::BUFFER_OPTIMIZE.c_str();
static const char *const ENABLE_COMPRESS_WEIGHT = ge::ENABLE_COMPRESS_WEIGHT.c_str();
static const char *const COMPRESS_WEIGHT_CONF = "compress_weight_conf";
static const char *const OUT_NODES = ge::OUTPUT_NODE_NAME.c_str();
static const char *const INPUT_FP16_NODES = ge::INPUT_FP16_NODES.c_str();
static const char *const LOG_LEVEL = "log";
static const char *const OPTYPELIST_FOR_IMPLMODE = ge::OPTYPELIST_FOR_IMPLMODE.c_str();

// for interface: aclgrphBuildModel
const std::set<std::string> ir_builder_suppported_options = {
  INPUT_FORMAT,       INPUT_SHAPE,    OP_NAME_MAP,    DYNAMIC_BATCH_SIZE,
  DYNAMIC_IMAGE_SIZE, INSERT_OP_FILE, PRECISION_MODE, EXEC_DISABLE_REUSED_MEMORY,
  AUTO_TUNE_MODE,     OUTPUT_TYPE,    OUT_NODES,      INPUT_FP16_NODES,
  LOG_LEVEL};
// for interface: aclgrphBuildInitialize
const std::set<std::string> global_options = {CORE_TYPE,
                                              SOC_VERSION,
                                              BUFFER_OPTIMIZE,
                                              ENABLE_COMPRESS_WEIGHT,
                                              COMPRESS_WEIGHT_CONF,
                                              PRECISION_MODE,
                                              EXEC_DISABLE_REUSED_MEMORY,
                                              AUTO_TUNE_MODE,
                                              ENABLE_SINGLE_STREAM,
                                              AICORE_NUM,
                                              FUSION_SWITCH_FILE,
                                              ENABLE_SMALL_CHANNEL,
                                              QUANT_OPTIMIZE,
                                              OP_SELECT_IMPL_MODE,
                                              OPTYPELIST_FOR_IMPLMODE};
}  // namespace ir_option
}  // namespace ge

#endif  // INC_EXTERNAL_GE_GE_API_TYPES_H_
