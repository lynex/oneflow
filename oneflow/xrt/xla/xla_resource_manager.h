#ifndef ONEFLOW_XRT_XLA_XLA_RESOURCE_MANAGER_H_
#define ONEFLOW_XRT_XLA_XLA_RESOURCE_MANAGER_H_

#define EIGEN_USE_THREADS

#include "unsupported/Eigen/CXX11/Tensor"
#include "unsupported/Eigen/CXX11/ThreadPool"

#include "oneflow/xrt/types.h"
#include "oneflow/xrt/xla/xla_allocator.h"
#include "tensorflow/compiler/xla/client/local_client.h"
#include "tensorflow/compiler/xla/client/xla_builder.h"
#include "tensorflow/stream_executor/stream.h"

namespace oneflow {
namespace xrt {
namespace mola {

namespace resource_mgr {

se::Platform::Id GetPlatformId(const XrtDevice &device);

const se::Platform *GetPlatform(const XrtDevice &device);

Eigen::ThreadPoolDevice *GetOrCreateEigenHostDevice();

typedef void *StreamId;

DeviceBufferAllocator *GetOrCreateBufferAllocator(const XrtDevice &device,
                                                  const StreamId &stream_id,
                                                  se::Stream *stream,
                                                  int device_ordinal);

xla::LocalClient *GetOrCreateLocalClient(const XrtDevice &device);

}  // namespace resource_mgr

}  // namespace mola
}  // namespace xrt
}  // namespace oneflow

#endif  // ONEFLOW_XRT_XLA_XLA_RESOURCE_MANAGER_H_
