// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_device_impl.h"

#include <string>
#include <librealsense/rscore.hpp>  // NOLINT

namespace {

void HandleRsError(rs_error* error) {
  DLOG(ERROR) << "HandleRsError(), Error message: "
              << rs_get_error_message(error)
              << " Failed function: " << rs_get_failed_function(error)
              << " Failed args: " << rs_get_failed_args(error);
  rs_free_error(error);
}
}

namespace librs {

DeviceImpl::DeviceImpl(rs_device* rs_device_handle,
                       mojom::DeviceRequest request)
    : handle_(rs_device_handle), binding_(this, std::move(request)) {
  DCHECK(handle_);
}

DeviceImpl::~DeviceImpl() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void DeviceImpl::GetName(const GetNameCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: DeviceImpl::GetName()";

  // static int count = 0;
  // std::string name = "Dummy rs device name No." + std::to_string(++count);
  // callback.Run(name);

  rs_error* e = nullptr;
  const char* name = rs_get_device_name(handle_, &e);
  if (e) {
    HandleRsError(e);
    delete this;
  } else {
    callback.Run(name);
  }
}

}  // namespace librs
