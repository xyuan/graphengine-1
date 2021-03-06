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

#include "single_op_parser.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "framework/common/debug/ge_log.h"
#include "common/util/error_manager/error_manager.h"
#include "common/ge_inner_error_codes.h"
#include "framework/common/util.h"
#include "graph/utils/tensor_utils.h"

using Json = nlohmann::json;
using std::map;
using std::string;
using std::vector;

namespace ge {
namespace {
constexpr char const *kKeyOp = "op";
constexpr char const *kKeyInputDesc = "input_desc";
constexpr char const *kKeyOutputDesc = "output_desc";
constexpr char const *kKeyAttr = "attr";
constexpr char const *kKeyName = "name";
constexpr char const *kKeyType = "type";
constexpr char const *kKeyShape = "shape";
constexpr char const *kKeyValue = "value";
constexpr char const *kKeyFormat = "format";
constexpr char const *kFileSuffix = ".om";
constexpr int kDumpJsonIndent = 2;

map<string, GeAttrValue::ValueType> kAttrTypeDict = {
  {"bool", GeAttrValue::VT_BOOL},
  {"int", GeAttrValue::VT_INT},
  {"float", GeAttrValue::VT_FLOAT},
  {"string", GeAttrValue::VT_STRING},
  {"list_bool", GeAttrValue::VT_LIST_BOOL},
  {"list_int", GeAttrValue::VT_LIST_INT},
  {"list_float", GeAttrValue::VT_LIST_FLOAT},
  {"list_string", GeAttrValue::VT_LIST_STRING},
  {"list_list_int", GeAttrValue::VT_LIST_LIST_INT},
};

map<string, DataType> kDataTypeDict = {
  {"bool", DT_BOOL},    {"int8", DT_INT8},     {"uint8", DT_UINT8}, {"int16", DT_INT16},   {"uint16", DT_UINT16},
  {"int32", DT_INT32},  {"uint32", DT_UINT32}, {"int64", DT_INT64}, {"uint64", DT_UINT64}, {"float16", DT_FLOAT16},
  {"half", DT_FLOAT16}, {"fp16", DT_FLOAT16},  {"float", DT_FLOAT}, {"float32", DT_FLOAT}, {"double", DT_DOUBLE},
};

map<string, Format> kFormatDict = {
  {"nchw", FORMAT_NCHW},           {"nhwc", FORMAT_NHWC},       {"nd", FORMAT_ND}, {"fractal_nz", FORMAT_FRACTAL_NZ},
  {"fractal_z", FORMAT_FRACTAL_Z}, {"nc1hwc0", FORMAT_NC1HWC0},
};
}  // namespace

template <typename T>
void SetAttrValue(const Json &j, SingleOpAttr &attr) {
  attr.value.SetValue<T>(j.at(kKeyValue).get<T>());
}

template <typename T>
T GetValue(const map<string, T> &dict, string &key, T default_val) {
  transform(key.begin(), key.end(), key.begin(), ::tolower);
  auto it = dict.find(key);
  if (it == dict.end()) {
    return default_val;
  }

  return it->second;
}

void from_json(const Json &j, SingleOpTensorDesc &desc) {
  desc.dims = j.at(kKeyShape).get<vector<int64_t>>();
  string format_str = j.at(kKeyFormat).get<string>();
  string type_str = j.at(kKeyType).get<string>();
  desc.format = GetValue(kFormatDict, format_str, FORMAT_RESERVED);
  desc.type = GetValue(kDataTypeDict, type_str, DT_UNDEFINED);
  auto tensor_name = j.find(kKeyName);
  if (tensor_name != j.end()) {
    desc.name = tensor_name->get<string>();
  }
}

void from_json(const Json &j, SingleOpAttr &attr) {
  attr.name = j.at(kKeyName).get<string>();
  attr.type = j.at(kKeyType).get<string>();
  auto it = kAttrTypeDict.find(attr.type);
  if (it == kAttrTypeDict.end()) {
    GELOGE(UNSUPPORTED, "Parse attr[%s] failed. Unsupported type: %s", attr.name.c_str(), attr.type.c_str());
    return;
  }

  switch (it->second) {
    case GeAttrValue::VT_BOOL:
      SetAttrValue<bool>(j, attr);
      break;
    case GeAttrValue::VT_INT:
      SetAttrValue<int64_t>(j, attr);
      break;
    case GeAttrValue::VT_FLOAT:
      SetAttrValue<float>(j, attr);
      break;
    case GeAttrValue::VT_STRING:
      SetAttrValue<string>(j, attr);
      break;
    case GeAttrValue::VT_LIST_BOOL:
      SetAttrValue<vector<bool>>(j, attr);
      break;
    case GeAttrValue::VT_LIST_INT:
      SetAttrValue<vector<int64_t>>(j, attr);
      break;
    case GeAttrValue::VT_LIST_FLOAT:
      SetAttrValue<vector<float>>(j, attr);
      break;
    case GeAttrValue::VT_LIST_STRING:
      SetAttrValue<vector<string>>(j, attr);
      break;
    case GeAttrValue::VT_LIST_LIST_INT:
      SetAttrValue<vector<vector<int64_t>>>(j, attr);
      break;
    default:
      GELOGE(UNSUPPORTED, "Parse attr[%s] failed. Unsupported type: %s", attr.name.c_str(), attr.type.c_str());
      break;
  }
}

void from_json(const Json &j, SingleOpDesc &desc) {
  desc.op = j.at(kKeyOp).get<string>();

  auto input_desc = j.find(kKeyInputDesc);
  if (input_desc != j.end()) {
    desc.input_desc = input_desc->get<vector<SingleOpTensorDesc>>();
  }

  auto output_desc = j.find(kKeyOutputDesc);
  if (output_desc != j.end()) {
    desc.output_desc = output_desc->get<vector<SingleOpTensorDesc>>();
  }

  auto attr_field = j.find(kKeyAttr);
  if (attr_field != j.end()) {
    desc.attrs = attr_field->get<vector<SingleOpAttr>>();
  }
}

Status SingleOpParser::ReadJsonFile(const std::string &file, Json &json_obj) {
  std::string real_path = RealPath(file.c_str());
  if (real_path.empty()) {
    ErrorManager::GetInstance().ATCReportErrMessage("E10023", {"value"}, {file});
    GELOGE(FAILED, "Input parameter[--singleop]'s value[%s] is not a valid path.", file.c_str());
    return INTERNAL_ERROR;
  }

  std::ifstream ifs(real_path);
  if (!ifs.is_open()) {
    ErrorManager::GetInstance().ATCReportErrMessage("E10024", {"value"}, {file});
    GELOGE(FAILED, "Open file[%s] provided in input parameter[--singleop] failed.", file.c_str());
    return FAILED;
  }
  try {
    ifs >> json_obj;
  } catch (const std::exception &e) {
    ErrorManager::GetInstance().ATCReportErrMessage("E10025", {"realpath", "errmsg"}, {real_path, e.what()});
    GELOGE(PARAM_INVALID, "Parse file[%s] provided in input parameter[--singleop] failed, exception = %s.",
           real_path.c_str(), e.what());
    return PARAM_INVALID;
  }

  ifs.close();
  return SUCCESS;
}

bool SingleOpParser::Validate(const SingleOpDesc &op_desc) {
  if (op_desc.op.empty()) {
    ErrorManager::GetInstance().ATCReportErrMessage("E10026");
    GELOGE(PARAM_INVALID, "Op name is empty");
    return false;
  }

  int index = 0;
  for (auto &tensor_desc : op_desc.input_desc) {
    if (tensor_desc.type == DT_UNDEFINED) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10027", {"input", "index"}, {"input", std::to_string(index)});
      GELOGE(false, "Input index[%d]'s dataType is invalid", index);
      return false;
    }

    if (tensor_desc.format == FORMAT_RESERVED) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10028", {"input", "index"}, {"input", std::to_string(index)});
      GELOGE(PARAM_INVALID, "Input index[%d]'s format is invalid", index);
      return false;
    }
    ++index;
  }

  index = 0;
  for (auto &tensor_desc : op_desc.output_desc) {
    if (tensor_desc.type == DT_UNDEFINED) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10027", {"input", "index"}, {"output", std::to_string(index)});
      GELOGE(PARAM_INVALID, "Output[%d] dataType is invalid", index);
      return false;
    }

    if (tensor_desc.format == FORMAT_RESERVED) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10028", {"input", "index"}, {"output", std::to_string(index)});
      GELOGE(PARAM_INVALID, "Output[%d] format is invalid", index);
      return false;
    }
    ++index;
  }

  for (auto &attr : op_desc.attrs) {
    if (attr.name.empty()) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10029");
      GELOGE(PARAM_INVALID, "attr name is empty");
      return false;
    }

    if (attr.value.IsEmpty()) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10030", {"attrname"}, {attr.name});
      GELOGE(PARAM_INVALID, "Parse attr \"%s\" failed. ", attr.name.c_str());
      return false;
    }
  }

  return true;
}

