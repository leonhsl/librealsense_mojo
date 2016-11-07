// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/clients/lib/librs_mojo_context.h"

#include "base/bind.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/threading/thread_task_runner_handle.h"
#include "services/shell/public/cpp/connector.h"

namespace librs {
namespace client {

namespace {

// TODO(leonhsl): move to proper place.
shell::Connector* g_connector_for_process = nullptr;

// TODO(leonhsl): move to proper place.
scoped_refptr<base::SequencedTaskRunner> g_connector_task_runner = nullptr;

void ConnectOnUserThread(std::unique_ptr<shell::Connector> thread_connector,
                         mojom::ContextRequest request) {
  // In case that Context has been destroyed.
  if (!request.is_pending())
    return;

  shell::Connector::ConnectParams params(
      shell::Identity("mojo:librealsense", shell::mojom::kRootUserID));
  std::unique_ptr<shell::Connection> connection =
      thread_connector->Connect(&params);
  if (connection)
    connection->GetInterface(std::move(request));
}

void GetCloneConnectorAndConnect(
    scoped_refptr<base::SequencedTaskRunner> thread_runner,
    mojom::ContextRequest request) {
  DCHECK(g_connector_task_runner->RunsTasksOnCurrentThread());
  std::unique_ptr<shell::Connector> thread_connector =
      g_connector_for_process->Clone();
  thread_runner->PostTask(FROM_HERE, base::Bind(&ConnectOnUserThread,
                                                base::Passed(&thread_connector),
                                                base::Passed(&request)));
}

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

Context::Context() {
  DCHECK(g_connector_for_process);
  DCHECK(g_connector_task_runner);

  if (!base::MessageLoop::current()) {
    // Prepare the message loop by ourselves if caller thread has no message
    // loop ready.
    DLOG(INFO) << "Client Context::Context(), no message loop";
    message_loop_.reset(new base::MessageLoop());
  }

  mojom::ContextRequest request = mojo::GetProxy(&context_);
  if (base::ThreadTaskRunnerHandle::Get() == g_connector_task_runner) {
    // Already on connector thread.
    DLOG(INFO) << "Client Context::Context(), is on global connector thread";
    ConnectOnUserThread(g_connector_for_process->Clone(), std::move(request));
  } else {
    // Clone a connector on connector thread, and then use that clone connector
    // on this thread.
    DLOG(INFO) << "Client Context::Context(), is not on global connector "
                  "thread, will clone and connect";
    g_connector_task_runner->PostTask(
        FROM_HERE, base::Bind(&GetCloneConnectorAndConnect,
                              base::ThreadTaskRunnerHandle::Get(),
                              base::Passed(&request)));
  }

  DCHECK(context_);
}

Context::~Context() {
  DCHECK(thread_checker_.CalledOnValidThread());
}

void Context::GetDeviceCount(std::function<void(uint32_t)> callback) {
  DCHECK(thread_checker_.CalledOnValidThread());

  DLOG(INFO) << "Client Context::GetDeviceCount() called: " << this;
  context_->GetDeviceCount(base::Bind([](std::function<void(uint32_t)> callback,
                                         uint32_t count) { callback(count); },
                                      callback));
}

}  // namespace client
}  // namespace librs
