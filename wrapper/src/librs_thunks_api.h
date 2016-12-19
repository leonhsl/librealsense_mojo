// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_API_H_
#define LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_API_H_

#include <functional>
#include <stdint.h>

namespace librs {
namespace wrapper {

void* CreateContextThunk();

void DeleteContextThunk(void* context);

int ContextGetDeviceCountThunk(void* context);

void* ContextGetDeviceThunk(void* context, int index);

void DeleteDeviceThunk(void* device);

const char* DeviceGetNameThunk(void* device);

}  // namespace wrapper
}  // namespace librs

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_API_H_
