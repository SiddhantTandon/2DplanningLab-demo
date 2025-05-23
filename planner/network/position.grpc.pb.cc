// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: position.proto

#include "position.pb.h"
#include "position.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace position {

static const char* PositionService_method_names[] = {
  "/position.PositionService/StreamPosition",
};

std::unique_ptr< PositionService::Stub> PositionService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< PositionService::Stub> stub(new PositionService::Stub(channel, options));
  return stub;
}

PositionService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_StreamPosition_(PositionService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::ClientReader< ::position::PositionResponse>* PositionService::Stub::StreamPositionRaw(::grpc::ClientContext* context, const ::position::PositionRequest& request) {
  return ::grpc::internal::ClientReaderFactory< ::position::PositionResponse>::Create(channel_.get(), rpcmethod_StreamPosition_, context, request);
}

void PositionService::Stub::async::StreamPosition(::grpc::ClientContext* context, const ::position::PositionRequest* request, ::grpc::ClientReadReactor< ::position::PositionResponse>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::position::PositionResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_StreamPosition_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::position::PositionResponse>* PositionService::Stub::AsyncStreamPositionRaw(::grpc::ClientContext* context, const ::position::PositionRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::position::PositionResponse>::Create(channel_.get(), cq, rpcmethod_StreamPosition_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::position::PositionResponse>* PositionService::Stub::PrepareAsyncStreamPositionRaw(::grpc::ClientContext* context, const ::position::PositionRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::position::PositionResponse>::Create(channel_.get(), cq, rpcmethod_StreamPosition_, context, request, false, nullptr);
}

PositionService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PositionService_method_names[0],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< PositionService::Service, ::position::PositionRequest, ::position::PositionResponse>(
          [](PositionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::position::PositionRequest* req,
             ::grpc::ServerWriter<::position::PositionResponse>* writer) {
               return service->StreamPosition(ctx, req, writer);
             }, this)));
}

PositionService::Service::~Service() {
}

::grpc::Status PositionService::Service::StreamPosition(::grpc::ServerContext* context, const ::position::PositionRequest* request, ::grpc::ServerWriter< ::position::PositionResponse>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace position

