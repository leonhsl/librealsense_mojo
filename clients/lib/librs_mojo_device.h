// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_CLIENTS_LIB_LIBRS_MOJO_DEVICE_H_
#define LIBRS_MOJO_CLIENTS_LIB_LIBRS_MOJO_DEVICE_H_

#include <memory>

#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"
#include "base/threading/thread_task_runner_handle.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"

namespace librs {
namespace client {

// Must be created/accessed on the same thread.
class Device final {
 public:
  Device(mojom::DevicePtr device,
         scoped_refptr<base::SequencedTaskRunner> runner);
  ~Device();

  const std::string* GetName();

 private:
  class ConnectorThreadContext;

  std::unique_ptr<std::string> name_;

  scoped_refptr<base::SequencedTaskRunner> connector_thread_runner_;
  scoped_refptr<ConnectorThreadContext> context_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(Device);
};

}  // namespace client
}  // namespace librs

#endif  // LIBRS_MOJO_CLIENTS_LIB_LIBRS_MOJO_DEVICE_H_
