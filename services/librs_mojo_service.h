// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_H_

#include <stdint.h>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/threading/thread_checker.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"
#include "librs_mojo/services/librs_mojo_services_export.h"
#include "mojo/public/cpp/bindings/binding_set.h"
#include "services/shell/public/cpp/service.h"
#include "services/shell/public/cpp/service_context_ref.h"

namespace librs {

class LIBRS_MOJO_SERVICES_EXPORT MojoLibRealsenseService
    : public NON_EXPORTED_BASE(shell::Service),
      public NON_EXPORTED_BASE(shell::InterfaceFactory<mojom::Context>),
      public NON_EXPORTED_BASE(mojom::Context) {
 public:
  explicit MojoLibRealsenseService(const base::Closure& quit_closure);
  ~MojoLibRealsenseService() final;

 private:
  // shell::Service implementation.
  void OnStart(const shell::Identity& identity) final;
  bool OnConnect(const shell::Identity& remote_identity,
                 shell::InterfaceRegistry* registry) final;
  bool OnStop() final;

  // shell::InterfaceFactory<mojom::Context> implementation.
  void Create(const shell::Identity& remote_identity,
              mojom::ContextRequest request) final;

  // mojom::Context implementation.
  void GetDeviceCount(const GetDeviceCountCallback& callback) final;

  shell::ServiceContextRefFactory ref_factory_;

  mojo::BindingSet<mojom::Context> bindings_;

  base::ThreadChecker thread_checker_;
};

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_H_
