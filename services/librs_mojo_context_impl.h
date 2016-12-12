// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_CONTEXT_IMPL_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_CONTEXT_IMPL_H_

#include <memory>

#include "base/macros.h"
#include "base/threading/thread_checker.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"
#include "mojo/public/cpp/bindings/strong_binding.h"
#include "services/shell/public/cpp/service_context_ref.h"

struct rs_context;

namespace librs {

// This implements mojom::Context interface and serves librs::client::Context.
// This holds a rs_context instance |handle_| to call librealsense APIs,
// and if any errors happened while calling librealsense APIs, we suppose
// |handle_| has become invalid and can't be used anymore, so we will delete
// self to close underlying message pipe then peer librs::client::Context will
// get a disconnection notification.
class ContextImpl : public mojom::Context {
 public:
  ContextImpl(rs_context* rs_context_handle,
              mojom::ContextRequest request,
              std::unique_ptr<shell::ServiceContextRef> connection_ref);
  ~ContextImpl() override;

 private:
  // mojom::Context:
  void GetDeviceCount(const GetDeviceCountCallback& callback) override;
  void GetDevice(int index, mojom::DeviceRequest request) override;

  rs_context* handle_;

  mojo::StrongBinding<mojom::Context> binding_;

  std::unique_ptr<shell::ServiceContextRef> connection_ref_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(ContextImpl);
};

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_CONTEXT_IMPL_H_