OpDesc *SingleOpParser::CreateOpDesc(const string &op_type) { return new (std::nothrow) OpDesc(op_type, op_type); }

Status SingleOpParser::ConvertToBuildParam(int index, const SingleOpDesc &single_op_desc,
                                           SingleOpBuildParam &build_param) {
  auto *op_desc = CreateOpDesc(single_op_desc.op);
  if (op_desc == nullptr) {
    GELOGE(MEMALLOC_FAILED, "Failed to create instance of opDesc");
    return MEMALLOC_FAILED;
  }

  std::stringstream file_name;
  file_name << index;
  file_name << "_" << single_op_desc.op;
  for (auto &desc : single_op_desc.input_desc) {
    file_name << "_" << desc.type << "_" << desc.format;
    for (auto dim : desc.dims) {
      file_name << "_" << dim;
    }
    GeTensorDesc ge_tensor_desc(GeShape(desc.dims), desc.format, desc.type);
    ge_tensor_desc.SetOriginFormat(desc.format);
    TensorUtils::SetRealDimCnt(ge_tensor_desc, desc.dims.size());
    TensorUtils::SetInputTensor(ge_tensor_desc, true);
    TensorUtils::SetOutputTensor(ge_tensor_desc, false);
    if (desc.name.empty()) {
      op_desc->AddInputDesc(ge_tensor_desc);
    } else {
      op_desc->AddInputDesc(desc.name, ge_tensor_desc);
    }
    if (desc.format == FORMAT_FRACTAL_NZ || desc.format == FORMAT_FRACTAL_Z) {
      ge_tensor_desc.SetFormat(FORMAT_ND);
      ge_tensor_desc.SetOriginFormat(FORMAT_ND);
    }
    build_param.inputs.emplace_back(ge_tensor_desc);
  }

  for (auto &desc : single_op_desc.output_desc) {
    file_name << "_" << desc.type << "_" << desc.format;
    for (auto dim : desc.dims) {
      file_name << "_" << dim;
    }

    GeTensorDesc ge_tensor_desc(GeShape(desc.dims), desc.format, desc.type);
    ge_tensor_desc.SetOriginFormat(desc.format);
    TensorUtils::SetRealDimCnt(ge_tensor_desc, desc.dims.size());
    TensorUtils::SetInputTensor(ge_tensor_desc, false);
    TensorUtils::SetOutputTensor(ge_tensor_desc, true);
    op_desc->AddOutputDesc(ge_tensor_desc);
    if (desc.format == FORMAT_FRACTAL_NZ || desc.format == FORMAT_FRACTAL_Z) {
      ge_tensor_desc.SetFormat(FORMAT_ND);
      ge_tensor_desc.SetOriginFormat(FORMAT_ND);
    }
    build_param.outputs.emplace_back(ge_tensor_desc);
  }

  for (const auto &attr : single_op_desc.attrs) {
    op_desc->SetAttr(attr.name, attr.value);
  }

  file_name << kFileSuffix;
  build_param.file_name = file_name.str();

  build_param.op_desc.reset(op_desc);
  return SUCCESS;
}

