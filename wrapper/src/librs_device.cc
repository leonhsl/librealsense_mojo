// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "../include/librealsense_wrapper/librs_device.h"

#include <assert.h>

#include "librs_thunks_api.h"

namespace librs {
namespace wrapper {

Device::Device(Handle handle) : handle_(handle) {
  assert(handle_);
}

Device::~Device() {
  DeleteDeviceThunk(handle_);
}

const char* Device::GetName() const {
  return DeviceGetNameThunk(handle_);
}

}  // namespace wrapper
}  // namespace librs
