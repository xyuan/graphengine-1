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

#ifndef GE_HYBRID_MODEL_HYBRID_MODEL_BUILDER_H_
#define GE_HYBRID_MODEL_HYBRID_MODEL_BUILDER_H_

#include <vector>
#include <queue>
#include <memory>
#include "framework/common/ge_inner_error_codes.h"
#include "graph/load/new_model_manager/task_info/task_info.h"
#include "graph/node.h"
#include "hybrid/model/hybrid_model.h"
#include "hybrid/model/node_item.h"
#include "model/ge_model.h"

namespace ge {
class VarManager;
namespace hybrid {
class HybridModelBuilder {
 public:
  explicit HybridModelBuilder(HybridModel &hybrid_model);
  ~HybridModelBuilder() = default;
  Status Build();

 private:
  static Status UpdateAnchorStatus(const NodePtr &node);
  static Status DoUnlinkDataAnchors(const OutDataAnchorPtr &out_data_anchor, const InDataAnchorPtr &in_data_anchor);
  static Status DoLinkDataAnchors(OutDataAnchorPtr &out_data_anchor, InDataAnchorPtr &in_data_anchor);
  static NodePtr GetPeerNode(const InDataAnchorPtr &in_data_anchor);
  static Status GetParentNodeOutputIndex(const OpDesc &op_desc, int index, uint32_t &out_index);
  static Status GetPeerNodeAcrossSubGraphs(const NodePtr &data_node, NodePtr &peer_node, int &peer_out_index);
  static Status HandleDtString(const GeTensor &tensor, void *var_addr);
  static Status MergeInputNodes(ComputeGraph &compute_graph);
  static Status MergeNetOutputNode(ComputeGraph &compute_graph);
  static Status MergeSubgraphs(ComputeGraph &root_graph, ComputeGraphPtr &merged_graph);
  static Status InitWeights();

  Status ValidateParams();
  Status LoadGraph();
  Status LoadTasks();
  Status ParsePartitionedCall(NodeItem &node_item);
  Status ParseNetOutput(const NodeItem &node_item);
  Status BuildNoteItem(const NodePtr &node, NodeItem &node_item);
  Status GetOrCreateNodeItem(const NodePtr &node, NodeItem **node_item);
  Status ParseDependentInputNodes(NodeItem &node_item, const std::vector<string> &dependencies);
  Status ResolveRootNodes();
  Status IndexTaskDefs();
  Status IndexSpecialNodes();
  Status InitRuntimeParams();
  Status InitModelMem();
  Status TransAllVarData();
  Status CopyVarData();
  Status VarNodeToTensor(const NodePtr &var_node, std::unique_ptr<TensorValue> &tensor);
  Status InitConstantOps();
  Status InitVariableTensors();

  const char *GetGraphName() const { return graph_name_.c_str(); }

  const NodeItem *GetNodeItem(const NodePtr &node) const;
  NodeItem *MutableNodeItem(const NodePtr &node);

  GeRootModelPtr ge_root_model_;
  std::string graph_name_;
  std::map<int, std::unique_ptr<TensorValue>> weights_;
  HybridModel &hybrid_model_;
  std::map<NodePtr, std::vector<std::pair<int, NodePtr>>> node_ref_inputs_;

  RuntimeParam &runtime_param_;
  VarManager *var_manager_ = nullptr;
};
}  // namespace hybrid
}  // namespace ge
#endif  // GE_HYBRID_MODEL_HYBRID_MODEL_BUILDER_H_
