LOCAL_PATH := $(call my-dir)

LIBGE_LOCAL_SRC_FILES := \
    proto/fusion_model.proto \
    proto/optimizer_priority.proto \
    common/formats/format_transfers/datatype_transfer.cc \
    common/formats/format_transfers/format_transfer_c1hwncoc0_hwcn.cc \
    common/formats/format_transfers/format_transfer_dhwcn_fracz3D.cc \
    common/formats/format_transfers/format_transfer_dhwnc_fracz3D_transpose.cc \
    common/formats/format_transfers/format_transfer_fractal_nz.cc \
    common/formats/format_transfers/format_transfer_fractal_z.cc \
    common/formats/format_transfers/format_transfer_fractal_zz.cc \
    common/formats/format_transfers/format_transfer_fracz_hwcn.cc \
    common/formats/format_transfers/format_transfer_fracz_nchw.cc \
    common/formats/format_transfers/format_transfer_fracz_nhwc.cc \
    common/formats/format_transfers/format_transfer_hwcn_c1hwncoc0.cc \
    common/formats/format_transfers/format_transfer_nc1hwc0_nchw.cc \
    common/formats/format_transfers/format_transfer_nc1hwc0_nhwc.cc \
    common/formats/format_transfers/format_transfer_nchw_nc1hwc0.cc \
    common/formats/format_transfers/format_transfer_nhwc_nc1hwc0.cc \
    common/formats/format_transfers/format_transfer_transpose.cc \
    common/formats/formats.cc \
    common/formats/utils/formats_trans_utils.cc \
    common/fp16_t.cc \
    common/ge/plugin_manager.cc\
    common/helper/model_cache_helper.cc \
    common/profiling/profiling_manager.cc \
    engine_manager/dnnengine_manager.cc \
    ge_local_engine/engine/host_cpu_engine.cc \
    generator/ge_generator.cc \
    generator/generator_api.cc \
    graph/build/graph_builder.cc \
    graph/build/label_allocator.cc \
    graph/build/logical_stream_allocator.cc \
    graph/build/model_builder.cc \
    graph/build/run_context.cc \
    graph/build/stream_allocator.cc \
    graph/build/stream_graph_optimizer.cc \
    graph/build/task_generator.cc \
    graph/common/bcast.cc \
    graph/common/omg_util.cc \
    graph/common/transop_util.cc \
    graph/execute/graph_execute.cc \
    graph/label/case_label_maker.cc \
    graph/label/if_label_maker.cc \
    graph/label/label_maker.cc \
    graph/label/partitioned_call_label_maker.cc \
    graph/label/while_label_maker.cc \
    graph/load/graph_loader.cc \
    graph/load/new_model_manager/cpu_queue_schedule.cc \
    graph/load/new_model_manager/data_dumper.cc \
    graph/load/new_model_manager/data_inputer.cc \
    graph/load/new_model_manager/davinci_model.cc \
    graph/load/new_model_manager/davinci_model_parser.cc \
    graph/load/new_model_manager/model_manager.cc \
    graph/load/new_model_manager/model_utils.cc \
    graph/load/new_model_manager/aipp_utils.cc \
    graph/load/new_model_manager/task_info/end_graph_task_info.cc \
    graph/load/new_model_manager/task_info/event_record_task_info.cc \
    graph/load/new_model_manager/task_info/event_wait_task_info.cc \
    graph/load/new_model_manager/task_info/fusion_start_task_info.cc \
    graph/load/new_model_manager/task_info/fusion_stop_task_info.cc \
    graph/load/new_model_manager/task_info/hccl_task_info.cc \
    graph/load/new_model_manager/task_info/kernel_ex_task_info.cc \
    graph/load/new_model_manager/task_info/kernel_task_info.cc \
    graph/load/new_model_manager/task_info/label_set_task_info.cc \
    graph/load/new_model_manager/task_info/label_switch_by_index_task_info.cc \
    graph/load/new_model_manager/task_info/label_goto_ex_task_info.cc \
    graph/load/new_model_manager/task_info/memcpy_addr_async_task_info.cc \
    graph/load/new_model_manager/task_info/memcpy_async_task_info.cc \
    graph/load/new_model_manager/task_info/profiler_trace_task_info.cc \
    graph/load/new_model_manager/task_info/stream_active_task_info.cc \
    graph/load/new_model_manager/task_info/stream_switch_task_info.cc \
    graph/load/new_model_manager/task_info/stream_switchn_task_info.cc \
    graph/load/new_model_manager/task_info/super_kernel/super_kernel.cc \
    graph/load/new_model_manager/task_info/super_kernel/super_kernel_factory.cc   \
    graph/load/new_model_manager/task_info/task_info.cc \
    graph/load/new_model_manager/tbe_handle_store.cc \
    graph/load/new_model_manager/zero_copy_task.cc \
    graph/load/output/output.cc \
    graph/manager/graph_context.cc \
    graph/manager/graph_manager.cc \
    graph/manager/graph_manager_utils.cc \
    graph/manager/graph_mem_allocator.cc \
    graph/manager/graph_caching_allocator.cc \
    graph/manager/graph_var_manager.cc \
    graph/manager/model_manager/event_manager.cc        \
    graph/manager/trans_var_data_utils.cc \
    graph/manager/util/debug.cc                       \
    graph/manager/util/hcom_util.cc                 \
    graph/manager/util/rt_context_util.cc               \
    graph/manager/util/variable_accelerate_ctrl.cc               \
    graph/optimize/graph_optimize.cc \
    graph/optimize/optimizer/allreduce_fusion_pass.cc \
    graph/optimize/summary_optimize.cc \
    graph/partition/engine_place.cc \
    graph/partition/graph_partition.cc \
    graph/passes/addn_pass.cc \
    graph/passes/aicpu_constant_folding_pass.cc \
    graph/passes/assert_pass.cc \
    graph/passes/atomic_addr_clean_pass.cc \
    graph/partition/dynamic_shape_partition.cc \
    graph/passes/base_pass.cc \
    graph/passes/cast_remove_pass.cc \
    graph/passes/cast_translate_pass.cc \
    graph/passes/common_subexpression_elimination_pass.cc \
    graph/passes/transop_symmetry_elimination_pass.cc \
    graph/passes/compile_nodes_pass.cc \
    graph/passes/constant_folding_pass.cc \
    graph/passes/constant_fuse_same_pass.cc \
    graph/passes/control_trigger_pass.cc \
    graph/passes/dimension_adjust_pass.cc \
    graph/passes/dimension_compute_pass.cc \
    graph/passes/dropout_pass.cc \
    graph/passes/hccl_group_pass.cc \
    graph/passes/switch_fusion_pass.cc \
    graph/passes/switch_split_pass.cc \
    graph/passes/enter_pass.cc \
    graph/passes/flow_ctrl_pass.cc \
    host_kernels/transpose_kernel.cc \
    host_kernels/add_kernel.cc \
    host_kernels/broadcast_args_kernel.cc \
    host_kernels/broadcast_gradient_args_kernel.cc \
    host_kernels/cast_kernel.cc \
    host_kernels/concat_offset_kernel.cc \
    host_kernels/concat_v2_kernel.cc \
    host_kernels/dynamic_stitch_kernel.cc \
    host_kernels/empty_kernel.cc \
    host_kernels/expanddims_kernel.cc \
    host_kernels/fill_kernel.cc \
    host_kernels/floordiv_kernel.cc \
    host_kernels/floormod_kernel.cc \
    host_kernels/gather_v2_kernel.cc  \
    host_kernels/greater_kernel.cc \
    host_kernels/kernel_utils.cc \
    host_kernels/maximum_kernel.cc \
    host_kernels/mul_kernel.cc \
    host_kernels/pack_kernel.cc \
    host_kernels/permute_kernel.cc \
    host_kernels/range_kernel.cc \
    host_kernels/rank_kernel.cc \
    host_kernels/reduce_prod_kernel.cc \
    host_kernels/reshape_kernel.cc \
    host_kernels/rsqrt_kernel.cc \
    host_kernels/shape_kernel.cc \
    host_kernels/shape_n_kernel.cc \
    host_kernels/size_kernel.cc \
    host_kernels/slice_d_kernel.cc \
    host_kernels/slice_kernel.cc \
    host_kernels/squeeze_kernel.cc \
    host_kernels/unsqueeze_kernel.cc \
    host_kernels/ssd_prior_box_kernel.cc \
    host_kernels/strided_slice_kernel.cc \
    host_kernels/sub_kernel.cc \
    host_kernels/transdata_kernel.cc \
    host_kernels/unpack_kernel.cc \
    graph/passes/folding_pass.cc \
    graph/passes/get_original_format_pass.cc \
    graph/passes/guarantee_const_pass.cc \
    graph/passes/hccl_memcpy_pass.cc \
    graph/passes/identify_reference_pass.cc \
    graph/passes/identity_pass.cc \
    graph/passes/infershape_pass.cc \
    graph/passes/isolated_op_remove_pass.cc \
    graph/passes/iterator_op_pass.cc \
    graph/passes/link_gen_mask_nodes_pass.cc \
    graph/passes/merge_pass.cc \
    graph/passes/multi_batch_pass.cc \
    graph/passes/net_output_pass.cc \
    graph/passes/next_iteration_pass.cc \
    graph/passes/no_use_reshape_remove_pass.cc \
    graph/passes/pass_manager.cc \
    graph/passes/pass_utils.cc \
    graph/passes/permute_pass.cc \
    graph/passes/placeholder_with_default_pass.cc \
    graph/passes/prevent_gradient_pass.cc \
    graph/passes/print_op_pass.cc \
    graph/passes/prune_pass.cc \
    graph/passes/ctrl_edge_transfer_pass.cc \
    graph/passes/replace_with_empty_const_pass.cc \
    graph/passes/reshape_remove_pass.cc \
    graph/passes/reshape_recovery_pass.cc \
    graph/passes/resource_pair_add_control_pass.cc \
    graph/passes/resource_pair_remove_control_pass.cc \
    graph/passes/same_transdata_breadth_fusion_pass.cc \
    graph/passes/save_pass.cc \
    graph/passes/shape_operate_op_remove_pass.cc \
    graph/passes/snapshot_pass.cc \
    graph/passes/stop_gradient_pass.cc \
    graph/passes/subgraph_pass.cc \
    graph/passes/data_pass.cc \
    graph/passes/switch_data_edges_bypass.cc \
    graph/passes/switch_logic_remove_pass.cc \
    graph/passes/switch_op_pass.cc \
    graph/passes/switch_dead_branch_elimination.cc \
    graph/passes/replace_transshape_pass.cc \
    graph/passes/transop_breadth_fusion_pass.cc \
    graph/passes/transop_depth_fusion_pass.cc \
    graph/passes/transop_nearby_allreduce_fusion_pass.cc \
    graph/passes/transop_without_reshape_fusion_pass.cc \
    graph/passes/transpose_transdata_pass.cc \
    graph/passes/unused_const_pass.cc \
    graph/passes/unused_op_remove_pass.cc \
    graph/passes/var_is_initialized_op_pass.cc \
    graph/passes/parallel_concat_start_op_pass.cc \
    graph/passes/cond_pass.cc \
    graph/passes/cond_remove_pass.cc \
    graph/passes/for_pass.cc \
    graph/passes/variable_format_pass.cc \
    graph/passes/variable_op_pass.cc \
    graph/passes/variable_prepare_op_pass.cc \
    graph/passes/variable_ref_delete_op_pass.cc \
    graph/passes/variable_ref_useless_control_out_delete_pass.cc \
    graph/preprocess/graph_preprocess.cc \
    graph/preprocess/insert_op/ge_aipp_op.cc \
    graph/preprocess/insert_op/util_insert_aipp_op.cc \
    graph/preprocess/multi_batch_copy_graph.cc \
    init/gelib.cc \
    model/ge_model.cc \
    model/ge_root_model.cc \
    omm/csa_interact.cc \
    opskernel_manager/ops_kernel_manager.cc \
    session/inner_session.cc \
    session/session_manager.cc \
    single_op/single_op.cc \
    single_op/single_op_manager.cc \
    single_op/single_op_model.cc \
    single_op/stream_resource.cc \
    single_op/task/build_task_utils.cc \
    single_op/task/op_task.cc \
    single_op/task/tbe_task_builder.cc \
    single_op/task/aicpu_task_builder.cc \
    hybrid/common/tensor_value.cc                                        \
    hybrid/common/npu_memory_allocator.cc                                \
    hybrid/executor/rt_callback_manager.cc                               \
    hybrid/executor/node_state.cc                                        \
    hybrid/executor/node_done_manager.cc                                 \
    hybrid/executor/hybrid_profiler.cc                                   \
    hybrid/executor/hybrid_model_executor.cc                             \
    hybrid/executor/hybrid_model_async_executor.cc                       \
    hybrid/executor/hybrid_execution_context.cc                          \
    hybrid/executor/worker/task_compile_engine.cc                        \
    hybrid/executor/worker/shape_inference_engine.cc                     \
    hybrid/executor/worker/execution_engine.cc                           \
    hybrid/model/hybrid_model.cc                                         \
    hybrid/model/hybrid_model_builder.cc                                 \
    hybrid/model/node_item.cc                                            \
    hybrid/node_executor/aicore/aicore_node_executor.cc                  \
    hybrid/node_executor/aicore/aicore_op_task.cc                        \
    hybrid/node_executor/aicore/aicore_task_builder.cc                   \
    hybrid/node_executor/aicore/aicore_task_compiler.cc                  \
    hybrid/node_executor/aicpu/aicpu_ext_info.cc                         \
    hybrid/node_executor/aicpu/aicpu_node_executor.cc                    \
    hybrid/node_executor/compiledsubgraph/known_node_executor.cc         \
    hybrid/node_executor/hostcpu/ge_local_node_executor.cc               \
    hybrid/node_executor/node_executor.cc                                \
    hybrid/node_executor/task_context.cc                                 \
    hybrid/hybrid_davinci_model.cc                                       \
    executor/ge_executor.cc \

