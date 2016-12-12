// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_service.h"

#include <librealsense/rscore.hpp>  // NOLINT
#include <utility>

#include "librs_mojo/services/librs_mojo_context_impl.h"

namespace {

void HandleRsError(rs_error* error) {
  if (!error)
    return;

  DLOG(ERROR) << "HandleRsError(), Error message: "
              << rs_get_error_message(error)
              << " Failed function: " << rs_get_failed_function(error)
              << " Failed args: " << rs_get_failed_args(error);
  rs_free_error(error);
}
}

namespace librs {

MojoLibRealsenseService::MojoLibRealsenseService(
    const base::Closure& quit_closure)
    : ref_factory_(quit_closure) {}

MojoLibRealsenseService::~MojoLibRealsenseService() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void MojoLibRealsenseService::OnStart(const shell::Identity& identity) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: MojoLibRealsenseService::OnStart()";
}

bool MojoLibRealsenseService::OnConnect(const shell::Identity& remote_identity,
                                        shell::InterfaceRegistry* registry) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: MojoLibRealsenseService::OnConnect()";
  registry->AddInterface<mojom::Context>(this);
  return true;
}

bool MojoLibRealsenseService::OnStop() {
  DCHECK(thread_checker_.CalledOnValidThread());
  return true;
}

void MojoLibRealsenseService::Create(const shell::Identity& remote_identity,
                                     mojom::ContextRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: MojoLibRealsenseService::Create()";

  rs_error* e = nullptr;
  rs_context* rs_context_handle = rs_create_context(RS_API_VERSION, &e);
  if (e) {
    HandleRsError(e);
    // |request| will destroy silently here, thus the underlying message pipe
    // will be colsed and peer librs::client::Context will get a disconnection
    // notification.
  } else {
    new ContextImpl(rs_context_handle, std::move(request),
                    ref_factory_.CreateRef());
  }
}

}  // namespace librs
