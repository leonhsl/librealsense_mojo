// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_context_impl.h"

#include "librs_mojo/services/librs_mojo_device_impl.h"

namespace librs {

ContextImpl::ContextImpl(
    mojom::ContextRequest request,
    std::unique_ptr<shell::ServiceContextRef> connection_ref)
    : binding_(this, std::move(request)),
      connection_ref_(std::move(connection_ref)) {}

ContextImpl::~ContextImpl() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void ContextImpl::GetDeviceCount(const GetDeviceCountCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDeviceCount()";
  // TODO(leonhsl): Get real count from librealsense.
  static int count = 0;
  callback.Run(++count);
}

void ContextImpl::GetDevice(int index, mojom::DeviceRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDevice()";
  // TODO(leonhsl): Get real device from librealsense.
  new DeviceImpl(std::move(request));
}

}  // namespace librs
