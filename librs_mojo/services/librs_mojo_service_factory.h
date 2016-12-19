// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_FACTORY_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_FACTORY_H_

#include <memory>

#include "base/callback_forward.h"
#include "librs_mojo/services/librs_mojo_services_export.h"
#include "services/shell/public/cpp/service.h"

namespace librs {

// Creates a MojoLibRealsenseService instance.
std::unique_ptr<shell::Service> LIBRS_MOJO_SERVICES_EXPORT
CreateMojoLibRealsenseService(const base::Closure& quit_closure);

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICE_FACTORY_H_
