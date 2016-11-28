// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_context_impl.h"

#include <librealsense/rs.hpp>  // NOLINT

#include "librs_mojo/services/librs_mojo_device_impl.h"

namespace librs {

ContextImpl::ContextImpl(
    mojom::ContextRequest request,
    std::unique_ptr<shell::ServiceContextRef> connection_ref)
    : binding_(this, std::move(request)),
      connection_ref_(std::move(connection_ref)) {
  rs_context_.reset(new rs::context);
}

ContextImpl::~ContextImpl() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void ContextImpl::GetDeviceCount(const GetDeviceCountCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDeviceCount()";

  // static int count = 0;
  // count++;
  int count = rs_context_->get_device_count();
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDeviceCount(), rs "
                "context get device count: "
             << count;
  callback.Run(count);
}

void ContextImpl::GetDevice(int index, mojom::DeviceRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: ContextImpl::GetDevice()";
  // TODO(leonhsl): Get real device from librealsense.
  new DeviceImpl(std::move(request));
}

}  // namespace librs
