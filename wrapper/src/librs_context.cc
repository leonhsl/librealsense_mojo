// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "../include/librealsense_wrapper/librs_context.h"

#include <assert.h>
#include <functional>

#include "librs_thunks_api.h"

namespace librs {
namespace wrapper {

Context::Context() {
  handle_ = CreateContextThunk();
  assert(handle_);
}

Context::~Context() {
  for (auto& iter : devices_)
    delete iter.second;

  DeleteContextThunk(handle_);
}

int Context::GetDeviceCount() {
  return ContextGetDeviceCountThunk(handle_);
}

Device* Context::GetDevice(int index) {
  Device* device = nullptr;

  void* device_handle = ContextGetDeviceThunk(handle_, index);
  if (device_handle)
    device = new Device(device_handle);

  if (devices_.find(index) != devices_.end())
    delete devices_[index];
  devices_[index] = device;
  return device;
}

}  // namespace wrapper
}  // namespace librs
