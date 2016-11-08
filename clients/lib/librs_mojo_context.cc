// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/clients/lib/librs_mojo_context.h"

#include "base/bind.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread_task_runner_handle.h"
#include "services/shell/public/cpp/connector.h"

namespace librs {
namespace client {

namespace {

// TODO(leonhsl): move to proper place.
shell::Connector* g_connector_for_process = nullptr;

// TODO(leonhsl): move to proper place.
scoped_refptr<base::SequencedTaskRunner> g_connector_task_runner = nullptr;

}  // namespace

// static
void Context::SetConnector(shell::Connector* connector) {
  DCHECK(!g_connector_for_process);
  DCHECK(!g_connector_task_runner);
  DCHECK(connector);

  g_connector_for_process = connector;
  // |connector| access MUST happen on this thread.
  g_connector_task_runner = base::ThreadTaskRunnerHandle::Get();
}

Context::Context() : weak_factory_(this) {
  DCHECK(g_connector_for_process);
  DCHECK(g_connector_task_runner);

  if (base::ThreadTaskRunnerHandle::Get() == g_connector_task_runner) {
    // Already on connector thread. Maybe no need to support this, because
    // librealsense_wrapper is never expected to run on browser main thread or
    // renderer thread.
    DLOG(INFO) << "Client Context::Context(), is on global connector thread";
    ConnectOnConnectorThread();
  } else {
    DLOG(INFO)
        << "Client Context::Context(), is not on global connector thread";
    g_connector_task_runner->PostTask(
        FROM_HERE, base::Bind(&Context::ConnectOnConnectorThread,
                              weak_factory_.GetWeakPtr()));
  }
}

Context::~Context() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

int Context::GetDeviceCount() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DLOG(INFO) << "Client Context::GetDeviceCount() called: " << this;

  int count = -1;
  if (base::ThreadTaskRunnerHandle::Get() == g_connector_task_runner) {
    // Already on connector thread. Maybe no need to support this, because
    // librealsense_wrapper is never expected to run on browser main thread or
    // renderer thread.
    DLOG(INFO)
        << "Client Context::GetDeviceCount(), is on global connector thread";
    base::RunLoop loop;
    context_->GetDeviceCount(base::Bind(
        [](const base::Closure& quit_closure, int* out_count, int count) {
          *out_count = count;
          quit_closure.Run();
        },
        loop.QuitClosure(), &count));
    loop.Run();
  } else {
    DLOG(INFO) << "Client Context::GetDeviceCount(), is not on global "
                  "connector thread";
    base::WaitableEvent done_event(
        base::WaitableEvent::ResetPolicy::AUTOMATIC,
        base::WaitableEvent::InitialState::NOT_SIGNALED);
    g_connector_task_runner->PostTask(
        FROM_HERE, base::Bind(&Context::GetDeviceCountOnConnectorThread,
                              base::Unretained(this), &done_event, &count));
    done_event.Wait();
  }

  return count;
}

void Context::OnContextConnectionError() {
  // Signal all pending events which are expected to be signaled by mojo
  // function callbacks.
  std::set<base::WaitableEvent*> events;
  events.swap(pending_waitable_events_);
  for (base::WaitableEvent* event : events)
    event->Signal();
}

void Context::ConnectOnConnectorThread() {
  DCHECK(g_connector_task_runner->RunsTasksOnCurrentThread());
  DCHECK(!context_);

  shell::Connector::ConnectParams params(
      shell::Identity("mojo:librealsense", shell::mojom::kRootUserID));
  g_connector_for_process->ConnectToInterface(&params, &context_);
  context_.set_connection_error_handler(base::Bind(
      &Context::OnContextConnectionError, weak_factory_.GetWeakPtr()));
}

void Context::GetDeviceCountOnConnectorThread(base::WaitableEvent* done_event,
                                              int* out_count) {
  DLOG(INFO) << "Client Context::GetDeviceCountOnConnectorThread() called";
  DCHECK(context_);

  if (context_.encountered_error()) {
    *out_count = -1;
    done_event->Signal();
    return;
  }

  pending_waitable_events_.insert(done_event);
  context_->GetDeviceCount(base::Bind(&Context::GetDeviceCountCallback,
                                      base::Unretained(this), done_event,
                                      out_count));
}

void Context::GetDeviceCountCallback(base::WaitableEvent* done_event,
                                     int* out_count,
                                     int count) {
  pending_waitable_events_.erase(done_event);

  *out_count = count;
  done_event->Signal();
}

}  // namespace client
}  // namespace librs
