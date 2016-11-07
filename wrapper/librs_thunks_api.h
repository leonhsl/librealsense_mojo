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

void GetDeviceCountThunk(void* context, std::function<void(uint32_t)> callback);

}  // namespace wrapper
}  // namespace librs

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_API_H_
