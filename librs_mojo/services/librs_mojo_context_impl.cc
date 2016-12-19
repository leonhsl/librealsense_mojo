// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_context_impl.h"

#include <librealsense/rscore.hpp>  // NOLINT

#include "librs_mojo/services/librs_mojo_device_impl.h"

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

ContextImpl::ContextImpl(
    rs_context* rs_context_handle,
    mojom::ContextRequest request,
    std::unique_ptr<shell::ServiceContextRef> connection_ref)
    : handle_(rs_context_handle),
      binding_(this, std::move(request)),
      connection_ref_(std::move(connection_ref)) {
  DCHECK(handle_);
}

ContextImpl::~ContextImpl() {
  DCHECK(thread_checker_.CalledOnValidThread());
  rs_delete_context(handle_, nullptr);
}

void ContextImpl::GetDeviceCount(const GetDeviceCountCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDeviceCount()";

  // static int count = 0;
  // count++;
  rs_error* e = nullptr;
  int count = rs_get_device_count(handle_, &e);
  if (e) {
    HandleRsError(e);
    delete this;
  } else {
    DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDeviceCount(), rs "
                  "context get device count: "
               << count;
    callback.Run(count);
  }
}

void ContextImpl::GetDevice(int index, mojom::DeviceRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDevice()";

  rs_error* e = nullptr;
  rs_device* rs_device_handle = rs_get_device(handle_, index, &e);
  if (e) {
    HandleRsError(e);
    delete this;
    // |request| will destroy silently here, thus the underlying message pipe
    // will be colsed and peer librs::client::Device will get a disconnection
    // notification.
  } else {
    new DeviceImpl(rs_device_handle, std::move(request));
  }
}

}  // namespace librs
