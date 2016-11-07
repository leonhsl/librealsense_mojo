// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/wrapper/librs_context.h"

#include <assert.h>
#include <functional>

#include "librs_mojo/wrapper/librs_thunks_api.h"

namespace librs {
namespace wrapper {

Context::Context() {
  handle_ = CreateContextThunk();
  assert(handle_);
}

Context::~Context() {
  DeleteContextThunk(handle_);
}

void Context::GetDeviceCount(std::function<void(uint32_t)> callback) {
  GetDeviceCountThunk(handle_, callback);
}

}  // namespace wrapper
}  // namespace librs
