// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_DEVICE_IMPL_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_DEVICE_IMPL_H_

#include "base/macros.h"
#include "base/threading/thread_checker.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"
#include "mojo/public/cpp/bindings/strong_binding.h"

namespace librs {

class DeviceImpl : public mojom::Device {
 public:
  DeviceImpl(mojom::DeviceRequest request);
  ~DeviceImpl() override;

 private:
  // mojom::Device:
  void GetName(const GetNameCallback& callback) override;

  mojo::StrongBinding<mojom::Device> binding_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(DeviceImpl);
};

}  // namespace librs

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_DEVICE_IMPL_H_
