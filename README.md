# librealsense_mojo
Multiplexing support for librealsense via mojo

### interfaces
Definitions of mojom interfaces.

### services - librealsense_service
* Be a chromium component, called as librealsense_service, build is controlled by a gn switch: enable_mojo_librealsense.
* Produce an embedded mojo service running inside browser process, would be process-singleton and on a dedicated thread.
* librealsense_service exposes mojo interfaces, and implement them by requesting librealsense.


### clients - librealsense_client
* Be a chromium component, called as librealsense_client, build is controlled by a gn switch: enable_mojo_librealsense.
* Can be initialized in any process who can provide a service manager connector, so that librealsense_client can employ the connector to connect with librealsense_service. Browser process example:
```
  shell::Connector* connector = content::MojoShellConnection::GetForProcess()->GetConnector();
  librs::client::Init(connector);
```
* librealsense_client initializes librealsense_wrapper by providing thunks implementation to it.
* librealsense_client consumes mojo interfaces exposed by librealsense_service.

### wrapper - librealsense_wrapper
* Be a shared library, called as librealsense_wrapper, build is controlled by a gn switch: enable_mojo_librealsense.
* librealsense_wrapper is initialized by librealsense_client.
* librealsense_wrapper exports APIs somehow equivalent with librealsense, but implements these APIs by requesting librealsense_client.
* Original users(WebRTC and librealsense worker library) of librealsense can link/consume librealsense_wrapper now instead.
```
  auto rs_context = base::MakeUnique<librs::wrapper::Context>();
  DLOG(INFO) << "GetDeviceCount returned: " << rs_context->GetDeviceCount();
```
* Can run on multiple threads at the same time. But is forbidden to running on thread of the connector got by librs::client::Init(), usually such thread would be browser UI thread or render main thread.
