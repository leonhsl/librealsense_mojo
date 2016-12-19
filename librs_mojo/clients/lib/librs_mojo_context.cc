// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/clients/lib/librs_mojo_context.h"

#include <memory>
#include <set>

#include "base/bind.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread_task_runner_handle.h"
#include "librs_mojo/clients/lib/librs_mojo_device.h"
#include "librs_mojo/interfaces/librealsense.mojom.h"
#include "services/shell/public/cpp/connector.h"

namespace librs {
namespace client {

namespace {

// TODO(leonhsl): move to proper place.
shell::Connector* g_connector_for_process = nullptr;

// TODO(leonhsl): move to proper place.
scoped_refptr<base::SequencedTaskRunner> g_connector_task_runner = nullptr;

}  // namespace

// A ref-counted object which owns Context state on the connector thread.
// Be created on user thread, destroyed on user thread or connector thread, but
// all other member functions run on connector thread, responsible to do mojo
// communications.
class Context::ConnectorThreadContext
    : public base::RefCountedThreadSafe<ConnectorThreadContext> {
 public:
  ConnectorThreadContext() {
    DLOG(INFO) << "Client Context::ConnectorThreadContext constructor";
    // This will be reattached by any of the connector thread functions on first
    // call.
    connector_thread_checker_.DetachFromThread();
  }

  void Connect() {
    DLOG(INFO) << "Client Context::ConnectorThreadContext::Connect() called";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    DCHECK(!context_);

    shell::Connector::ConnectParams params(
        shell::Identity("mojo:librealsense", shell::mojom::kRootUserID));
    g_connector_for_process->ConnectToInterface(&params, &context_);
    context_.set_connection_error_handler(
        base::Bind(&ConnectorThreadContext::OnContextConnectionError, this));
  }

  void Close() {
    DLOG(INFO) << "Client Context::ConnectorThreadContext::Close() called";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    // This will disconnect message pipe, to release reference ptr held by
    // connection error handler.
    context_.reset();
  }

  void GetDeviceCount(base::WaitableEvent* done_event, int* out_count) {
    DLOG(INFO)
        << "Client Context::ConnectorThreadContext::GetDeviceCount() called";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    DCHECK(context_);

    if (context_.encountered_error()) {
      *out_count = -1;
      done_event->Signal();
      return;
    }

    pending_waitable_events_.insert(done_event);
    context_->GetDeviceCount(
        base::Bind(&ConnectorThreadContext::GetDeviceCountCallback, this,
                   done_event, out_count));
  }

  void GetDevice(int index, mojom::DeviceRequest request) {
    DLOG(INFO) << "Client Context::ConnectorThreadContext::GetDevice() called";
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    DCHECK(context_);

    context_->GetDevice(index, std::move(request));
  }

 private:
  friend class base::RefCountedThreadSafe<ConnectorThreadContext>;

  ~ConnectorThreadContext() {
    // Be possible to happen on connector thread or user thread.
    DLOG(INFO) << "Client Context::ConnectorThreadContext destructor";
  }

  void OnContextConnectionError() {
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    // Signal all pending events which are expected to be signaled by mojo
    // function callbacks.
    std::set<base::WaitableEvent*> events;
    events.swap(pending_waitable_events_);
    for (base::WaitableEvent* event : events)
      event->Signal();
  }

  void GetDeviceCountCallback(base::WaitableEvent* done_event,
                              int* out_count,
                              int count) {
    DCHECK(connector_thread_checker_.CalledOnValidThread());
    pending_waitable_events_.erase(done_event);

    *out_count = count;
    done_event->Signal();
  }

  mojom::ContextPtr context_;

  std::set<base::WaitableEvent*> pending_waitable_events_;

  base::ThreadChecker connector_thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(ConnectorThreadContext);
};

// static
void Context::SetConnector(shell::Connector* connector) {
  DCHECK(!g_connector_for_process);
  DCHECK(!g_connector_task_runner);
  DCHECK(connector);

  g_connector_for_process = connector;
  // |connector| access MUST happen on this thread.
  g_connector_task_runner = base::ThreadTaskRunnerHandle::Get();
}

Context::Context() {
  DLOG(INFO) << "Client Context constructor";
  DCHECK(g_connector_for_process);
  DCHECK(g_connector_task_runner);
  // librealsense_wrapper is never expected to run on browser main thread or
  // renderer thread.
  DCHECK(base::ThreadTaskRunnerHandle::Get() != g_connector_task_runner);

  context_ = new ConnectorThreadContext();
  // Connect to mojo service on connector thread.
  g_connector_task_runner->PostTask(
      FROM_HERE,
      base::Bind(&Context::ConnectorThreadContext::Connect, context_.get()));
}

Context::~Context() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "Client Context destructor";
  g_connector_task_runner->PostTask(
      FROM_HERE,
      base::Bind(&Context::ConnectorThreadContext::Close, context_.get()));
}

int Context::GetDeviceCount() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "Client Context::GetDeviceCount() called";

  int count = -1;
  base::WaitableEvent done_event(
      base::WaitableEvent::ResetPolicy::AUTOMATIC,
      base::WaitableEvent::InitialState::NOT_SIGNALED);
  // Dispatch task to connector thread.
  g_connector_task_runner->PostTask(
      FROM_HERE, base::Bind(&Context::ConnectorThreadContext::GetDeviceCount,
                            context_.get(), &done_event, &count));
  done_event.Wait();

  return count;
}

Device* Context::GetDevice(int index) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "Client Context::GetDevice() called";

  mojom::DevicePtr device;
  // Get mojom::Device interface on connector thread.
  g_connector_task_runner->PostTask(
      FROM_HERE,
      base::Bind(&Context::ConnectorThreadContext::GetDevice, context_.get(),
                 index, base::Passed(mojo::GetProxy(&device))));
  return new Device(std::move(device), g_connector_task_runner);
}

}  // namespace client
}  // namespace librs
