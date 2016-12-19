// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/clients/lib/librs_mojo_device.h"

#include <set>
#include <string>

#include "base/bind.h"
#include "base/synchronization/waitable_event.h"

namespace librs {
namespace client {

// A ref-counted object which owns Device state on the connector thread.
// Be created on user thread, destroyed on user thread or connector thread, but
// all other member functions run on connector thread, responsible to do mojo
// communications.
class Device::ConnectorThreadContext
    : public base::RefCountedThreadSafe<ConnectorThreadContext> {
 public:
  ConnectorThreadContext() {
    DLOG(INFO) << "Client Device::ConnectorThreadContext constructor";
    // This will be reattached by any of the connector thread functions on first
    // call.
    connector_thread_checker_.DetachFromThread();
  }

  void BindInterface(mojom::DevicePtrInfo ptr_info) {
    DLOG(INFO) << "Client Device::ConnectorThreadContext::BindInterface() ";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    DCHECK(!device_);

    device_.Bind(std::move(ptr_info));
    device_.set_connection_error_handler(
        base::Bind(&ConnectorThreadContext::OnDeviceConnectionError, this));
  }

  void Close() {
    DLOG(INFO) << "Client Device::ConnectorThreadContext::Close() called";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    // This will disconnect message pipe, to release reference ptr held by
    // connection error handler.
    device_.reset();
  }

  void GetName(base::WaitableEvent* done_event,
               std::unique_ptr<std::string>* out_name) {
    DLOG(INFO) << "Client Device::ConnectorThreadContext::GetName()";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    DCHECK(device_);

    if (device_.encountered_error()) {
      out_name->reset();
      done_event->Signal();
      return;
    }

    pending_waitable_events_.insert(done_event);
    device_->GetName(base::Bind(&ConnectorThreadContext::GetNameCallback, this,
                                done_event, out_name));
  }

 private:
  friend class base::RefCountedThreadSafe<ConnectorThreadContext>;

  ~ConnectorThreadContext() {
    // Be possible to happen on connector thread or user thread.
    DLOG(INFO) << "Client Device::ConnectorThreadContext destructor";
  }

  void OnDeviceConnectionError() {
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    // Signal all pending events which are expected to be signaled by mojo
    // function callbacks.
    std::set<base::WaitableEvent*> events;
    events.swap(pending_waitable_events_);
    for (base::WaitableEvent* event : events)
      event->Signal();
  }

  void GetNameCallback(base::WaitableEvent* done_event,
                       std::unique_ptr<std::string>* out_name,
                       const std::string& name) {
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    pending_waitable_events_.erase(done_event);

    out_name->reset(new std::string(name));
    done_event->Signal();
  }

  mojom::DevicePtr device_;

  std::set<base::WaitableEvent*> pending_waitable_events_;

  base::ThreadChecker connector_thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(ConnectorThreadContext);
};

Device::Device(mojom::DevicePtr device,
               scoped_refptr<base::SequencedTaskRunner> runner)
    : connector_thread_runner_(runner) {
  DLOG(INFO) << "Client Device constructor";
  // librealsense_wrapper is never expected to run on browser main thread or
  // renderer thread.
  DCHECK(base::ThreadTaskRunnerHandle::Get() != connector_thread_runner_.get());

  context_ = new ConnectorThreadContext();
  // Bind mojom::Device interface on connector thread.
  connector_thread_runner_->PostTask(
      FROM_HERE,
      base::Bind(&Device::ConnectorThreadContext::BindInterface, context_.get(),
                 base::Passed(device.PassInterface())));
}

Device::~Device() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "Client Device destructor";
  connector_thread_runner_->PostTask(
      FROM_HERE,
      base::Bind(&Device::ConnectorThreadContext::Close, context_.get()));
}

const std::string* Device::GetName() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "Client Device::GetName() called";

  if (!name_) {
    base::WaitableEvent done_event(
        base::WaitableEvent::ResetPolicy::AUTOMATIC,
        base::WaitableEvent::InitialState::NOT_SIGNALED);
    // Dispatch task to connector thread.
    connector_thread_runner_->PostTask(
        FROM_HERE, base::Bind(&Device::ConnectorThreadContext::GetName,
                              context_.get(), &done_event, &name_));
    done_event.Wait();
  }

  return name_.get();
}

}  // namespace client
}  // namespace librs
