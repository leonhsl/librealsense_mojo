// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_WRAPPER_LIBRS_CONTEXT_H_
#define LIBRS_MOJO_WRAPPER_LIBRS_CONTEXT_H_

#include "librs_mojo/wrapper/librs_wrapper_export.h"

#include <functional>
#include <stdint.h>

namespace librs {
namespace wrapper {

class LIBRS_MOJO_WRAPPER_EXPORT Context {
 public:
  Context();
  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

  ~Context();

  int GetDeviceCount();

 private:
  using Handle = void*;
  Handle handle_ = nullptr;
};

}  // namespace wrapper
}  // namespace librs

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_CONTEXT_H_
