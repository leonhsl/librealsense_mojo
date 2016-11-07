// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_service_factory.h"

#include "librs_mojo/services/librs_mojo_service.h"

namespace librs {

// static
std::unique_ptr<shell::Service> CreateMojoLibRealsenseService(
    const base::Closure& quit_closure) {
  DLOG(INFO) << "Create mojo librealsense service instance on thread: "
             << base::PlatformThread::CurrentId();
  return std::unique_ptr<shell::Service>(
      new MojoLibRealsenseService(quit_closure));
}

}  // namespace librs
