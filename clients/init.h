// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_CLIENTS_INIT_H_
#define LIBRS_MOJO_CLIENTS_INIT_H_

#include "librs_mojo/clients/librs_mojo_clients_export.h"

namespace shell {
class Connector;
}

namespace librs {
namespace client {

// Must be called firstly, on the thread of |connector|, and only once. Wrapper
// library becomes available only after this call.
LIBRS_MOJO_CLIENTS_EXPORT void Init(shell::Connector* connector);

}  // namespace client
}  // namespace librs

#endif  // LIBRS_MOJO_CLIENTS_INIT_H_
