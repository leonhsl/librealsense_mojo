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

namespace rs {
class context;
}

namespace librs {

class ContextImpl : public mojom::Context {
 public:
  ContextImpl(mojom::ContextRequest request,
              std::unique_ptr<shell::ServiceContextRef> connection_ref);
  ~ContextImpl() override;

 private:
  // mojom::Context:
  void GetDeviceCount(const GetDeviceCountCallback& callback) override;
  void GetDevice(int index, mojom::DeviceRequest request) override;

  std::unique_ptr<rs::context> rs_context_;

  mojo::StrongBinding<mojom::Context> binding_;

  std::unique_ptr<shell::ServiceContextRef> connection_ref_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(ContextImpl);
};

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_CONTEXT_IMPL_H_
