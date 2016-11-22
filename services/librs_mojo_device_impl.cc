// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_device_impl.h"

#include <string>

namespace librs {

DeviceImpl::DeviceImpl(mojom::DeviceRequest request)
    : binding_(this, std::move(request)) {}

DeviceImpl::~DeviceImpl() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void DeviceImpl::GetName(const GetNameCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: DeviceImpl::GetName()";
  // TODO(leonhsl): Get real device name from librealsense.
  static int count = 0;
  std::string name = "Dummy rs device name No." + std::to_string(++count);
  callback.Run(name);
}

}  // namespace librs