Status SingleOpParser::ParseSingleOpList(const std::string &file, std::vector<SingleOpBuildParam> &op_list) {
  Json single_op_list_json;
  auto ret = ReadJsonFile(file, single_op_list_json);
  if (ret != SUCCESS) {
    return ret;
  }

  int index = 0;
  for (const Json &single_op_json : single_op_list_json) {
    GELOGI("Parsing op[%d], jsonStr = %s", index, single_op_json.dump(kDumpJsonIndent).c_str());
    SingleOpDesc single_op_desc;
    try {
      single_op_desc = single_op_json;
    } catch (const nlohmann::json::exception &e) {
      ErrorManager::GetInstance().ATCReportErrMessage(
        "E10045", {"index", "jsonfile", "exception", "jsonStr"},
        {std::to_string(index), file, e.what(), single_op_json.dump(kDumpJsonIndent)});
      GELOGE(PARAM_INVALID, "Parse op[%d] failed when read json file[%s], exception[%s], jsonStr[%s]", index,
             file.c_str(), e.what(), single_op_json.dump(kDumpJsonIndent).c_str());
      return PARAM_INVALID;
    }

    if (!Validate(single_op_desc)) {
      ErrorManager::GetInstance().ATCReportErrMessage("E10046", {"index", "jsonfile"}, {std::to_string(index), file});
      GELOGE(PARAM_INVALID, "Validate op[%d] failed when read json file[%s].", index, file.c_str());
      return PARAM_INVALID;
    }

    SingleOpBuildParam param;
    ret = ConvertToBuildParam(index, single_op_desc, param);
    if (ret != SUCCESS) {
      return ret;
    }

    op_list.emplace_back(param);
    GELOGI("Parse op[%d] success", index);
    index += 1;
  }

  return SUCCESS;
}
}  // namespace ge