LIBCLIENT_LOCAL_SRC_FILES := \
    proto/ge_api.proto \
    client/ge_api.cc \

RUNNER_LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) ./ \
    $(LOCAL_PATH)/../ \
    $(LOCAL_PATH)/../../ \
    $(TOPDIR)inc \
    $(TOPDIR)inc/common \
    $(TOPDIR)inc/external \
    $(TOPDIR)inc/external/graph \
    $(TOPDIR)inc/framework \
    $(TOPDIR)inc/framework/common \
    $(TOPDIR)inc/graph \
    $(TOPDIR)inc/runtime \
    $(TOPDIR)libc_sec/include \
    $(TOPDIR)ops/built-in/op_proto/inc \
    proto/fwk_adapter.proto \
    proto/ge_ir.proto \
    proto/insert_op.proto \
    proto/om.proto \
    proto/op_mapping_info.proto \
    proto/task.proto \
    proto/tensorflow/attr_value.proto \
    proto/tensorflow/function.proto \
    proto/tensorflow/graph.proto \
    proto/tensorflow/node_def.proto \
    proto/tensorflow/op_def.proto \
    proto/tensorflow/resource_handle.proto \
    proto/tensorflow/tensor.proto \
    proto/tensorflow/tensor_shape.proto \
    proto/tensorflow/types.proto \
    proto/tensorflow/versions.proto \
    third_party/json/include \
    third_party/opencv/include \
    third_party/protobuf/include \



