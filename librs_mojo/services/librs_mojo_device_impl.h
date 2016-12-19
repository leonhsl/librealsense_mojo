// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_DEVICE_IMPL_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_DEVICE_IMPL_H_

#include "base/macros.h"
#include "base/threading/thread_checker.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"
#include "mojo/public/cpp/bindings/strong_binding.h"

struct rs_device;

namespace librs {

// This implements mojom::Device interface and serves librs::client::Device.
// This holds a rs_device instance |handle_| to call librealsense APIs,
// and if any errors happened while calling librealsense APIs, we suppose
// |handle_| has become invalid and can't be used anymore, so we will delete
// self to close underlying message pipe then peer librs::client::Device will
// get a disconnection notification.
class DeviceImpl : public mojom::Device {
 public:
  DeviceImpl(rs_device* rs_device_handle, mojom::DeviceRequest request);
  ~DeviceImpl() override;

 private:
  // mojom::Device:
  void GetName(const GetNameCallback& callback) override;

  rs_device* handle_;

  mojo::StrongBinding<mojom::Device> binding_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(DeviceImpl);
};

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_DEVICE_IMPL_H_
