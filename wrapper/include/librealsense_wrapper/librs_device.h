// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_WRAPPER_LIBRS_DEVICE_H_
#define LIBRS_MOJO_WRAPPER_LIBRS_DEVICE_H_

#include <stdint.h>

#include "librs_wrapper_export.h"

namespace librs {
namespace wrapper {

class LIBRS_MOJO_WRAPPER_EXPORT Device {
 public:
  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;

  const char* GetName() const;

 private:
  friend class Context;

  using Handle = void*;

  // Must call Context::GetDevice() to get device instances.
  Device(Handle handle);
  // All device instances are owned by Context instance.
  ~Device();

  Handle handle_ = nullptr;
};

}  // namespace wrapper
}  // namespace librs

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_DEVICE_H_
