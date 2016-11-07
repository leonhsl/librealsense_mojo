// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_H_
#define LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_H_

#include <functional>
#include <stddef.h>
#include <stdint.h>

#include "librs_mojo/wrapper/librs_wrapper_export.h"

namespace librs {
namespace wrapper {

#pragma pack(push, 8)
struct LibraryThunks {
  size_t size;  // Should be set to sizeof(LibraryThunks).
  void* (*CreateContext)();
  void (*DeleteContext)(void* context);
  void (*GetDeviceCount)(void* context, std::function<void(uint32_t)> callback);
};
#pragma pack(pop)

LIBRS_MOJO_WRAPPER_EXPORT size_t SetThunks(const struct LibraryThunks* thunks);

}  // namespace wrapper
}  // namespace librs

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_THUNKS_H_
