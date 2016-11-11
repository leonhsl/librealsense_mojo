// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_CLIENTS_LIB_LIBRS_MOJO_CONTEXT_H_
#define LIBRS_MOJO_CLIENTS_LIB_LIBRS_MOJO_CONTEXT_H_

#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"

namespace shell {
class Connector;
}

namespace librs {
namespace client {

// Holds a mojo connection to librs.mojom.Context interface inside librealsense
// mojo service, must be created/accessed on the same thread.
class Context final {
 public:
  Context();
  ~Context();

  // Set the global service manager connector per process.
  static void SetConnector(shell::Connector* connector);

  int GetDeviceCount();

 private:
  class ConnectorThreadContext;

  base::ThreadChecker thread_checker_;

  scoped_refptr<ConnectorThreadContext> context_;

  DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace client
}  // namespace librs

#endif  // LIBRS_MOJO_CLIENTS_LIB_LIBRS_MOJO_CONTEXT_H_
