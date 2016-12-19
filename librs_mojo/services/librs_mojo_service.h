// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_H_

#include <stdint.h>

#include "base/compiler_specific.h"
#include "base/threading/thread_checker.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"
#include "librs_mojo/services/librs_mojo_services_export.h"
#include "services/shell/public/cpp/service.h"
#include "services/shell/public/cpp/service_context_ref.h"

namespace librs {

class LIBRS_MOJO_SERVICES_EXPORT MojoLibRealsenseService
    : public NON_EXPORTED_BASE(shell::Service),
      public NON_EXPORTED_BASE(shell::InterfaceFactory<mojom::Context>) {
 public:
  explicit MojoLibRealsenseService(const base::Closure& quit_closure);
  ~MojoLibRealsenseService() final;

 private:
  // shell::Service:
  void OnStart(const shell::Identity& identity) final;
  bool OnConnect(const shell::Identity& remote_identity,
                 shell::InterfaceRegistry* registry) final;
  bool OnStop() final;

  // shell::InterfaceFactory<mojom::Context>:
  void Create(const shell::Identity& remote_identity,
              mojom::ContextRequest request) override;

  shell::ServiceContextRefFactory ref_factory_;

  base::ThreadChecker thread_checker_;
};

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_H_
