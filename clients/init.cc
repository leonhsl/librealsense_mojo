// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/clients/init.h"

#include <functional>

#include "librs_mojo/clients/lib/librs_mojo_context.h"
#include "librs_mojo/wrapper/librs_thunks.h"

namespace librs {
namespace client {

namespace {

void* CreateContextImpl() {
  // Caller owns this object and is responsible to call thunk function
  // DeleteContext() later to delete.
  return new Context();
}

void DeleteContextImpl(void* context) {
  delete static_cast<Context*>(context);
}

int GetDeviceCountImpl(void* context) {
  return static_cast<Context*>(context)->GetDeviceCount();
}

}  // namespace

void Init(shell::Connector* connector) {
  // Set connector to enable Context to connect with service manager.
  DCHECK(connector);
  Context::SetConnector(connector);

  // Set thunks implementation into wrapper library to enable it to do actual
  // work.
  librs::wrapper::LibraryThunks thunks = {sizeof(librs::wrapper::LibraryThunks),
                                          CreateContextImpl, DeleteContextImpl,
                                          GetDeviceCountImpl};
  librs::wrapper::SetThunks(&thunks);
}

}  // namespace client
}  // namespace librs
