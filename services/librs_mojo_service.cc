// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/services/librs_mojo_service.h"

#include <utility>

namespace librs {

MojoLibRealsenseService::MojoLibRealsenseService(
    const base::Closure& quit_closure)
    : ref_factory_(quit_closure) {}

MojoLibRealsenseService::~MojoLibRealsenseService() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void MojoLibRealsenseService::OnStart(const shell::Identity& identity) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: MojoLibRealsenseService::OnStart() "
                "called on thread: "
             << base::PlatformThread::CurrentId();
}

bool MojoLibRealsenseService::OnConnect(const shell::Identity& remote_identity,
                                        shell::InterfaceRegistry* registry) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: MojoLibRealsenseService::OnConnect() "
                "called on thread: "
             << base::PlatformThread::CurrentId();
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
  DLOG(INFO) << "LibRealsenseService: MojoLibRealsenseService::Create() called "
                "on thread: "
             << base::PlatformThread::CurrentId();
  bindings_.AddBinding(this, std::move(request));
}

void MojoLibRealsenseService::GetDeviceCount(
    const GetDeviceCountCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "LibRealsenseService: "
                "MojoLibRealsenseService::GetDeviceCount() called on thread: "
             << base::PlatformThread::CurrentId();
  // TODO(leonhsl): Get real count from librealsense.
  static int count = 0;
  callback.Run(++count);
}

}  // namespace librs