#compiler for GeRunner
include $(CLEAR_VARS)

LOCAL_MODULE := libge_runner

LOCAL_CFLAGS += -DPROTOBUF_INLINE_NOT_IN_HEADERS=0 -DREUSE_MEMORY=1 -O2
LOCAL_CFLAGS += -DFMK_SUPPORT_DUMP -DDAVINCI_SUPPORT_PROFILING -DDAVINCI_CLOUD
ifeq ($(DEBUG), 1)
LOCAL_CFLAGS += -g -O0
endif


LOCAL_C_INCLUDES := $(RUNNER_LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(LIBGE_LOCAL_SRC_FILES)
LOCAL_SRC_FILES += $(LIBCLIENT_LOCAL_SRC_FILES)

LOCAL_STATIC_LIBRARIES := libge_memory \

LOCAL_SHARED_LIBRARIES := \
    libc_sec \
    libprotobuf \
    libslog \
    libmmpa \
    libgraph \
    libregister \
    libge_common \
    libhccl \
    libmsprof \
    liberror_manager \


LOCAL_LDFLAGS := -lrt -ldl

LOCAL_SHARED_LIBRARIES += \
    libruntime \
    libresource \

include $(BUILD_HOST_SHARED_LIBRARY)


# add engine_conf.json to host
include $(CLEAR_VARS)

LOCAL_MODULE := engine_conf.json

LOCAL_SRC_FILES := engine_manager/engine_conf.json

LOCAL_MODULE_CLASS := ETC

LOCAL_INSTALLED_PATH := $(HOST_OUT_ROOT)/engine_conf.json
include $(BUILD_HOST_PREBUILT)

# add optimizer_priority.pbtxt to host
include $(CLEAR_VARS)

LOCAL_MODULE := optimizer_priority.pbtxt

LOCAL_SRC_FILES := opskernel_manager/optimizer_priority.pbtxt

LOCAL_MODULE_CLASS := ETC

LOCAL_INSTALLED_PATH := $(HOST_OUT_ROOT)/optimizer_priority.pbtxt
include $(BUILD_HOST_PREBUILT)

#compiler for GeRunner static lib
include $(CLEAR_VARS)

LOCAL_MODULE := libge_runner

LOCAL_CFLAGS += -DPROTOBUF_INLINE_NOT_IN_HEADERS=0 -DREUSE_MEMORY=1 -O2
LOCAL_CFLAGS += -DFMK_SUPPORT_DUMP -DDAVINCI_SUPPORT_PROFILING -DDAVINCI_CLOUD

LOCAL_CFLAGS += -g -O0


LOCAL_C_INCLUDES := $(RUNNER_LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(LIBGE_LOCAL_SRC_FILES)
LOCAL_SRC_FILES += $(LIBCLIENT_LOCAL_SRC_FILES)

LOCAL_STATIC_LIBRARIES := libge_memory \

LOCAL_SHARED_LIBRARIES := \
    libc_sec \
    libslog \
    libmmpa \
    libhccl \
    libmsprof \

LOCAL_LDFLAGS := -lrt -ldl

LOCAL_SHARED_LIBRARIES += \
    libruntime \
    libresource \

include $(BUILD_HOST_STATIC_LIBRARY)

#compiler for GeRunner static lib device
include $(CLEAR_VARS)

LOCAL_MODULE := libge_runner

LOCAL_CFLAGS += -DPROTOBUF_INLINE_NOT_IN_HEADERS=0 -DREUSE_MEMORY=1 -O2
LOCAL_CFLAGS += -DFMK_SUPPORT_DUMP -DDAVINCI_SUPPORT_PROFILING -DDAVINCI_CLOUD

LOCAL_CFLAGS += -g -O0

LOCAL_C_INCLUDES := $(RUNNER_LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(LIBGE_LOCAL_SRC_FILES)
LOCAL_SRC_FILES += $(LIBCLIENT_LOCAL_SRC_FILES)

LOCAL_STATIC_LIBRARIES := libge_memory \

LOCAL_SHARED_LIBRARIES := \
    libc_sec \
    libslog \
    libmmpa \
    libhccl \
    libmsprof \

LOCAL_LDFLAGS := -lrt -ldl

LOCAL_SHARED_LIBRARIES += \
    libruntime \
    libresource \

include $(BUILD_STATIC_LIBRARY)
